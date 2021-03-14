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
const char * text = "ftp://mail.ruindex.html";
struct address * addr = parse(text);
ASSERT_EQ(addr,nullptr);
address_free(addr);
}


TEST(addr, all4){
const char * text = "https://yandex.ru/search/dr.txt";
struct address * addr = parse(text);
ASSERT_EQ(0,strcmp(addr->protocol,"https"));
ASSERT_EQ(0,strcmp(addr->top_level_domain,".ru"));
ASSERT_EQ(0,strcmp(addr->sub_domains,"yandex"));
ASSERT_EQ(0,strcmp(addr->url,"/search/dr.txt"));
address_free(addr);
}



//=============PROTOCOL SET==================================
TEST(addr, protocol_set_check1){
const char * text = "http://mail.ru/index.html";
struct address addr;
int answer = set_protocol(text,strlen(text),&addr);
EXPECT_EQ(answer,1);
EXPECT_EQ(0,strcmp("http",addr.protocol));
free(addr.protocol);
}

TEST(addr, protocol_set_check2){
const char * text = "ftp://ma.ru/index.html";
struct address addr;
int answer = set_protocol(text,strlen(text),&addr);
EXPECT_EQ(answer,1);
ASSERT_EQ(0,strcmp("ftp",addr.protocol));
free(addr.protocol);
}

TEST(addr, protocol_set_check3){
const char * text = "fp://mail.ru/index.html";
struct address addr;
int answer = set_protocol(text,strlen(text),&addr);
EXPECT_EQ(answer,0);
}


TEST(addr, protocol_set_check4){
const char * text = "blablabla";
struct address addr;
int answer = set_protocol(text,strlen(text),&addr);
EXPECT_EQ(answer,0);
}




//============URL SET==================================
TEST(addr, url_set_check_correct){
const char * text = "http://mail.ru/index.html";
struct address addr = {"http"};
int answer2 = set_url(text,strlen(text),&addr);
EXPECT_EQ(answer2,1);
ASSERT_EQ(0,strcmp("/index.html",addr.url));
free(addr.url);
}

TEST(addr, url_set_check_correct2){
const char * text = "ftp://google.com/stat/index.html";
struct address addr = {"ftp"};
int answer2 = set_url(text,strlen(text),&addr);
EXPECT_EQ(answer2,1);
ASSERT_EQ(0,strcmp("/stat/index.html",addr.url));
free(addr.url);
}

TEST(addr, url_set_check_incorrect3){
const char * text = "ftp://index.html";
struct address addr;
int answer2 = set_url(text,strlen(text),&addr);
EXPECT_EQ(answer2,0);
}


TEST(addr, url_set_check_incorrect4){
const char * text = "blablabla";
struct address addr;
int answer2 = set_url(text,strlen(text),&addr);
EXPECT_EQ(answer2,0);
}
//============DOMAIN SET================================
TEST(addr, domain_set_check_correct1){
const char * text = "http://www.google.com/index.html";
struct address addr;
addr.protocol = "http";
addr.url = "/index.html";
int answer = set_domain(text,strlen(text),&addr);
EXPECT_EQ(answer,1);
ASSERT_EQ(0,strcmp("www.google",addr.sub_domains));
ASSERT_EQ(0,strcmp(".com",addr.top_level_domain));
free(addr.sub_domains);
free(addr.top_level_domain);
}

TEST(addr, domain_set_check_correct2){
const char * text = "ftp://mail.ru/stat/index.html";
struct address addr;
addr.protocol = "ftp";
addr.url = "/stat/index.html";
int answer = set_domain(text,strlen(text),&addr);
ASSERT_EQ(0,strcmp("mail",addr.sub_domains));
ASSERT_EQ(0,strcmp(".ru",addr.top_level_domain));
free(addr.sub_domains);
free(addr.top_level_domain);
}

TEST(addr, domain_set_check_incorrect1){
const char * text = "ftp://index.html";
struct address addr; 
addr.protocol = "ftp";
addr.url = "/index.html";
int answer = set_domain(text,strlen(text),&addr);
EXPECT_EQ(answer,0);
}


TEST(addr, domain_set_check_incorrect2){
const char * text = "blablabfgfdgfdgla";
struct address addr;
int answer = set_domain(text,strlen(text),&addr);
EXPECT_EQ(answer,0);
}

//================================================



int main(int argc, char**argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();

}
