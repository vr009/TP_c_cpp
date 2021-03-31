/*
 * Вариант 18
 * Сравните и выведите в консоль время работы последовательного и параллельного
 * с использованием нескольких процессов алгоритмов, каждый из которых выделяет
 * в динамической памяти символьный массив размером 100 Мб и выполняет поиск
 * в тексте максимальной последовательности неповторяющихся символов.
 *
 * */

#include <dlfcn.h> //для работы с динамической библиотекой
#include <stdio.h>
//#include "libs/parallel/maxsubseq.h"
#include "libs/simple/maxsubseq.h"
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>


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





int main(int argc, char **argv){

    if(argc < 2){
        printf("Usage <program> <input_data.txt>");
        return 0;
    }

    FILE *f = fopen(argv[1], "r");

    if(f != NULL){

        size_t file_size = getFileSize(f);

        char * input = mmap(NULL, file_size - 1, PROT_READ, MAP_SHARED | MAP_PRIVATE, fileno(f), 0);

        if ( input == NULL ) {
            printf("Failed to map\n");
            return 1;
        }

        clock_t start_t, end_t;
        //Simple implementation
        start_t = clock();

        char * output = trigger(input, file_size - 1);
        if(output != NULL)
            printf("Simple implementation's answer: %s\n", output);
        else
            printf("Empty string\n");

        end_t = clock();

        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("(Simple version) Time taken: %lf\n", total_t);

        free(output);


        //Parallel implementtion

        //работа с динамической библиотекой

        void* library;
        library = dlopen("./libSUBSEQUENCE_STR_PARALLEL.so", RTLD_NOW);

        if (NULL == library) {
            munmap(input, file_size);
            printf("Lib ./libSUBSEQUENCE_STR_PARALLEL.so not found \n");
            return -1;
        }

        //создаем указатель на функцию
        char * (*mt_trigger)(char* shared_input, size_t f_size);
        mt_trigger = dlsym(library, "MT_trigger");

        start_t = clock();

        output = mt_trigger(input, file_size);
        printf("Parallel implementation's result: %s\n", output);

        end_t = clock();

        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("(Parallel version) Time taken: %lf\n", total_t);

        dlclose(library);

        free(output);
        munmap(input, file_size);
        fclose(f);

    } else {
        printf("File not found\n");
    }

    return 0;
}