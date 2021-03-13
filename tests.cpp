#include "gtest/gtest.h"
#include "gmock/gmock.h"
extern "C"{
#include "address.h"

}
TEST(addr, prot){
        const char * text = "http://mail.ru/index.html";
        address * addr = parse(text);
        int answer = get_protocol(text,strlen(text),addr);
        ASSERT_EQ(answer,1);
    }

int main(int argc, char**argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS(); 

}
