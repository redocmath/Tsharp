#include <stdio.h>
#include <stdlib.h>
#include "include/lex.h"
#include "include/parser.h"
#include "include/asm.h"
#include "include/io.h"
#include "include/scope.h"
#include <string.h>

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

    system("/usr/local/Cellar/nasm/2.15.05/bin/nasm -f macho64 main.asm");

    system("ld main.o -o main -no_pie -macosx_version_min 11.4 -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem");

    return 0;
}