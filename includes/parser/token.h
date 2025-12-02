#ifndef __TOKENS__
#define __TOKENS__

#include <stddef.h>
#include <stdio.h>
#include <panic_handler.h>

#define MAXINTSIZE 1000000
#define MAXSTRNGLEN 8
#define MAXSTRNGLENURL 50

typedef enum __token_t__ {
    // unrecognized default token
    unrecognized,
    /*
    Default tokens
    */
    string, intconst, text,
    semicolon, period, quotas, othersy,

    /*
    URL tokens
    */
    digits, xalphas, xpalphas, safe,
    extra, hex, separator, dotseparator,
    esc, column, httpd, tcp
} token_t;

static const char* TOKEN_NAMES[] = {
    // unrecognized default token
    "UNRECOGNIZED",
    /*
    Default token names
    */
    "STRING",
    "INTCONST",
    "TEXT",
    "SEMICOLON",
    "PERIOD",
    "QUOTAS",
    "OTHERSY",

    /*
    URL token names
    */
    "DIGITS",
    "XALPHAS",
    "XPALPHAS",
    "SAFE",
    "EXTRA",
    "HEX",
    "SEPARATOR",
    "DOTSEPARATOR",
    "ESCAPE",
    "COLUMN",
    "HTTP/HTTPS",
    "TCP"
};

typedef struct __token_ident__ {
    char lexeme[MAXSTRNGLENURL];
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

inline static void print_token(token t)
{
    char buff[100];
    printf("Token-Type: [%s] lex [%s]\n", TOKEN_NAMES[t.token_type],
    (t.token_type == intconst || t.token_type == digits) ?
    (sprintf(buff, "%i", t.data.constant.constant)),
    buff : t.data.ident.lexeme);
}

inline static char* token_lex(token* t)
{
    if(t->data.ident.size <= 1) {
        print_token(*t);
        printf("%d\n", t->data.ident.size);
        CALLPANIC("Token dosen't have a defined lexeme");
    }

    return t->data.ident.lexeme;
}

#endif
