#ifndef LEXER_H
#define LEXER_H

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 10

//Java Token Enum
typedef enum {
    //Keywords
    CLASS, PUBLIC, STATIC, VOID, MAIN, STRING, EXTENDS, RETURN, INT, BOOLEAN, IF, ELSE, WHILE, SYSTEM, OUT, PRINTLN, LENGTH, THIS, NEW, TRUE, FALSE, PRIVATE, PROTECTED,
    FOR, BREAK, CONTINUE, SUPER, NULL_KEYWORD, VAR, FLOAT, DOUBLE, CHAR, SHORT, LONG, BYTE, ENUM, INTERFACE, IMPLEMENTS, PACKAGE, IMPORT, INSTANCEOF,
    //Operators
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULUS, AND, OR, NOT, EQUAL, NOT_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN, MULTIPLY_ASSIGN, DIVIDE_ASSIGN, MODULUS_ASSIGN,
    BANG, BANG_EQUAL, EQUAL_EQUAL,
    //Punctuation
    SEMICOLON, COMMA, DOT, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET, STAR,
    //Identifiers
    IDENTIFIER,
    //Literals
    NUMBER_LITERAL, STRING_LITERAL,
    // ERROR
    ERROR,
    //End of File
    END_OF_FILE
} TokenType;

/* TokenStream struct
 * Contains the token and the lexeme and metadata
 */

typedef struct Token{
    TokenType type;
    const char *lexeme;
    int line;
    int column;
    int length;
} Token;

typedef struct TokenStream{
    Token *tokens;
    int size;
    int capacity;
    int index;
} TokenStream;

typedef struct Lexer{
    const char *start;
    const char *current;
    int line;
} Lexer;

void init_lexer(char *input);
void free_token_stream(TokenStream *ts);
void scan_tokens(TokenStream *ts);
TokenStream *token_stream_new();

#endif // LEXER_H
