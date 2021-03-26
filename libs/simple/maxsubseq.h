#ifndef MAX_SUB_H
#define MAX_SUB_H
#include <string.h>
struct substr_descriptor{
    size_t index;
    size_t substr_size;
};
typedef struct substr_descriptor substr_d;

substr_d * max_subseq(char * input, size_t size);

#endif