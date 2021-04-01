#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

extern "C"{

#include <sys/mman.h>
#include "file_worker.h"
#include "file_worker.c"
}

//===========COMPLEX SET======================================
TEST(full, all1){
const char * file_name = "../../test/aaa.txt";
    FILE *f = fopen(file_name, "r");
    if(f != nullptr){

        size_t file_size = getFileSize(f);

        EXPECT_EQ(35, file_size);

        fclose(f);
    }
}

TEST(full, all2){
const char * file_name = "../../test/autotest.txt";

FILE * f = fopen(file_name, "r");
    if(f != nullptr){
        size_t file_size = getFileSize(f);

        EXPECT_EQ(1000167, file_size);

        fclose(f);
    }
}



int main(int argc, char**argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();

}