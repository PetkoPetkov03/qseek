#ifndef __TOKENS__
#define __TOKENS__

#include <stddef.h>
#include <stdio.h>

#define MAXINTSIZE 1000000
#define MAXSTRNGLEN 8

typedef enum __token_t__ {
    string, intconst, text,
    semicolon, period, quotas, othersy
} token_t;

static const char* TOKEN_NAMES[] = {
    "STRING",
    "INTCONST",
    "TEXT",
    "SEMICOLON",
    "PERIOD",
    "QUOTAS",
    "OTHERSY"
};

typedef struct __token_ident__ {
    char lexeme[MAXSTRNGLEN];
    size_t size;
} token_ident;

typedef struct __token_constant__ {
    int constant;
} token_constant;

typedef union __token_data__ {
    char symb;
    token_ident ident;
    token_constant constant;
} token_data;

typedef struct __token__ {
    token_t token_type;
    token_data data;
} token;

inline static void print_token(token token) 
{
    char buff[100];
    printf("Token-Type: [%s] lex [%s]\n", TOKEN_NAMES[token.token_type], token.token_type == intconst ?  (
    sprintf(buff, "%i", token.data.constant.constant)  ), buff : token.data.ident.lexeme);
}

#endif
