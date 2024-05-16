#ifndef AUX_H
#define AUX_H

#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 1024
#define STUDENT_ID_LENGTH 6
#define EXERCISE_ID_LENGTH 4

struct CompilationParams {
    char buffer[BUFFER_SIZE];
    char lang[2];
    char studID[STUDENT_ID_LENGTH + 1];
    char exerciseID[EXERCISE_ID_LENGTH + 1];
    char input[BUFFER_SIZE];
    uint16_t input_length;
    uint16_t buffer_length;
};

void compile_program(struct CompilationParams *, char *, char *);

#endif
