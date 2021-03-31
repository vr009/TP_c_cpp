#include "maxsubseq.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

struct substr_descriptor;

typedef struct substr_descriptor substr_d;


// возвращаем размер неповторяющейся последовательнотси с текущего индекса(start_i) в массиве
size_t get_des(char * input, size_t input_size, size_t start_i, size_t * next_index){

    char temp[100] = {};
    size_t temp_size = 0;

    size_t i = start_i;
    while( i <  input_size + start_i ){

        if (strchr(temp, input[i]) == NULL && input[i] != '\n'){
            temp[temp_size] = input[i];
            ++temp_size;
            ++i;
            *next_index = i + 1;
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


// данная функция понадобилась для того чтобы получить размер неповторяющейся последовательности для правой границы подмассива
// используется при мердже граничных подмассивов

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

    while(current_i < input_size && input[current_i] != '\n'){

        size_t next_size = get_des(input, input_size, current_i, &next_i);

        if(max->substr_size < next_size){
            max->substr_size = next_size;
            max->index = current_i;
        }

        current_i = next_i;

    }

    return max->substr_size;
}


//=====================================================================================


char * MT_trigger( char * shared_input, size_t file_size ){

    if( shared_input != NULL ){

        size_t process = sysconf(_SC_NPROCESSORS_ONLN) % (file_size - 1);
        if (process <= 0) {
            return NULL;
        }



        //создаем область для общего хранения дескрипторов
        substr_d *shared_max_buffer = (substr_d*)mmap(NULL, sizeof(substr_d) * process , PROT_READ | PROT_WRITE,
                                                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);


        //область для граничных дескрипторов
        substr_d *shared_merged_buffer = (substr_d*)mmap(NULL, sizeof(substr_d) * (process - 1) , PROT_READ | PROT_WRITE,
                                                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);


        if ( shared_input == NULL || shared_max_buffer == NULL || shared_merged_buffer == NULL ) {
            printf("Failed to map\n");
            return NULL;
        }


        size_t section_size = (file_size)/process;


        for (size_t i = 0; i < process; ++i) {

            int pid = fork();
            //не вышло
            if (pid == -1) {

                munmap(shared_max_buffer, sizeof(substr_d) * process);
                munmap(shared_merged_buffer, sizeof(substr_d) * (process - 1));
                return NULL;
            }

            //вышло, мы в потомке
            if (pid == 0) {
                //разбить на несколько частей массив
                //для каждой части запустить поток

                max_subseq(shared_input, i*section_size  ,section_size , &shared_max_buffer[i]);

                exit(EXIT_SUCCESS);
            }
        }

        //проверяем все процессы
        int status = 0;
        for (size_t i = 0; i < process; ++i) {

            wait(&status);
        }


        //merge section
        for(size_t i = 1; i < process; ++i){

            size_t a = 0;
            size_t left_index = section_size * i - get_right_des(shared_input, section_size, i * section_size);

            size_t right_index = section_size * i + get_des(shared_input, file_size - section_size*i , i * section_size + 1, &a);

            max_subseq(shared_input, left_index , right_index + 1, &shared_merged_buffer[i - 1]);

        }


        size_t max = 0;
        for(size_t i = 1; i < process ; ++i){
            if( shared_max_buffer[i].substr_size > shared_max_buffer[max].substr_size )
                max = i;


        }


        size_t merge_max = 0;
        for(size_t i = 1; i < process ; ++i){

            if( shared_merged_buffer[i].substr_size > shared_merged_buffer[merge_max].substr_size )
                merge_max = i;

        }


        if( process > 1 && shared_merged_buffer[merge_max].substr_size > shared_max_buffer[max].substr_size){
            shared_max_buffer[max] = shared_merged_buffer[merge_max];
        }


        char *out = (char *) malloc(shared_max_buffer[max].substr_size + 1);
        strncpy(out, shared_input + shared_max_buffer[max].index, shared_max_buffer[max].substr_size );


        munmap(shared_max_buffer, sizeof(substr_d) * process );
        munmap(shared_merged_buffer, sizeof(substr_d) * (process - 1) );

        return out;
    } else
        return NULL;
}

