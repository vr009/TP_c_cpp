/*
 * Сравните и выведите в консоль время работы последовательного и параллельного
 * с использованием нескольких процессов алгоритмов, каждый из которых выделяет
 * в динамической памяти символьный массив размером 100 Мб и выполняет поиск
 * в тексте максимальной последовательности неповторяющихся символов.
 * */


#include <stdio.h>
#include "libs/simple/maxsubseq.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){
    char * input = "ekfnwierninornorsnveirnerifmweldkmwekefwefwf";
    size_t size = strlen(input);

    substr_d * temp = max_subseq(input,size);

    printf("%d %d\n", temp->substr_size, temp->index);


    char * out = (char*)malloc(temp->substr_size+1);
    memcpy(out, input + temp->index, temp->substr_size);
    out[temp->substr_size] = '\0';
    printf("%s\n", out);

    free(out);
    free(temp);
    return 0;
}