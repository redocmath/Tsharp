#ifndef AST_H
#define AST_H
#include <stdio.h>

typedef struct AST_STRUCT
{
    enum {
        AST_VARIABLE_DEFINITION,
        AST_FUNCTION_DEFINITION,
        AST_VARIABLE,
        AST_FUNCTION_CALL,
        AST_COMPARE,
        AST_STRING,
        AST_INT,
        AST_COMPOUND,
        AST_NOOP,
    } type;

    struct SCOPE_STRUCT* scope;

    // AST_VARIABLE_DEFINITION
    char* variable_definition_variable_name;
    struct AST_STRUCT* variable_definition_value;
    char* variable_definition_func_name;

    // AST_FUNCTION_DEFINITION
    struct AST_STRUCT* function_definition_body;
    char* function_definition_name;
    
    // AST_VARIABLE
    char* variable_name;
    char* variable_func_name;

    // AST_FUNCTION_CALL
    char* function_call_name;
    struct AST_STRUCT** args;
    size_t args_size;

    // AST_COMPARE
    struct AST_STRUCT* left;
    struct AST_STRUCT* right;
    int compare_op; 

    // AST_STRING
    char* string_value;

    // AST_INT
    long int int_value;

    // AST_COMPOUND
    struct AST_STRUCT** compound_value;
    size_t compound_size;

} AST_T;

AST_T* init_ast(int type);
#endif