#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    token_T* prev_token = parser->current_token;

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

AST_T* parser_parse(parser_T* parser)
{
    return parser_parse_statements(parser);
}

AST_T* parser_parse_statement(parser_T* parser)
{
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser);
        default: return 0;
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_statements(parser_T* parser)
{
    AST_T* compound = init_ast(AST_COMPOUND);

    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* ast_statement = parser_parse_statement(parser);

    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;

    while (parser->current_token->type == TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_SEMI);
        
        AST_T* ast_statement = parser_parse_statement(parser);

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

    return compound;
}

AST_T* parser_parse_expr(parser_T* parser)
{
    switch (parser->current_token->type)
    {
        case TOKEN_STRING: return parser_parse_string(parser);
        default: return 0;
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_string(parser_T* parser)
{
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);

    return ast_string;
}

AST_T* parser_parse_print(parser_T* parser)
{
    AST_T* ast = init_ast(AST_PRINT);

    parser_eat(parser, TOKEN_ID);

    parser_eat(parser, TOKEN_TWO_COLON);

    AST_T* print_value = parser_parse_expr(parser);

    ast->print_value = print_value;

    return ast;
}

AST_T* parser_parse_id(parser_T* parser)
{
    if (strcmp(parser->current_token->value, "print") == 0)
    {
        return parser_parse_print(parser);
    }

    return init_ast(AST_NOOP);
}