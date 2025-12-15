#include <httpc/dns.h>
#include <httpc/httpclient.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stl/cast.h>
#include <parser/context.h>
#include <parser/tokenizer/tokenizer.h>
#include <parser/scanner/scanner.h>
#include <parser/analyzer/analyzer.h>
#include <parser/ruleset.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

void replace_char(char* src, char find, char replace)
{
    for(;*src!='\0'; src++) {
        if(*src == find) {
            *src = replace;
        }
    }
}

void client_fetch_input()
{
    char input[200];
    char cwd[PATH_MAX];

    getcwd(cwd, PATH_MAX);

    printf("Enter url: ");
    scanf("%s", input);

    char buffer[PATH_MAX] = {0};
    strcat(buffer, cwd);
    strcat(buffer, "/test.txt");

    int inputFd = open(buffer, O_CREAT|O_RDWR);
    strcat(input, "\n");

    write(inputFd, input, sizeof(input)/sizeof(input[0]));

    close(inputFd);
}

void client_parse_url()
{
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);

    strcat(cwd, "/test.txt");

    context_t ctx = context_init(cwd);

    set_mode(&ctx, URL);

    configure_linking(&ctx);

    parse_func(&ctx);

    if(context_is_error_trown(&ctx)) {
        abort_and_dump(&ctx);
    }

    context_clean(&ctx);
}

void client_fetch_content()
{
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);
    strcat(cwd, "/test.txt");

    int fd = open(cwd, O_RDONLY);

    char temp;
    char buffer[PATH_MAX] = {0};
    int buff_size = 0;

    while(read(fd, &temp, 1) > 0) {
        buffer[buff_size++] = temp;
    }
    buffer[++buff_size] = '\0';

    char protocol[32] = "";
    char hostname[256] = "";
    char path[PATH_MAX] = "";

    size_t i=0, pi = 0;
    while(buffer[i] != ':' && buffer[i] != '\0') {
        protocol[pi++] = buffer[i++];
    }
    i+=3;
    pi = 0;

    while(buffer[i] != '/' && buffer[i] != '\0') {
        hostname[pi++] = buffer[i++];
    }
    i++;
    pi = 0;

    while(buffer[i] != '\0') {
        path[pi++] = buffer[i++];
    }

    char* ip = htoip(hostname);

    printf("%s://%s||%s/%s\n", protocol, hostname, ip, path);

    int status, valread, clientfd;

    struct sockaddr_in serv_addr;

    char httpfilebuffer[MAXSTRNGLEN];

    if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Socket creation error\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);

    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }

    if((status =
    connect(clientfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        fprintf(stderr, "Connection failed!\n");
        exit(EXIT_FAILURE);
    }

    char scwd[PATH_MAX];
    getcwd(scwd, PATH_MAX);
    strcat(scwd, "/cache");
    struct stat sb;

    if(stat(scwd, &sb) != 0) {
        printf("Stat running\n");
        char command[512];
        memset(command, '\0', sizeof(command));
        strcat(command, "mkdir ");
        printf("RUN scwd: %s\n", scwd);
        strcat(command, scwd);
        system(command);
    }

    replace_char(path, '\n', '\0');

    char request[512];
    snprintf(request, sizeof(request),
             "GET /%s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:146.0)"
             "Gecko/20100101 Firefox/146.0\r\n"
             "Accept: text/html,"
             "application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
             "Accept-Language: en-US,en;q=0.5"
             "Priority: u=0, i"
             "Connection: close\r\n"
             "\r\n",
             path, hostname);

    printf("Constructed reqest: %s\n", request);

    send(clientfd, request, strlen(request), 0);

    char htmlfilebuff[BUFFER_SIZE];
    int header_done = 0;
    ssize_t n;

    char* hname= ((strtok(hostname, ".")));

    sprintf(scwd, "%s/%s.html", scwd, hname);
    int htmlfd = open(scwd, O_CREAT|O_WRONLY|O_TRUNC, 0644);

    if(htmlfd < 0) {
        fprintf(stderr, "html file coud'nt open\n");
        exit(EXIT_FAILURE);
    }

    while((n = recv(clientfd, htmlfilebuff, sizeof(htmlfilebuff), 0)) > 0) {
        if(!header_done) {
            char *body = strstr(htmlfilebuff, "\r\n\r\n");

            if(body) {
                body += 4;

                printf("Body: %s\n", body);

                write(htmlfd, body, n - (body - htmlfilebuff));
                header_done = 1;
            }
        } else {
            printf("FileBuff: %s\n", htmlfilebuff);
            write(htmlfd, htmlfilebuff, n);
        }
    }


    char browser_buffer[PATH_MAX];
    sprintf(browser_buffer, "xdg-open %s", scwd);
    system(browser_buffer);
    close(clientfd);
    close(fd);
}
