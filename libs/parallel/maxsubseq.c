#include "maxsubseq.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

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
            *next_index = i;
        } else {
            size_t j = temp_size;
            while(temp[j] != input[i] && j > start_i){
                --j;
            }
            *next_index = start_i + j + 1;
            break;
        }

    }


    return temp_size;
}

size_t get_right_des(char * input, size_t input_size, size_t end_i){

    char temp[100] = {};
    size_t temp_size = 0;

    size_t i = end_i ;
    while( strchr(temp, input[i]) == NULL && i >= end_i - input_size+1 ){

        ++temp_size;
        temp[end_i - i] = input[i];
        --i;

    }

    return temp_size;
}


// функция возвращает размер и дескриптор самой большой подпоследовательности в массиве
size_t max_subseq(char * input, size_t start ,size_t input_size, substr_d * max){

    if(max == NULL) return 0;

    size_t current_i = start;
    size_t next_i = start;

    max->substr_size = 0;
    max->index = start;

    while(current_i < input_size){

        size_t next_size = get_des(input, input_size, current_i, &next_i);

        //printf("%d %d\n %d \n", current_i, input_size, start);

        if(max->substr_size < next_size){
            max->substr_size = next_size;
            max->index = current_i;
        }

        current_i = next_i;

    }

    return max->substr_size;
}


size_t merge(char * input, size_t left, size_t m, size_t right, substr_d * result){

    size_t temp_size = max_subseq(input, left,right - left, result);

    if(temp_size > m - left + 1 && temp_size > right-m){
        return temp_size;
    } else
        return 0;

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


        size_t right = file_size - 1;
        size_t left = 0;
        size_t m = left + (right - left) / 2;

        int status;
        int pid = fork();

        if( pid != 0 ){
            max_subseq(shared_input,0 ,m - left ,&shared_buffer[0]);

            shared_buffer[1].substr_size = get_right_des(shared_input, m+1, m);
            shared_buffer[1].index = m - shared_buffer[1].substr_size + 1;
            wait(&status);
//            printf("%d %d\n" , shared_buffer[0].index , shared_buffer[0].substr_size);
//            printf("%d %d\n" , shared_buffer[1].index , shared_buffer[1].substr_size);
        } else {
            size_t t = 0;
            shared_buffer[2].index = m + 1;
            shared_buffer[2].substr_size = get_des(shared_input, right - m, m + 1, &t);

            max_subseq(shared_input, m + 1, right - m, &shared_buffer[3]);
//            printf("%d %d\n" , shared_buffer[2].index , shared_buffer[2].substr_size);
//            printf("%d %d\n" , shared_buffer[3].index , shared_buffer[3].substr_size);
            exit(EXIT_SUCCESS);

        }

        size_t max = 0;
        for(size_t i = 1; i < 4 ; ++i){
            if( shared_buffer[i].substr_size > shared_buffer[max].substr_size )
                max = i;
        }


        substr_d * temp = (substr_d*)malloc(sizeof(substr_d));
        merge(shared_input, shared_buffer[1].index, m, shared_buffer[2].index + shared_buffer[2].substr_size+1 ,temp);


        if(temp == NULL || shared_buffer[max].substr_size >= temp->substr_size ){
            char *out = (char *) malloc(shared_buffer[max].substr_size + 1);
            strncpy(out, shared_input + shared_buffer[max].index, shared_buffer[max].substr_size );
            out[shared_buffer[max].substr_size] = '\0';
            fprintf(fout, "%s\n", out);

            free(out);
        } else {
            char *out = (char *) malloc(temp->substr_size + 1);
            strncpy(out, shared_input + temp->index, temp->substr_size);

            fprintf(fout, "%s\n", out);

            free(out);

        }

        free(temp);

        munmap(shared_input, file_size);
        munmap(shared_buffer, sizeof(substr_d) * 4 );

        return 1;
    } else
        return 0;
}
