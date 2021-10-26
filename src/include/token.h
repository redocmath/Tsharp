#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
    enum
    {
        /* 00 */TOKEN_STRING, // "abc"
        /* 01 */TOKEN_ID, // abc
        /* 02 */TOKEN_SEMI, // ;
        /* 03 */TOKEN_LPAREN, // (
        /* 04 */TOKEN_RPAREN, // )
        /* 05 */TOKEN_LBRACE, // {
        /* 06 */TOKEN_RBRACE, // }
        /* 07 */TOKEN_EQUAL, // =
        /* 08 */TOKEN_COMMA, // ,
        /* 09 */TOKEN_DOT, // .
        /* 10 */TOKEN_GREATER_THAN,
        /* 11 */TOKEN_LESS_THAN,
        /* 12 */TOKEN_EQUALS,
        /* 13 */TOKEN_NOT_EQUALS,
        /* 14 */TOKEN_END, // } -> end
        /* 15 */TOKEN_DO, // { -> do
        /* 16 */TOKEN_ELSE, // else
        /* 17 */TOKEN_INT, // 123
        /* 18 */TOKEN_BOOL, // true, false
        /* 19 */TOKEN_PLUS_PLUS, // ++
        /* 20 */TOKEN_MINUS_MINUS, // --
        /* 21 */TOKEN_EOF // End Of File
    } type;

    char* value;
} token_T;

token_T* init_token(int type, char* value);

#endif