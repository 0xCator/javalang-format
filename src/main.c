#include <stdio.h>
#include "lexer.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File not found\n");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = (char *)malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    init_lexer(buffer);
    TokenStream *ts = token_stream_new();
    scan_tokens(ts);
    for (int i = 0; i < ts->size; i++) {
        Token t = ts->tokens[i];
        printf("Token: %d, Line: %d, Column: %d, Lexeme: %s\n", t.type, t.line, t.column, t.lexeme);
    }
    free(buffer);
    free_token_stream(ts);

    fclose(file);
    return 0;

}
