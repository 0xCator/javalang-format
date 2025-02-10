#include "lexer.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Java lexer implementation */

Lexer lexer;

TokenStream *token_stream_new(){
    TokenStream *ts = (TokenStream *)malloc(sizeof(TokenStream));
    ts->size = 0;
    ts->capacity = INITIAL_CAPACITY;
    ts->tokens = (Token *)malloc(sizeof(Token)*ts->capacity);
    return ts;
}

void free_token_stream(TokenStream *ts){
    free(ts->tokens);
    free(ts);
}

static void add_token(TokenStream *ts,Token token){

    if(ts->size == ts->capacity){
        ts->capacity *= 2;
        ts->tokens = (Token *)realloc(ts->tokens, sizeof(Token)*ts->capacity);
    }
    ts->tokens[ts->size++] = token;
}

void init_lexer(char *input){
    lexer.start = input;
    lexer.current = input;
    lexer.line = 1;
}

static char advance(){
    lexer.current++;
    return lexer.current[-1];
}

static char peek(){
    return *lexer.current;
}

static void skip_whitespace(){
    for(;;){
        char c = peek();
        switch(c){
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                lexer.line++;
                advance();
                break;
            case '/':
                if(peek() == '/'){
                    while(peek() != '\n' && peek() != '\0') advance();
                }else{
                    return;
                }
                break;
            case '\0':
                return;
            default:
                return;
        }
    }

}

static bool is_at_end(){
    return *lexer.current == '\0';
}

static bool is_digit(char c){
    return c >= '0' && c <= '9';
}

static bool is_alpha(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_alphanumeric(char c){
    return is_alpha(c) || is_digit(c);
}

static bool match(char expected){
    if(is_at_end()) return false;
    if(*lexer.current != expected) return false;

    lexer.current++;
    return true;
}

static char peek_next(){
    if(is_at_end()) return '\0';
    return lexer.current[1];
}

static Token token_new(TokenType type) {
    int length = lexer.current - lexer.start;
    char *lexeme = (char *)malloc(length + 1);
    memcpy(lexeme, lexer.start, length);
    lexeme[length] = '\0';

    return (Token){
        .type = type,
        .line = lexer.line,
        .column = length,
        .length = length,
        .lexeme = lexeme
    };
}

static Token string(){
    while(peek() != '"' && !is_at_end()){
        if(peek() == '\n') lexer.line++;
        advance();
    }

    if(is_at_end()){
        printf("Unterminated string\n");
        exit(1);
    }

    advance();
    return token_new(STRING_LITERAL);
}

static Token number(){
    while(is_digit(peek())) advance();

    if(peek() == '.' && is_digit(peek_next())){
        advance();
        while(is_digit(peek())) advance();
    }

    return token_new(NUMBER_LITERAL);
}

static TokenType check_keyword(int start, int length, char *rest, TokenType type){
    if(lexer.current - lexer.start == start + length && memcmp(lexer.start + start, rest, length) == 0){
        return type;
    }

    return IDENTIFIER;
}

// TODO: Implement the rest of the keywords
static TokenType identifer_type(){
    switch(lexer.start[0]){
        case 'a': return check_keyword(1, 2, "nd", AND);
        case 'c': return check_keyword(1, 4, "lass", CLASS);
        case 'e': return check_keyword(1, 3, "lse", ELSE);
        case 'f':
            if(lexer.current - lexer.start > 1){
                switch(lexer.start[1]){
                    case 'a': return check_keyword(2, 3, "lse", FALSE);
                    case 'o': return check_keyword(2, 1, "r", FOR);
                }
            }
            break;
        case 'i': 
            if(lexer.current - lexer.start > 1){
                switch(lexer.start[1]){
                    case 'f': return check_keyword(2, 0, "", IF);
                    case 'm': return check_keyword(2, 4, "port", IMPORT);
                    case 'n': return check_keyword(2, 1, "t", INT);
                }
            }
            break;
        case 'n': return check_keyword(1, 2, "il", NULL_KEYWORD);
        case 'o': return check_keyword(1, 1, "r", OR);
        case 'r': return check_keyword(1, 5, "eturn", RETURN);
        case 's': return check_keyword(1, 4, "uper", SUPER);
        case 't':
            if(lexer.current - lexer.start > 1){
                switch(lexer.start[1]){
                    case 'h': return check_keyword(2, 2, "is", THIS);
                    case 'r': return check_keyword(2, 2, "ue", TRUE);
                }
            }
            break;
        case 'v': return check_keyword(1, 2, "ar", VAR);
        case 'w': return check_keyword(1, 4, "hile", WHILE);
        case 'p': 
            if(lexer.current - lexer.start > 1){
                switch(lexer.start[1]){
                    case 'u': return check_keyword(2, 4, "blic", PUBLIC);
                    case 'r': 
                        if(lexer.current - lexer.start > 2){
                            switch(lexer.start[2]){
                                case 'i': return check_keyword(3, 1, "v", PRIVATE);
                                case 'o': return check_keyword(3, 1, "t", PROTECTED);
                            }
                        }
                        break;
                }
            }
            break;
    }

    return IDENTIFIER;
}

static Token identifier(){
    while(is_alphanumeric(peek())) advance();

    return token_new(identifer_type());
}


Token scan_token(){
    skip_whitespace();
    lexer.start = lexer.current;

    if(is_at_end()) return token_new(END_OF_FILE);

    char c = advance();

    if(is_alpha(c)) return identifier();
    if(is_digit(c)) return number();

    switch(c){
        case '(': return token_new(LEFT_PAREN);
        case ')': return token_new(RIGHT_PAREN);
        case '{': return token_new(LEFT_BRACE);
        case '}': return token_new(RIGHT_BRACE);
        case '[': return token_new(LEFT_BRACKET);
        case ']': return token_new(RIGHT_BRACKET);
        case ',': return token_new(COMMA);
        case '.': return token_new(DOT);
        case '-': return token_new(MINUS);
        case ';': return token_new(SEMICOLON);
        case '+': return token_new(PLUS);
        case '*': return token_new(STAR);
        case '!': return token_new(match('=') ? BANG_EQUAL : BANG);
        case '=': return token_new(match('=') ? EQUAL_EQUAL : EQUAL);
        case '<': return token_new(match('=') ? LESS_EQUAL : LESS);
        case '>': return token_new(match('=') ? GREATER_EQUAL : GREATER);
        case '"': return string();
    }

    return token_new(ERROR);
}

void scan_tokens(TokenStream *ts){
    Token token;
    while((token = scan_token()).type != END_OF_FILE){
        add_token(ts, token);
    }
    add_token(ts, token);
}
