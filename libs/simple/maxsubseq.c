#include "maxsubseq.h"
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <sys/mman.h>

// данная структура введена для простоты сравнения подпоследовательностей поле-индекс указывает на начало подпоследовательности
struct substr_descriptor;

typedef struct substr_descriptor substr_d;


// возвращаем размер неповторяющейся последовательнотси с текущего индекса(start_i) в массиве
size_t get_des(char * input, size_t input_size, size_t start_i, size_t * next_index){

    char temp[100] = {};
    size_t temp_size = 0;

    size_t i = start_i;
    while( i <=  input_size){

        if (strchr(temp, input[i]) == NULL && input[i] != '\n'){
            temp[temp_size] = input[i];
            ++temp_size;
            ++i;
        } else {
            size_t j = temp_size;
            while(temp[j] != input[i]){
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


//=============logic for a library trigger =================

// подсчет символов в файле
int64_t getFileSize(FILE *f){

    int64_t _file_size = 0;

    struct stat _fileStatbuff;
    int fd = fileno(f);

    if(fd == -1){
        _file_size = -1;
    }

    else{
        if ((fstat(fd, &_fileStatbuff) != 0) || (!S_ISREG(_fileStatbuff.st_mode))) {
            _file_size = -1;
        }
        else{
            _file_size = _fileStatbuff.st_size;
        }
    }
    return _file_size;
}


//=======================================================================

int trigger(FILE *f, FILE *fout){

    if( f != NULL && fout != NULL ){

        size_t file_size = getFileSize(f);

        char * input = mmap(NULL, file_size, PROT_READ, MAP_SHARED | MAP_PRIVATE, fileno(f), 0);

        if ( input == NULL ) {
            printf("Failed to map\n");
            return 1;
        }

        substr_d *temp = (substr_d *)malloc(sizeof(substr_d));


        char *out = (char *) malloc(temp->substr_size + 1);
        strncpy(out, input + temp->index , temp->substr_size);

        fprintf(fout, "%s\n", out);

        free(out);
        free(temp);

        munmap(input, file_size);

        return 1;
    } else
        return 0;
}
