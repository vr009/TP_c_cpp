#include "maxsubseq.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    while( check_symbol(temp,0,temp_size,input[i]) == -1 && i < start_i + input_size){

        ++temp_size;
        temp[i - start_i] = input[i];
        ++i;

    }

    substr_d * out_descriptor = (substr_d*) malloc( sizeof(substr_d) );

    out_descriptor->index = start_i;
    out_descriptor->substr_size = temp_size + 1;

    return out_descriptor;
}

substr_d * get_right_des(char * input, size_t input_size, size_t end_i){

    char temp[100];
    size_t temp_size = 0;

    size_t i = end_i ;
    while( check_symbol(temp,0,temp_size,input[i]) == -1 && i >= end_i - input_size+1 ){

        ++temp_size;
        temp[end_i - i] = input[i];
        --i;

    }

    substr_d * out_descriptor = (substr_d*) malloc( sizeof(substr_d) );
    out_descriptor->index = end_i - input_size + 1;
    out_descriptor->substr_size = temp_size;

    return out_descriptor;
}





substr_d * merge(char * input, substr_d * left, substr_d * right){

    substr_d * temp = max_subseq(input,left->index, right->index + right->substr_size - 1);

    if(temp->substr_size > left->substr_size && temp->substr_size > right->substr_size){
        return temp;
    } else {

        free(temp);
        return NULL;
    }

}



// функция возвращает дескриптор самой большой подпоследовательности в массиве
substr_d * max_subseq(char * input, size_t start_i, size_t input_size){

    substr_d * base = get_des(input, input_size, start_i);

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


//================================================


int trigger(FILE *f, FILE *fout){

    if( f != NULL && fout != NULL ){

        size_t file_size = getFileSize(f);

        // загружаем файл в память
        char * shared_input = mmap(NULL, file_size, PROT_READ, MAP_SHARED | MAP_PRIVATE, fileno(f), 0);

        //создаем область для общего хранения дескрипторов
        substr_d *shared_buffer = (substr_d*)mmap(NULL, sizeof(substr_d) * 4 , PROT_READ | PROT_WRITE,
                                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        if ( shared_input == NULL || shared_buffer == NULL ) {
            printf("Failed to map\n");
            return 1;
        }

        int status;
        int pid = fork();

        size_t right = file_size - 1;
        size_t left = 0;
        size_t m = left + (right - left) / 2;

        if( pid == 0 ){

            shared_buffer[0] = *max_subseq(shared_input, left, m);
            shared_buffer[1] = *get_right_des(shared_input, m+1, m);
            exit(0);
        } else {

            shared_buffer[2] = *get_des(shared_input, right - m, m + 1);
            shared_buffer[3] = *max_subseq(shared_input, m + 1, right - m);
            waitpid(pid,&status,4);
        }

        substr_d * temp = merge(shared_input, &shared_buffer[1], &shared_buffer[2]);

        size_t max = 0;
        for(size_t i = 1; i < 4 ; ++i){
            if( shared_buffer[i].substr_size > shared_buffer[max].substr_size )
                max = i;
        }


        if(temp == NULL || shared_buffer[max].substr_size <= temp->substr_size ){

            char *out = (char *) malloc(shared_buffer[max].substr_size + 1);
            strncpy(out, shared_input + shared_buffer[max].index, shared_buffer[max].substr_size );
            out[shared_buffer[max].substr_size] = '\0';
            fprintf(fout, "%s\n", out);

        } else {

            char *out = (char *) malloc(temp->substr_size + 1);
            strncpy(out, shared_input + temp->index, temp->substr_size);

            fprintf(fout, "%s\n", out);

        }


        munmap(shared_input, file_size);
        munmap(shared_buffer, sizeof(substr_d) * 4 );

        return 1;
    } else
        return 0;
}
