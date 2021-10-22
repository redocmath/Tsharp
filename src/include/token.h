#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
    enum
    {
        TOKEN_STRING, // "abc"
        TOKEN_ID, // abc
        TOKEN_SEMI, // ;
        TOKEN_LPAREN, // (
        TOKEN_RPAREN, // )
        TOKEN_LBRACE, // {
        TOKEN_RBRACE, // }
        TOKEN_EQUAL, // =
        TOKEN_COMMA, // ,
        TOKEN_DOT, // .
        TOKEN_INT, // 123
        TOKEN_EOF // End Of File
    } type;

    char* value;
} token_T;

token_T* init_token(int type, char* value);

#endif