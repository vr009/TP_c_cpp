#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C"{
#include "maxsubseq.h"
#include "maxsubseq.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "../file_worker/file_worker.h"
}

//===========COMPLEX SET======================================
TEST(full, all1){
const char * file_name = "../../test/autotest.txt";

FILE * f = fopen(file_name, "r");
size_t file_size = getFileSize(f);

if (f != nullptr){
char * input = (char*)mmap(nullptr, file_size - 1, PROT_READ, MAP_SHARED | MAP_PRIVATE, fileno(f), 0);
char * answer = trigger(input, file_size - 1);
ASSERT_EQ(0, strcmp(answer, "saliutrewqkdjfhg["));
munmap(input, file_size - 1);
}

fclose(f);
}

TEST(full, all2){
const char * file_name = "../../test/simple_test.txt";

FILE * f = fopen(file_name, "r");
size_t file_size = getFileSize(f);

if (f != nullptr){
char * input = (char*)mmap(nullptr, file_size - 1, PROT_READ, MAP_SHARED | MAP_PRIVATE, fileno(f), 0);
char * answer = trigger(input, file_size - 1);
ASSERT_EQ(0, strcmp(answer, "qwertyuiopasdfghjklzxcvbnm"));
munmap(input, file_size - 1);
}


fclose(f);
}


TEST(full, all3){
const char * file_name = "../../test/aaa.txt";

FILE * f = fopen(file_name, "r");
size_t file_size = getFileSize(f);

if (f != nullptr){
char * input = (char*)mmap(nullptr, file_size - 1, PROT_READ, MAP_SHARED | MAP_PRIVATE, fileno(f), 0);
char * answer = trigger(input, file_size - 1);
ASSERT_EQ(0, strcmp(answer, "a"));
munmap(input, file_size - 1);
}


fclose(f);
}


int main(int argc, char**argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();

}