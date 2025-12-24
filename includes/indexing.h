#ifndef __WEB_INDEXING__
#define __WEB_INDEXING__
#include <stl/btree.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

typedef uint8_t u8;


void tfidf(char* word, u8);

#endif
