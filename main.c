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
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>

#include "libs/simple/maxsubseq.h"
#include "libs/file_worker/file_worker.h"





int main(int argc, char **argv){

    //файл для вывода результата, по дефолту будем задавать stdout

    if(argc < 2){
        fprintf( stdout, "Usage <program> <input_data.txt> <output_file>\n");
        return 0;
    }

    FILE * f_out = NULL;

    if(argc == 3){
        f_out = fopen(argv[2],  "w" );
    }

    if( f_out == NULL || argc == 2){
        f_out = stdout;
    }


    FILE *f = fopen(argv[1], "r");

    if(f != NULL){

        size_t file_size = getFileSize(f);

        char * input = mmap(NULL, file_size - 1, PROT_READ, MAP_SHARED | MAP_PRIVATE, fileno(f), 0);

        if ( input == NULL ) {

            fprintf( f_out,"Failed to map\n");

            fclose(f);
            fclose(f_out);
            return 1;
        }

        clock_t start_t, end_t;

        //Simple implementation
        start_t = clock();

        char * output = trigger(input, file_size - 1);

        if(output != NULL)
            fprintf( f_out, "Simple implementation's answer: %s\n", output);
        else
            fprintf( f_out, "Empty string\n");

        end_t = clock();

        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        fprintf( f_out, "(Simple version) Time taken: %lf\n", total_t);

        free(output);


        //Parallel implementtion
        //работа с динамической библиотекой

        void* library;
        library = dlopen("./libSUBSEQUENCE_STR_PARALLEL.so", RTLD_NOW);

        if (NULL == library) {
            munmap(input, file_size);
            fprintf( f_out, "Lib ./libSUBSEQUENCE_STR_PARALLEL.so not found \n");

            fclose(f);
            fclose(f_out);
            return -1;
        }

        //создаем указатель на функцию
        char * (*mt_trigger)(char* shared_input, size_t f_size);
        mt_trigger = dlsym(library, "MT_trigger");

        start_t = clock();

        output = mt_trigger(input, file_size);

        if(output != NULL)
            fprintf( f_out,"Parallel implementation's result: %s\n", output);
        else
            fprintf( f_out, "Empty string\n");

        end_t = clock();

        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        fprintf( f_out,"(Parallel version) Time taken: %lf\n", total_t);

        dlclose(library);

        free(output);
        munmap(input, file_size);
        fclose(f);
        fclose(f_out);

    } else {
        fprintf( stdout, "File not found\n");
        fclose(f_out);
    }

    return 0;
}