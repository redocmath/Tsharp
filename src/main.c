#include <stdio.h>
#include "include/lex.h"
#include "include/parser.h"

int main(int argc, char* argv[]) {

    lexer_T* lexer = init_lexer(
        "print :: \"Hello World!\";\n"
    );

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser);

    printf("%d\n", root->type);
    printf("%zu\n", root->compound_size);

    return 0;
}