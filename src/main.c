#include <stdio.h>
#include "include/lex.h"

int main(int argc, char* argv[]) {

    lexer_T* lexer = init_lexer(
        "print :: \"Hello World!\";\n"
    );

    token_T* token = (void*) 0;

    while ((token = lexer_get_next_token(lexer)) != (void*) 0)
    {
        printf("token %d, %s\n", token->type, token->value);
    }

    return 0;
}