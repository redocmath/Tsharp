#ifndef AST_H
#define AST_H

#include <stdio.h>

typedef struct AST_STRUCT
{
    enum {
        AST_PRINT,
        AST_STRING,
        AST_COMPOUND,
        AST_NOOP,
    } type;

    // AST_PRINT
    struct AST_STRUCT* print_value;

    // AST_STRING
    char* string_value;

    // AST_COMPOUND
    struct AST_STRUCT** compound_value;
    size_t compound_size;

} AST_T;

AST_T* init_ast(int type);
#endif