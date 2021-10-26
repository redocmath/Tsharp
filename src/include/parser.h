#ifndef PARSER_H
#define PARSER_H
#include "AST.h"
#include "lex.h"
#include "scope.h"

typedef struct PARSER_STRUCT
{
    lexer_T* lexer;

    token_T* current_token;

    token_T* prev_token;

    scope_T* scope;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

void parser_eat(parser_T* parser, int token_type);

AST_T* parser_parse(parser_T* parser, scope_T* scope);

AST_T* parser_parse_statement(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_statements(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_expr(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_function_call(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_variable_definition(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_variable_update_value_from_other_func(parser_T* parser, scope_T* scope, char* func_name, char* variable_name);

AST_T* parser_parse_function_definition(parser_T* parser, scope_T* scope);

AST_T* parser_parse_variable(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_if(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_compare(parser_T* parser, scope_T* scope, AST_T* left, char* func_name);

AST_T* parser_parse_while(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_binop_inc_dec(parser_T* parser, scope_T* scope, char* vname, char* func_name);

AST_T* parser_parse_string(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_int(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_bool(parser_T* parser, scope_T* scope, char* func_name);

AST_T* parser_parse_id(parser_T* parser, scope_T* scope, char* func_name);

#endif