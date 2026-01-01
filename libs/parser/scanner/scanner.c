#include <stl/queue.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <parser/scanner/scanner.h>
#include <parser/context.h>
#include <stl/cast.h>
#include <stdio.h>
#include <string.h>

scanner_t* init_scanner()
{

  queue_t* scanner_q = init_queue();

  scanner_t* scanner = castp(malloc, scanner_t, sizeof(scanner_t));

  scanner->scanner_queue = scanner_q;

  return scanner;
}

void scanner_load(scanner_t* scanner)
{
  char* path = strdup(scanner->ctx->file_path);
  int fd = open(path, O_RDONLY);

  if(fd == -1) {
      perror("file not found");
      exit(EXIT_FAILURE);
  }

  char buff;
  while((read(fd, &buff, 1)) != 0) {
      char* p = malloc(1);
      *p = buff;
      if(*p == '\0' || *p == '\r' || *p == '\n') {
          free(p);
          continue;
      }
      enqueue(scanner->scanner_queue, p);
  }

  close(fd);
  free(path);
}

char scanner_get_next_char(scanner_t* scanner)
{
    char symbol = cast_safe(dequeue, char, scanner->scanner_queue);

    return symbol;
}

void scanner_clean(scanner_t *scanner)
{
  queue_clear(scanner->scanner_queue);
  free(scanner);
}
