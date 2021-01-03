#include <stdio.h>
#include <stdlib.h>

#include "drivers.h"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

void print_tape(char *tape, int tape_length) {
    for (int i = 0; i < tape_length; i++) fprintf(stderr, "%d\t", tape[i]);
    fprintf(stderr, "\n");
}

void zero(char *buffer, int offset, int length) {
    for (int i = 0; i < length; i++) {
        buffer[i + offset] = 0;
    }
}

void next_bracket(char *code, int *pos, int direction) {
    int count = 1;

    (*pos) += direction;

    while (count) {
        switch (code[*pos]) {
            case '[':
                if (direction == 1)
                    count++;
                else if (direction == -1)
                    count--;
                break;
            case ']':
                if (direction == 1)
                    count--;
                else if (direction == -1)
                    count++;
                break;
            default:
                break;
        }
        (*pos) += direction;
    }

    if (direction == 1) {
        (*pos) -= 2;
    }
}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        printf("Missing Filename and Drivers\nUse: compiler.exe (bf code) [drivers]");
        return -1;
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Error loading bf code\n");
        return -1;
    }

    char *code;
    int length;
    int i;

    for (length = 0; fgetc(f) != EOF; length++)
        ;
    code = malloc(length + 1);
    fseek(f, 0, SEEK_SET);

    for (i = 0; i < length; i++) {
        code[i] = fgetc(f);
    }
    code[i] = '\0';

    int tape_length = 16;
    char *tape = malloc(tape_length);
    zero(tape, 0, tape_length);
    int pos = 0;

    Drivers *drivers = malloc(sizeof(Drivers));
    drivers->length = 2;
    drivers->list = malloc(sizeof(Driver *) * drivers->length);
    drivers->offset = 0;
    drivers->pos = 0;

    if (argc == 2) {
        printf("0 drivers to load");
    }

    char filename[255];
    for (int i = 2; i < argc; i++) {
        sprintf(filename, "./%s", argv[i]);
        printf("          Loading: %s\n", argv[i]);
        int status = append_driver(drivers, filename);
        if (!status) {
            printf("[  Ok   ] Done driver %s\n", argv[i]);
        } else {
            printf("[Warning] Driver %s not loaded\n", argv[i]);
        }
    }
    printf("\n");

    i = 0;

    while (code[i] != '\0') {
        switch (code[i]) {
            case '+':
                tape[pos]++;
                break;
            case '-':
                tape[pos]--;
                break;
            case '>':
                if (pos == tape_length - 1) {
                    tape = realloc(tape, tape_length * 2);
                    zero(tape, tape_length, tape_length);
                    tape_length *= 2;
                }
                pos++;
                break;
            case '<':
                if (pos <= 0) return -1;
                pos--;
                break;
            case ',':
                driver_to_tape(pos, tape, drivers);
                break;
            case '.':
                tape_to_driver(pos, tape, drivers);
                break;
            case '[':
                if (code[i] == '[' && code[i + 1] == '-' && code[i + 2] == ']') {
                    tape[pos] = 0;
                    i += 1;
                } else if (tape[pos] == 0) {
                    next_bracket(code, &i, 1);
                }
                break;
            case ']':
                if (tape[pos]) {
                    next_bracket(code, &i, -1);
                }
                break;
        }
        i++;
    }

    fclose(f);
    for (int i = 2; i < argc; i++) {
        free(drivers->list[i]);
    }
    free(drivers);
    free(code);
    free(tape);

    return 0;
}
