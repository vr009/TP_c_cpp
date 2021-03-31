#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C"{
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "file_worker.h"
#include "file_worker.c"
}

//===========COMPLEX SET======================================
TEST(full, all1){
const char * file_name = "../../test/aaa.txt";

FILE * f = fopen(file_name, "r");
size_t file_size = getFileSize(f);

ASSERT_EQ(35, file_size);

fclose(f);
}

TEST(full, all2){
const char * file_name = "../../test/autotest.txt";

FILE * f = fopen(file_name, "r");
size_t file_size = getFileSize(f);

ASSERT_EQ(1000167, file_size);

fclose(f);
}



int main(int argc, char**argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();

}