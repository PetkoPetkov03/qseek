
#include "cast.h"
#include "parser/token.h"
#include "queue.h"
#include <parser/tokenizer/tokenizer.h>
#include <stdlib.h>

tokenizer_t* init_tokenizer()
{
    tokenizer_t* tokenizer = castp(malloc, tokenizer_t, sizeof(tokenizer_t));

    tokenizer->tokens = init_queue();

    return tokenizer;
}

void tokenize(tokenizer_t *tokenizer) 
{
    
}

void print_tokens(tokenizer_t *tokenizer)
{
    while(!queue_is_empty(tokenizer->tokens)) {
        token t = cast_safe(dequeue, token, tokenizer->tokens);

        print_token(t);
    }
}

void tokenizer_clean(tokenizer_t *tokenizer)
{
    queue_clear(tokenizer->tokens);
    free(tokenizer);
}