#include <indexing.h>
#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <parser/context.h>
#include <parser/ruleset.h>
#include <stl/hashset.h>
#include <dirent.h>
#include <stl/array.h>

ArrayWrapper parse_dirs_fdoc(str path)
{
    ArrayWrapper wrapper = { .data = {0}, .size = 0 };
    struct dirent* ent;
    DIR* fd = opendir(path);

    if(fd == NULL) {
        fprintf(stderr, "Dir unable to open");
        exit(EXIT_FAILURE);
    }

    size_t size = 0;
    while((ent = readdir(fd)) != NULL) {
        if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
            str delim = strrchr(ent->d_name, '.');
            if(strcmp((delim+1), "html") == 0) {
                size_t fpathSize = strlen(path) + 1 + strlen(ent->d_name) + 1;
                char* full_path = malloc(fpathSize);

                int path_construct =
                sprintf(full_path, "%s/%s\0", path, ent->d_name);

                if(path_construct == 1) {
                    fprintf(stderr, "string construct error\n");
                    exit(EXIT_FAILURE);
                }

                (wrapper.data[size]) = doc_init(full_path);
                printf("PUSHED: %d:%s\n", size, full_path);
                size++;

                printf("Ent: %s\n", ent->d_name);
            }
        }
    }


    closedir(fd);
    wrapper.size = size;
    return wrapper;
}

void fill_words(ArrayWrapper wrapper)
{
    void** docs = wrapper.data;

    size_t size = wrapper.size;

    for(size_t i = 0; i < size; i++) {
        doc* doci = (doc*)docs[i];
        str hstr = strdup(doci->name);
        context_t ctx = context_init(hstr);

        set_mode(&ctx, HTML);

        configure_linking(&ctx);

        parse_func(&ctx);

        if(context_is_errors_thrown(&ctx)) {
            abort_and_dump(&ctx);
        }

        size_t cwordindex = 0;
        for(size_t j = 0; j < ctx.tokensSize; j++) {
            doci->name = hstr;
            if(ctx.tokens[j].token_type == htmlstr) {
                doci->words[cwordindex++] =
                strdup(ctx.tokens[j].data.ident.lexeme);
                doci->words_size++;
            }
        }

        printf("name ptr BEFORE: %p %s\n", (void*)doci->name, doci->name);
        context_clean(&ctx);
        printf("name ptr AFTER: %p %s\n", (void*)doci->name, doci->name);

    }
}

typedef struct {
    str doc_name;
    int used_index;
} DocToIndexMap;

int find_instances_for(int map[1024][1024],
DocToIndexMap used[], str word, str doc_name)
{

    return 0;
}

int main()
{
    char cwd[PATH_MAX] = {0};
    getcwd(cwd, PATH_MAX);

    strcat(cwd, "/cache");

    size_t p_size = strlen(cwd);

    ArrayWrapper files = parse_dirs_fdoc(cwd);

    fill_words(files);

    for(size_t filei = 0; filei < files.size; filei++) {
        void** docs = files.data;
        doc* cDoc = (doc*)docs[filei];
        printf("Path: %s\n", cDoc->name);
        for(size_t wordi = 0; wordi < cDoc->words_size; wordi++) {
            printf("Doc %ld Word %s\n", filei+1, cDoc->words[wordi]);
        }
    }


    int map[10][1024];
    DocToIndexMap used[1024];


    return 0;
}
