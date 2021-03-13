//
// Created by slava on 13.03.2021.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef IZ1_TASK_ADDRESS_H
#define IZ1_TASK_ADDRESS_H
struct address;
typedef struct address address;
int get_protocol(const char *,size_t ,address *);
int get_url(const char *,size_t,address * );
int get_domain(const char *,size_t ,address *);
address* parse(const char*);
void print_info(address *);
#endif //IZ1_TASK_ADDRESS_H
