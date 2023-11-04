#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bgl_error(char* message) {
    printf("%s", message);
    exit(1);
}

void _strip_newlines(char** buffer) {
    (*buffer)[strcspn(*buffer, "\n\r")] = 0;
}

char* bgl_read() {
    int size = 1024;
    char* line = malloc(sizeof(char) * size);
    printf("> ");

    fgets(line, size, stdin);
    _strip_newlines(&line);

    return line;
}

char* bgl_eval(char* text) {
    return text;
}

void bgl_print(char* text) {
    printf("%s\n", text);
    free(text);
}

int main() {
    for (;;) {
        bgl_print(bgl_eval(bgl_read()));
    }
}