#include <stdio.h>
#include <stdlib.h>
#include "include/lex.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"
#include "include/scope.h"

int main(int argc, char* argv[]) {

    lexer_T* lexer = init_lexer(
        get_file_contents(argv[1])
    );

    parser_T* parser = init_parser(lexer);

    AST_T* root = parser_parse(parser, parser->scope);

    visitor_T* visitor = init_visitor();

    visitor_visit(visitor, root);

    char* name = "main";
    AST_T* fdef = scope_find_func(parser->scope, name);

    system("gcc -o main comp.c");

    return 0;
}