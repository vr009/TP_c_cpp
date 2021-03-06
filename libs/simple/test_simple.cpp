#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

extern "C"{
#include "./maxsubseq.h"
#include "./maxsubseq.c"
#include <sys/mman.h>
#include "../file_worker/file_worker.h"
}

//===========COMPLEX SET======================================

TEST(full, all1){

    char input[] = "saliutrewqkdjfhg[";
    size_t size = strlen(input);
    char * answer = trigger(input, size );
    EXPECT_EQ(0, strcmp(answer, "saliutrewqkdjfhg["));
    free(answer);
}

TEST(full, all2){

    char input[] = "aaaaaaaaaaaaaaa";
    size_t size = strlen(input);
    char * answer = trigger(input, size );
    EXPECT_EQ(0, strcmp(answer, "a"));
    free(answer);

}


TEST(full, all3){

    char input[] = "asasasasasasasasasasasavbnhg";
    size_t size = strlen(input);
    char * answer = trigger(input, size );
    EXPECT_EQ(0, strcmp(answer, "savbnhg"));
    free(answer);
}


int main(int argc, char**argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();

}