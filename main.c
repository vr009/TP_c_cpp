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
    FILE *f=fopen("../correct_input.txt","r");

    if(f!=NULL){
        printf("here");
        char input[100];
        fscanf(f,"%s",input);
        address *temp = parse(input);
        if (temp == NULL) printf("Invalid input\n");
        else print_info(temp);
        address_free(temp);
        fclose(f);
    }

    return 0;
}
