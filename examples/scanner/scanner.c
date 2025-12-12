#include "parser/context.h"
#include <parser/scanner/scanner.h>
#include <stdio.h>
#include <stl/cast.h>
#include <stdlib.h>

int main()
{
    context_t ctx =
    context_init("/home/petkosuse/Dev/qseek/examples/scanner/test.txt");
  scanner_t* scanner = init_scanner();

  link_scanner_instance(&ctx, scanner);

  scanner_load(scanner);

  char symbol;
  while((symbol = scanner_get_next_char(scanner)) != 0) {
      printf("char: %c\n", symbol);
  }

  context_clean(&ctx);

  return 0;
}
