/*
 * Сравните и выведите в консоль время работы последовательного и параллельного
 * с использованием нескольких процессов алгоритмов, каждый из которых выделяет
 * в динамической памяти символьный массив размером 100 Мб и выполняет поиск
 * в тексте максимальной последовательности неповторяющихся символов.
 * */

//C:\\\\Users\\\\slava\\\\Documents\\\\plusplus\\\\c_proj\\\\TP_c_cpp\\\\autotest.txt

#include <stdio.h>
//#include "libs/parallel/maxsubseq.h"
#include "libs/simple/maxsubseq.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){

    if(argc < 2){
        printf("Usage <program> <input_data.txt>");
        return 0;
    }

    FILE *f = fopen(argv[1], "r");
    if(f != NULL){

        if( !trigger(f, stdout) )
            printf("Some library trouble");

        fclose(f);

    } else {
        printf("File not found\n");
    }

    return 0;
}