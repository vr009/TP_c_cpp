#include "maxsubseq.h"
#include <stddef.h>
#include <stdlib.h>

/*char *strpbrk(const char *s, const char *accept);
 * - находит первое вхождение любого символа, перечисленного в accept*/



typedef struct substr_descriptor substr_d;


int check_symbol(char * text, size_t start_index,size_t end_index, char symbol){
    for(size_t i = start_index; i <= end_index; ++i){
        if(text[i] == symbol )
            return i;
    }

    return -1;
}


substr_d * get_des(char * input, size_t input_size, size_t start_i){

    char temp[100];
    size_t temp_size = 0;

    size_t i = start_i;
    while( check_symbol(temp,0,temp_size,input[i]) == -1 && i < input_size){

        ++temp_size;
        temp[i - start_i] = input[i];
        ++i;

    }

    substr_d * out_descriptor = (substr_d*) malloc( sizeof(substr_d) );
    out_descriptor->index = start_i;
    out_descriptor->substr_size = temp_size;

    return out_descriptor;

}


substr_d * max_subseq(char * input, size_t size){


    substr_d * base = get_des(input, size, 0);
    size_t current_i = base->substr_size + base->index;

    while(current_i != size){
        substr_d * temp = get_des(input, size, current_i);
        current_i = temp->index + temp->substr_size;

        if( temp->substr_size > base->substr_size ){
            free(base);
            base = temp;
        } else {
            free(temp);
        }
    }

    return base;
}