#include "maxsubseq.h"
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
// данная структура введена для простоты сравнения подпоследовательностей поле-индекс указывает на начало подпоследовательности
struct substr_descriptor;

typedef struct substr_descriptor substr_d;

//проверка очередного символа в текущей подпоследовательности
int check_symbol(const char * text, size_t start_index,size_t end_index, char symbol){
    for(size_t i = start_index; i <= end_index; ++i){
        if(text[i] == symbol )
            return 1;
    }

    return -1;
}


// возвращаем неповторяющуюся подпоследовательность с текущего индекса(start_i) в массиве
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
    out_descriptor->substr_size = temp_size + 1;

    return out_descriptor;
}


// функция возвращает дескриптор самой большой подпоследовательности в массиве
substr_d * max_subseq(char * input, size_t input_size){

    substr_d * base = get_des(input, input_size, 0);

    size_t current_i = base->substr_size + base->index;                 // с текущего индекса будем считать очередной дескриптор подпоследовательности

    while(current_i <= input_size){
        substr_d * temp = get_des(input, input_size, current_i);
        current_i = temp->index + temp->substr_size;

        if( temp->substr_size > base->substr_size ){                    // по дескрипторам символьных неповторяющихся подпоследовательностей определяем которая из них больше
            free(base);
            base = temp;
        } else {
            free(temp);
        }
    }

    return base;
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



void fill(FILE *f, char * input, size_t file_size){

    fseek(f, 0, SEEK_SET);

    for (int i = 0; i < file_size; ++i) {
        fscanf(f, "%c", &input[i]);
    }
}

//================================================

int trigger(FILE *f, FILE *fout){

    if( f != NULL && fout != NULL ){

        size_t file_size = getFileSize(f);

        char *input = (char *) malloc(file_size);

        fill(f, input, file_size);

        substr_d *temp = max_subseq(input, file_size);

        char *out = (char *) malloc(temp->substr_size + 1);
        strncpy(out, input + temp->index, temp->substr_size);

        fprintf(fout, "%s\n", out);

        free(out);
        free(temp);
        free(input);

        return 1;
    } else
        return 0;
}
