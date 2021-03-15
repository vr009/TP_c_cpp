/*
 * Создать структуру для хранения информации об интернет-адресе: протоколе подключения к серверу,
 * домене верхнего уровня,доменной структуре на остальных уровнях и запрашиваемой странице.
 * Составить с ее использованием программу,включающую в себя функцию разбора переданного на вход адреса
 * (например, www.mail.ru).
 * На выход функция должна возвращать указатель на инициализированную структуру.
 * */
#include <stdio.h>
#include "address.h"

int main(int argc, char**argv){

    if(argc < 2){
        printf("Usage: <program> <file_with_data.txt> ...");
    } else{
        for(int i =1;i < argc;++i){
            FILE *f=fopen(argv[i],"r");
            if(f!=NULL){
                char input[100];
                fscanf(f,"%s",input);
                address *temp = parse(input);
                printf("-Data from file %s:\n",argv[i]);
                if (temp == NULL) printf("Invalid input\n");
                else print_info(temp);
                address_free(temp);
                fclose(f);
            } else printf("\n-file not found\n");
        }
    }



    return 0;
}
