#include "maxsubseq.h"
#include <stddef.h>
#include <stdlib.h>

// данная структура введена для простоты сравнения подпоследовательностей поле-индекс указывает на начало подпоследовательности
struct substr_descriptor;

typedef struct substr_descriptor substr_d;


// возвращаем размер неповторяющейся последовательнотси с текущего индекса(start_i) в массиве
size_t get_des(char * input, size_t input_size, size_t start_i, size_t * next_index){

    char temp[100] = {};
    size_t temp_size = 0;

    size_t i = start_i;
    while( i <= input_size){

        if (strchr(temp, input[i]) == NULL && input[i] != '\n'){

            temp[temp_size] = input[i];
            ++temp_size;
            ++i;
            *next_index += temp_size ;
        } else {
            size_t j = temp_size - 1 ;

            while(temp[j] != input[i] && j > 0){
                --j;
            }

            *next_index = start_i + j + 1;
            break;
        }

    }

    return temp_size;
}


// функция возвращает размер и дескриптор самой большой подпоследовательности в массиве
size_t max_subseq(char * input, size_t input_size, substr_d * max){

    if(max == NULL) return 0;

    size_t current_i = 0;
    size_t next_i = 0;

    max->substr_size = 0;
    max->index = 0;

    while(current_i < input_size){

        size_t next_size = get_des(input, input_size, current_i, &next_i);

        if(max->substr_size < next_size){
            max->substr_size = next_size;
            max->index = current_i;
        }

        current_i = next_i;
    }

    return max->substr_size;
}

//=======================================================================

char * trigger(const char * input,size_t file_size){

    if( input != NULL ){

        substr_d *temp = (substr_d *)malloc(sizeof(substr_d));

        max_subseq(input, file_size , temp);

        char *out = (char *) malloc(temp->substr_size + 1);
        strncpy(out, input + temp->index , temp->substr_size);

        free(temp);

        return out;
    } else
        return NULL;
}
