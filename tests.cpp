#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C"{
#include "address.h"
#include "address.c"
#include <string.h>
#include <stdio.h>
}

//===========COMPLEX SET======================================
TEST(addr, all1){
const char * text = "http://mail.ru/index.html";
struct address * addr = parse(text);
ASSERT_EQ(0,strcmp(addr->protocol,"http"));
ASSERT_EQ(0,strcmp(".ru",addr->top_level_domain));
ASSERT_EQ(0,strcmp("mail",addr->sub_domains));
ASSERT_EQ(0,strcmp("/index.html",addr->url));
address_free(addr);
}

TEST(addr, all2){
const char * text = "ftp:/mail.ru/index.html";
struct address * addr = parse(text);
ASSERT_EQ(addr,nullptr);
address_free(addr);
}


TEST(addr, all3){
const char * text = "https://yandex.ru/search/dr.txt";
struct address * addr = parse(text);
ASSERT_EQ(0,strcmp(addr->protocol,"https"));
ASSERT_EQ(0,strcmp(addr->top_level_domain,".ru"));
ASSERT_EQ(0,strcmp(addr->sub_domains,"yandex"));
ASSERT_EQ(0,strcmp(addr->url,"/search/dr.txt"));
address_free(addr);
}



//=============PROTOCOL SET==================================
TEST(addr, protocol_parse_check1){
const char * text = "http://mail.ru/index.html";
char * answer = parse_protocol(text,strlen(text));
ASSERT_EQ(0,strcmp("http",answer));
free(answer);
}

TEST(addr, protocol_parse_check2){
const char * text = "ftp://mail.ru/index.html";
char * answer = parse_protocol(text,strlen(text));
ASSERT_EQ(0,strcmp("ftp",answer));
free(answer);
}

TEST(addr, protocol_validation_check1){
const char * text = "http://mail.ru/index.html";
char * answer = parse_protocol(text,strlen(text));
int valid = protocol_is_valid(answer);
EXPECT_EQ(1,valid);
free(answer);
}

TEST(addr, protocol_validation_check2){
const char * text = "htp://mail.ru/index.html";
char * answer = parse_protocol(text,strlen(text));
int valid = protocol_is_valid(answer);
EXPECT_EQ(0,valid);
free(answer);
}


TEST(addr, protocol_set_check2){
const char * text = "ftp";
struct address addr;
int answer = set_protocol(text,strlen(text),&addr);
EXPECT_EQ(answer,1);
ASSERT_EQ(0,strcmp("ftp",addr.protocol));
free(addr.protocol);
}

TEST(addr, protocol_set_check3){
const char * text = "file";
struct address addr;
int answer = set_protocol(text,strlen(text),&addr);
EXPECT_EQ(answer,1);
ASSERT_EQ(0,strcmp("file",addr.protocol));
free(addr.protocol);
}


int main(int argc, char**argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();

}
