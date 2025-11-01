#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <parser/scanner/scanner.h>
#include <cast.h>

scanner_t* init_scanner(char* file[])
{
  char* path = "";

  strcat(path, file[0]);
  strcat(path, file[1]);

  int fd = open(path, O_RDONLY);

  queue_t* scanner_q = init_queue();

  scanner_t* scanner = castp(malloc, scanner_t, sizeof(scanner_t));

  scanner->scanner_queue = scanner_q;
  scanner->fd = fd;

  return scanner;
}

void scanner_load(scanner_t* scanner)
{
  char buff[1];
  while((read(scanner->fd, buff, 1)) != 0) {
    enqueue(scanner->scanner_queue, &buff[1]);
  }

  close(scanner->fd);
}

char scanner_get_next_char(scanner_t* scanner)
{
  char symbol = cast_safe(dequeue, char, scanner->scanner_queue);

  return symbol;
}
