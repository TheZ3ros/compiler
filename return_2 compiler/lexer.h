#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_CONSTANT,
    TOKEN_INT_KEYWORD,
    TOKEN_VOID_KEYWORD,
    TOKEN_RETURN_KEYWORD,
    TOKEN_OPEN_PARENTHESIS,
    TOKEN_CLOSE_PARENTHESIS,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_SEMICOLON
} TokenType;

typedef struct {
    TokenType type;
    union {
        char *id_name;
        int int_val;
    } data;
} Token;

#endif