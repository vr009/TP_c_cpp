#ifndef MAX_SUB_P_H
#define MAX_SUB_P_H
#include <stdio.h>
#include <string.h>
struct substr_descriptor {
    size_t index;
    size_t substr_size;
};
typedef struct substr_descriptor substr_d;

char * MT_trigger(const char * shared_input, size_t file_size );
size_t max_subseq(char * input, size_t start, size_t input_size, substr_d * max);

#endif