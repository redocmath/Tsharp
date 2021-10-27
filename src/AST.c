#include "include/AST.h"
#include <stdlib.h>

AST_T* init_ast(int type)
{
    AST_T* ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;

    ast->scope = (void*) 0;

    // AST_VARIABLE_DEFINITION
    ast->variable_definition_variable_name = (void*) 0;
    ast->variable_definition_value = (void*) 0;
    ast->variable_definition_func_name = (void*) 0;

    // AST_FUNCTION_DEFINITION
    ast->function_definition_body = (void*) 0;
    ast->function_definition_name = (void*) 0;
    ast->function_definition_args = (void*) 0;
    ast->function_definition_args_size = 0;

    // AST_VARIABLE
    ast->variable_name = (void*) 0;
    ast->variable_func_name = (void*) 0;

    // AST_FUNCTION_CALL
    ast->function_call_name = (void*) 0;
    ast->args = (void*) 0;
    ast->args_size = 0;

    // AST_IF
    ast->op = (void*) 0;
    ast->if_body = (void*) 0;
    ast->else_body = (void*) 0;
    ast->elifop = (void*) 0;
    ast->elifbody = (void*) 0;
    ast->elif_size = 0;

    // AST_COMPARE
    ast->left = (void*) 0;
    ast->right = (void*) 0;
    ast->compare_op = 0;

    // AST_WHILE
    ast->while_body = (void*) 0;

    // AST_BINOP_INC_DEC
    ast->binop_inc_dec_variable = (void*) 0;
    ast->binop_inc_dec_op = 0;
    ast->binop_inc_dec_func_name = (void*) 0;

    // AST_STRING
    ast->string_value = (void*) 0;

    // AST_INT
    ast->int_value = 0;

    // AST_BOOL
    ast->bool_value = (void*) 0;

    // AST_COMPOUND
    ast->compound_value = (void*) 0;
    ast->compound_size = 0;

    return ast;
}
