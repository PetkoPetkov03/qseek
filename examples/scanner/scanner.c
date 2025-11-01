#include <parser/scanner/scanner.h>
#include <stdio.h>
#include <cast.h>

int main()
{
  char* path[2];
  path[0] = "./";
  path[1] = "test.txt";
  scanner_t* scanner = init_scanner(path);

  scanner_load(scanner);

  char symbol;
  while((symbol = scanner_get_next_char(scanner)) != 0) {
    
  }
  
  return 0;
}
