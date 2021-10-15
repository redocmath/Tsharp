#include "include/parser.h"
#include "include/scope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    token_T* prev_token = parser->current_token;

    parser->scope = init_scope();

    return parser;
}

void parser_eat(parser_T* parser, int token_type)
{
    if (parser->current_token->type == token_type)
    {
        parser->prev_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        printf("\x1b[31m");
        printf(
            "SyntaxError: Unexpected token '%s' (line %d)\n",
            parser->current_token->value,
            parser->lexer->line_n
        );
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser, scope_T* scope)
{
    return parser_parse_statements(parser, scope);
}

AST_T* parser_parse_statement(parser_T* parser, scope_T* scope)
{
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser, scope);
        default: return 0;
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_statement_func_body(parser_T* parser, scope_T* scope)
{
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id_func_body(parser, scope);
        default: return 0;
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_statements(parser_T* parser, scope_T* scope)
{
    AST_T* compound = init_ast(AST_COMPOUND);

    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* ast_statement = parser_parse_statement(parser, scope);

    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;

    while (parser->current_token->type == TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_SEMI);
        
        AST_T* ast_statement = parser_parse_statement(parser, scope);

        if (ast_statement)
        {
            compound->compound_size += 1;

            compound->compound_value = realloc(
                compound->compound_value,
                compound->compound_size * sizeof(struct AST_STRUCT)
            );
            compound->compound_value[compound->compound_size-1] = ast_statement;
        }
    }

    compound->scope = scope;

    return compound;
}

AST_T* parser_parse_statements_func_body(parser_T* parser, scope_T* scope)
{
    AST_T* compound = init_ast(AST_COMPOUND);

    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* ast_statement = parser_parse_statement_func_body(parser, scope);

    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;

    while (parser->current_token->type == TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_SEMI);
        
        AST_T* ast_statement = parser_parse_statement_func_body(parser, scope);

        if (ast_statement)
        {
            compound->compound_size += 1;

            compound->compound_value = realloc(
                compound->compound_value,
                compound->compound_size * sizeof(struct AST_STRUCT)
            );
            compound->compound_value[compound->compound_size-1] = ast_statement;
        }
    }

    compound->scope = scope;

    return compound;
}

AST_T* parser_parse_expr(parser_T* parser, scope_T* scope)
{
    switch (parser->current_token->type)
    {
        case TOKEN_STRING: return parser_parse_string(parser, scope);
        case TOKEN_ID: return parser_parse_id(parser, scope);
        default: return 0;
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_variable_definition(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_VARIABLE_DEFINITION);

    char * variable_name = parser->prev_token->value;

    ast->variable_definition_variable_name = calloc(
        strlen(variable_name) + 1,
        sizeof(char)
    );

    strcpy(ast->variable_definition_variable_name, variable_name);

    parser_eat(parser, TOKEN_EQUAL);

    AST_T* variable_definition_value = parser_parse_expr(parser, scope);
    ast->variable_definition_value = variable_definition_value;

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_function_definition(parser_T* parser, scope_T* scope)
{
    AST_T* ast = init_ast(AST_FUNCTION_DEFINITION);

    parser_eat(parser, TOKEN_ID);

    char* function_name = parser->current_token->value;
    ast->function_definition_name = calloc(
        strlen(function_name) + 1,
        sizeof(char)
    );
    strcpy(ast->function_definition_name, function_name);

    parser_eat(parser, TOKEN_ID);

    parser_eat(parser, TOKEN_LPAREN);

    parser_eat(parser, TOKEN_RPAREN);
    parser_eat(parser, TOKEN_LBRACE);

    ast->function_definition_body = parser_parse_statements_func_body(parser, scope);

    parser_eat(parser, TOKEN_RBRACE);

    ast->scope = scope;

    return ast;
}


AST_T* parser_parse_variable(parser_T* parser, scope_T* scope)
{
    char* token_value = parser->current_token->value;

    parser_eat(parser, TOKEN_ID);

    if (parser->current_token->type == TOKEN_EQUAL) {
        return parser_parse_variable_definition(parser, scope);
    }

    if (parser->current_token->type == TOKEN_LPAREN) {
        return parser_parse_function_call(parser, scope);
    }

    AST_T* ast = init_ast(AST_VARIABLE);
    ast->variable_name = token_value;

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_variable_outside_func(parser_T* parser, scope_T* scope)
{
    char* token_value = parser->current_token->value;

    parser_eat(parser, TOKEN_ID);

    if (parser->current_token->type == TOKEN_EQUAL) {
        return parser_parse_variable_definition(parser, scope);
    }

    if (parser->current_token->type == TOKEN_LPAREN) {
        printf("\x1b[31m");
        printf("Syntax Error: non-declaration statement outside function body\n");
        exit(1);
    }

    AST_T* ast = init_ast(AST_VARIABLE);
    ast->variable_name = token_value;

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_string(parser_T* parser, scope_T* scope)
{
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);

    ast_string->scope = scope;

    return ast_string;
}

AST_T* parser_parse_function_call(parser_T* parser, scope_T* scope){
    AST_T* ast = init_ast(AST_FUNCTION_CALL);

    ast->function_call_name = parser->prev_token->value;

    parser_eat(parser, TOKEN_LPAREN);

    ast->function_call_arguments = calloc(1, sizeof(struct AST_STRUCT*));

    if (parser->current_token->type == TOKEN_ID) {
        ast->function_call_args_name = parser->current_token->value;
    }

    AST_T* ast_expr = parser_parse_expr(parser, scope);
    ast->function_call_arguments[0] = ast_expr;
    ast->function_call_arguments_size += 1;

    parser_eat(parser, TOKEN_RPAREN);

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_id(parser_T* parser, scope_T* scope)
{
    if (strcmp(parser->current_token->value, "func") == 0)
    {
        return parser_parse_function_definition(parser, scope);
    }
    if (strcmp(parser->current_token->value, "print") == 0)
    {
        printf("\x1b[31m");
        printf("Syntax Error: non-declaration statement outside function body\n");
        exit(1);
    }
    return parser_parse_variable_outside_func(parser, scope);
}

AST_T* parser_parse_id_func_body(parser_T* parser, scope_T* scope)
{
    if (strcmp(parser->current_token->value, "func") == 0)
    {
        return parser_parse_function_definition(parser, scope);
    }
    return parser_parse_variable(parser, scope);
}