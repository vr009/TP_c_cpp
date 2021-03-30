#ifndef MAX_SUB_H
#define MAX_SUB_H
#include <stdio.h>
#include <string.h>
struct substr_descriptor {
    size_t index;
    size_t substr_size;
};
typedef struct substr_descriptor substr_d;

int trigger(FILE * f, FILE *fout);
size_t max_subseq(char * input, size_t input_size, substr_d * max);

#endif