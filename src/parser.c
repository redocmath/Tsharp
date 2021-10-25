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
        int prevTokenSpace = 0;
        int expectedTokenSpace = 0;
        int num = strlen(parser->prev_token->value) - 10;

        printf(
            "SyntaxError: Unexpected token '%s' (line %d)\n",
            parser->current_token->value,
            parser->lexer->line_n
        );

        if (num > 0)
        {
            expectedTokenSpace = num;
            printf("%s\n", parser->prev_token->value);
            for (int i = 0; i < expectedTokenSpace; i++)
            {
                printf(" ");
            }
            printf(" ");
            printf("         ^\n");
            for (int i = 0; i < expectedTokenSpace; i++)
            {
                printf(" ");
            }
            printf(" ");
        }
        else
        if
        (num < 0)
        {
            prevTokenSpace = num * -1;
            for (int i = 1; i < prevTokenSpace; i++)
            {
                printf(" ");
            }
            printf("%s\n", parser->prev_token->value);
            printf("         ^\n");
        }
        else
        {
            printf("%s\n", parser->prev_token->value);
            printf("         ↑\n");
        }
        switch (token_type)
        {
            case 3: printf("Expected (\n"); break;
            case 4: printf("Expected )\n"); break;
            case 5: printf("Expected {\n"); break;
            case 6: printf("Expected }\n"); break;
            case 7: printf("Expected =\n"); break;
            case 8: printf("Expected ,\n"); break;
        }
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser, scope_T* scope)
{
    return parser_parse_statements(parser, scope, "");
}

AST_T* parser_parse_statement(parser_T* parser, scope_T* scope, char* func_name)
{
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser, scope, func_name);
        default: return 0;
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_statements(parser_T* parser, scope_T* scope, char* func_name)
{
    AST_T* compound = init_ast(AST_COMPOUND);

    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* ast_statement = parser_parse_statement(parser, scope, func_name);

    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;
    //while ((parser->prev_token->type != TOKEN_COLON && parser->current_token->type == TOKEN_SEMI) || (parser->current_token->type == TOKEN_END && parser->prev_token->type == TOKEN_COLON))
    while(parser->current_token->type == TOKEN_SEMI)
    {
        if (parser->current_token->type == TOKEN_SEMI)
            parser_eat(parser, TOKEN_SEMI);
        
        AST_T* ast_statement = parser_parse_statement(parser, scope, func_name);

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

AST_T* parser_parse_expr(parser_T* parser, scope_T* scope, char* func_name)
{
    switch (parser->current_token->type)
    {
        case TOKEN_STRING: return parser_parse_string(parser, scope, func_name);
        case TOKEN_INT: return parser_parse_int(parser, scope, func_name);
        case TOKEN_BOOL: return parser_parse_bool(parser, scope, func_name);
        case TOKEN_ID: return parser_parse_id(parser, scope, func_name);
        default: return 0;
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_function_call(parser_T* parser, scope_T* scope, char* func_name){
    AST_T* ast = init_ast(AST_FUNCTION_CALL);

    ast->function_call_name = parser->prev_token->value;

    parser_eat(parser, TOKEN_LPAREN);

    ast->args = calloc(1, sizeof(struct AST_STRUCT*));
    
    if (parser->current_token->type != TOKEN_RPAREN)
    {
        AST_T* ast_expr = parser_parse_expr(parser, scope, func_name);
        ast->args[0] = ast_expr;
        ast->args_size += 1;
    }

    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);
        
        AST_T* ast_expr = parser_parse_expr(parser, scope, func_name);
        
        ast->args_size += 1;

        ast->args = realloc(
            ast->args,
            ast->args_size * sizeof(struct AST_STRUCT)
        );
        ast->args[ast->args_size-1] = ast_expr;
    }

    parser_eat(parser, TOKEN_RPAREN);

    if (parser->current_token->type == TOKEN_COLON)
    {
        printf(
            "SyntaxError: Unexpected token '%s' (line %d)\n",
            parser->current_token->value,
            parser->lexer->line_n
        );
        exit(1);
    }

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_variable_definition(parser_T* parser, scope_T* scope, char* func_name)
{
    AST_T* ast = init_ast(AST_VARIABLE_DEFINITION);

    char * variable_name = parser->prev_token->value;

    ast->variable_definition_variable_name = calloc(
        strlen(variable_name) + 1,
        sizeof(char)
    );

    strcpy(ast->variable_definition_variable_name, variable_name);

    ast->variable_definition_func_name = calloc(
        strlen(func_name) + 1,
        sizeof(char)
    );

    strcpy(ast->variable_definition_func_name, func_name);

    parser_eat(parser, TOKEN_EQUAL);

    AST_T* variable_definition_value = parser_parse_expr(parser, scope, func_name);
    ast->variable_definition_value = variable_definition_value;

    if (parser->current_token->type == TOKEN_COLON)
    {
        printf(
            "SyntaxError: Unexpected token '%s' (line %d)\n",
            parser->current_token->value,
            parser->lexer->line_n
        );
        exit(1);
    }

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_variable_update_value_from_other_func(parser_T* parser, scope_T* scope, char* func_name, char* variable_name)
{
    AST_T* ast = init_ast(AST_VARIABLE_DEFINITION);

    ast->variable_definition_variable_name = calloc(
        strlen(variable_name) + 1,
        sizeof(char)
    );

    strcpy(ast->variable_definition_variable_name, variable_name);

    ast->variable_definition_func_name = calloc(
        strlen(func_name) + 1,
        sizeof(char)
    );

    strcpy(ast->variable_definition_func_name, func_name);

    parser_eat(parser, TOKEN_EQUAL);

    AST_T* variable_definition_value = parser_parse_expr(parser, scope, func_name);
    ast->variable_definition_value = variable_definition_value;

    if (parser->current_token->type == TOKEN_COLON)
    {
        printf(
            "SyntaxError: Unexpected token '%s' (line %d)\n",
            parser->current_token->value,
            parser->lexer->line_n
        );
        exit(1);
    }

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

    if (parser->current_token->type != TOKEN_RPAREN)
    {
        ast->function_definition_args = calloc(1, sizeof(struct AST_STRUCT*));

        AST_T* arg = parser_parse_variable(parser, scope, function_name);
        ast->function_definition_args_size += 1;
        ast->function_definition_args[ast->function_definition_args_size-1] = arg;

        while (parser->current_token->type == TOKEN_COMMA)
        {
            parser_eat(parser, TOKEN_COMMA);

            ast->function_definition_args_size += 1;

            ast->function_definition_args =
                realloc(
                    ast->function_definition_args,
                    ast->function_definition_args_size * sizeof(struct AST_STRUCT*)
                );

            AST_T* arg = parser_parse_variable(parser, scope, function_name);
            ast->function_definition_args[ast->function_definition_args_size-1] = arg;
        }
    }

    parser_eat(parser, TOKEN_RPAREN);
    parser_eat(parser, TOKEN_DO);

    if (parser->current_token->type == TOKEN_END)
    {
        printf("SyntaxError: function body is empty (line %d)\n", parser->lexer->line_n);
        exit(1);
    }

    ast->function_definition_body = parser_parse_statements(parser, scope, function_name);

    parser_eat(parser, TOKEN_END);

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_variable(parser_T* parser, scope_T* scope, char* func_name)
{
    char* token_value = parser->current_token->value;

    parser_eat(parser, TOKEN_ID);

    if (parser->current_token->type == TOKEN_EQUAL) {
        if (strcmp(func_name, "\0") == 0)
        {
            printf("Syntax Error: non-declaration statement outside function body\n");
            exit(1);
        }
        return parser_parse_variable_definition(parser, scope, func_name);
    }

    if (parser->current_token->type == TOKEN_LPAREN) {
        if (strcmp(func_name, "\0") == 0)
        {
            printf("Syntax Error: non-declaration statement outside function body\n");
            exit(1);
        }
        return parser_parse_function_call(parser, scope, func_name);
    }

    AST_T* ast = init_ast(AST_VARIABLE);
    
    char* variable_name;
    if (parser->current_token->type == TOKEN_DOT)
    {
        ast->variable_func_name = token_value;
        parser_eat(parser, TOKEN_DOT);

        variable_name = parser->current_token->value;

        parser_eat(parser, TOKEN_ID);

        ast->variable_name = variable_name;

        ast->scope = scope;
    }
    else
    {
        ast->variable_func_name = func_name;

        ast->variable_name = token_value;

        ast->scope = scope;
    }

    if (parser->current_token->type == TOKEN_EQUAL)
    {
        return parser_parse_variable_update_value_from_other_func(parser, scope, token_value, variable_name);
    }

    if (parser->current_token->type == TOKEN_EQUALS || parser->current_token->type == TOKEN_NOT_EQUALS || parser->current_token->type == TOKEN_GREATER_THAN || parser->current_token->type == TOKEN_LESS_THAN)
    {
        return parser_parse_compare(parser, scope, ast, func_name);
    }

    return ast;
}

AST_T* parser_parse_if(parser_T* parser, scope_T* scope, char* func_name)
{
    AST_T* ast = init_ast(AST_IF);

    parser_eat(parser, TOKEN_ID);

    AST_T* op = parser_parse_expr(parser, scope, func_name);
    ast->op = op;
    parser_eat(parser, TOKEN_DO);

    if (parser->current_token->type == TOKEN_END)
    {
        printf(
            "SyntaxError: if body is empty (line %d)\n",
            parser->lexer->line_n
        );
        exit(1);
    }

    ast->if_body = parser_parse_statements(parser, scope, func_name);

    if (parser->current_token->type == TOKEN_ELSE)
    {
        parser_eat(parser, TOKEN_ELSE);
        ast->else_body = parser_parse_statements(parser, scope, func_name);
    }
    parser_eat(parser, TOKEN_END);

    ast->scope = scope;
    return ast;
}

AST_T* parser_parse_compare(parser_T* parser, scope_T* scope, AST_T* left, char* func_name)
{
    AST_T* ast = init_ast(AST_COMPARE);

    ast->left = left;

    ast->compare_op = parser->current_token->type;

    parser_eat(parser, parser->current_token->type);

    AST_T* right = parser_parse_expr(parser, scope, func_name);

    ast->right = right;

    ast->scope = scope;
    return ast;
}

AST_T* parser_parse_while(parser_T* parser, scope_T* scope, char* func_name)
{
    AST_T* ast = init_ast(AST_WHILE);
    parser_eat(parser, TOKEN_ID);
    ast->op = parser_parse_expr(parser, scope, func_name);
    parser_eat(parser, TOKEN_DO);
    if (parser->current_token->type == TOKEN_END)
    {
        printf("SyntaxError: while body is empty (line %d)\n", parser->lexer->line_n);
        exit(1);
    }
    ast->while_body = parser_parse_statements(parser, scope, func_name);
    parser_eat(parser, TOKEN_END);
    ast->scope = scope;
    return ast;
}

AST_T* parser_parse_string(parser_T* parser, scope_T* scope, char* func_name)
{
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);

    ast_string->scope = scope;

    if (parser->current_token->type == TOKEN_EQUALS || parser->current_token->type == TOKEN_NOT_EQUALS || parser->current_token->type == TOKEN_GREATER_THAN || parser->current_token->type == TOKEN_LESS_THAN)
    {
        return parser_parse_compare(parser, scope, ast_string, func_name);
    }

    return ast_string;
}

AST_T* parser_parse_int(parser_T* parser, scope_T* scope, char* func_name)
{
    char* endPtr;
    long int int_value = strtol(parser->current_token->value, &endPtr, 10);
    
    parser_eat(parser, TOKEN_INT);
    AST_T* ast_int = init_ast(AST_INT);
    ast_int->int_value = int_value;

    ast_int->scope = scope;

    if (parser->current_token->type == TOKEN_EQUALS || parser->current_token->type == TOKEN_NOT_EQUALS || parser->current_token->type == TOKEN_GREATER_THAN || parser->current_token->type == TOKEN_LESS_THAN)
    {
        return parser_parse_compare(parser, scope, ast_int, func_name);
    }

    return ast_int;
}

AST_T* parser_parse_bool(parser_T* parser, scope_T* scope, char* func_name)
{
    AST_T* ast_bool = init_ast(AST_BOOL);
    ast_bool->bool_value = parser->current_token->value;

    parser_eat(parser, TOKEN_BOOL);

    ast_bool->scope = scope;

    return ast_bool;
}

AST_T* parser_parse_id(parser_T* parser, scope_T* scope, char* func_name)
{
    if (strcmp(parser->current_token->value, "func") == 0)
    {
        return parser_parse_function_definition(parser, scope);
    }

    if (strcmp(parser->current_token->value, "if") == 0)
    {
        return parser_parse_if(parser, scope, func_name);
    }

    if (strcmp(parser->current_token->value, "while") == 0)
    {
        return parser_parse_while(parser, scope, func_name);
    }

    return parser_parse_variable(parser, scope, func_name);
}