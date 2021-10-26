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
        AST_IF,
        AST_COMPARE,
        AST_WHILE,
        AST_STRING,
        AST_INT,
        AST_BOOL,
        AST_BINOP_INC_DEC,
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
    struct AST_STRUCT** function_definition_args;
    size_t function_definition_args_size;
    
    // AST_VARIABLE
    char* variable_name;
    char* variable_func_name;

    // AST_FUNCTION_CALL
    char* function_call_name;
    struct AST_STRUCT** args;
    size_t args_size;

    // AST_IF
    struct AST_STRUCT* op;
    struct AST_STRUCT* if_body;
    struct AST_STRUCT* else_body;

    // AST_COMPARE
    struct AST_STRUCT* left;
    struct AST_STRUCT* right;
    int compare_op;

    // AST_WHILE
    struct AST_STRUCT* while_body;

    // AST_BINOP_INC_DEC
    char* binop_inc_dec_variable;
    int binop_inc_dec_op;
    char* binop_inc_dec_func_name;

    // AST_STRING
    char* string_value;

    // AST_INT
    long int int_value;

    // AST_BOOL
    char* bool_value;

    // AST_COMPOUND
    struct AST_STRUCT** compound_value;
    size_t compound_size;

} AST_T;

AST_T* init_ast(int type);
#endif
