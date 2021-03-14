#include "gtest/gtest.h"
#include "gmock/gmock.h"

extern "C"{
#include "address.h"
#include <string.h>
}

//===========COMPLEX SET======================================
TEST(addr, all1){
const char * text = "http://mail.ru/index.html";
address * addr = parse(text);
ASSERT_EQ(1,strcmp(addr->protocol,"http"));
ASSERT_EQ(1,strcmp(".ru",addr->top_level_domain));
ASSERT_EQ(1,strcmp("mail",addr->sub_domains));
ASSERT_EQ(1,strcmp("/index.html",addr->url));
address_free(addr);
}

TEST(addr, all2){
const char * text = "ftp:/mail.ru/index.html";
address * addr = parse(text);
ASSERT_EQ(text,nullptr);
address_free(addr);
}

TEST(addr, all3){
const char * text = "ftp://mail.ruindex.html";
address * addr = parse(text);
ASSERT_EQ(text,nullptr);
address_free(addr);
}


TEST(addr, all4){
const char * text = "https://yandex.ru/search/dr.txt";
address * addr = parse(text);
ASSERT_EQ(strcmp(1,addr->protocol,"https"));
ASSERT_EQ(strcmp(addr->top_level_domain,".ru"));
ASSERT_EQ(strcmp(addr->sub_domains,"yandex"));
ASSERT_EQ(strcmp(addr->url,"/search/dr.txt"));
address_free(addr);
}



//=============PROTOCOL SET==================================
TEST(addr, protocol_set_check1){
const char * text = "http://mail.ru/index.html";
address addr;
int answer = set_protocol(text,strlen(text),&addr);
ASSERT_EQ(answer,1);
ASSERT_EQ(1,strcmp("http",addr->protocol));
}

TEST(addr, protocol_set_check2){
const char * text = "ftp://ma/index.html";
address addr;
int answer = set_protocol(text,strlen(text),&addr);
ASSERT_EQ(answer,1);
ASSERT_EQ(strcmp(1,"ftp",addr->protocol));
}

TEST(addr, protocol_set_check3){
const char * text = "fp://mail.ru/index.html";
address addr;
int answer = set_protocol(text,strlen(text),&addr);
ASSERT_EQ(answer,0);
}


TEST(addr, protocol_set_check4){
const char * text = "blablabla";
address addr;
int answer = set_protocol(text,strlen(text),&addr);
ASSERT_EQ(answer,0);
}




//============URL SET==================================
TEST(addr, url_set_check_correct){
const char * text = "http://mail.ru/index.html";
address addr;
int answer = set_URL(text,strlen(text),&addr);
ASSERT_EQ(answer,1);
ASSERT_EQ(1,strcmp("/index.html",addr->protocol));
}

TEST(addr, url_set_check_correct2){
const char * text = "ftp://google.com/stat/index.html";
address addr;
int answer = set_url(text,strlen(text),&addr);
ASSERT_EQ(answer,1);
ASSERT_EQ(strcmp("/stat/index.html",addr->url));
}

TEST(addr, url_set_check_incorrect3){
const char * text = "ftp://index.html";
address addr;
int answer = set_url(text,strlen(text),&addr);
ASSERT_EQ(answer,0);
}


TEST(addr, url_set_check_incorrect4){
const char * text = "blablabla";
address addr;
int answer = set_url(text,strlen(text),&addr);
ASSERT_EQ(answer,0);
}
//============DOMAIN SET================================
TEST(addr, domain_set_check_correct1){
const char * text = "http://www.google.com/index.html";
address addr;
int answer = set_domain(text,strlen(text),&addr);
ASSERT_EQ(answer,1);
ASSERT_EQ(1,strcmp("www.google",addr->protocol));
ASSERT_EQ(1,strcmp(".com",addr->protocol));
}

TEST(addr, domain_set_check_correct2){
const char * text = "ftp://mail.ru/stat/index.html";
address addr;
int answer = set_domain(text,strlen(text),&addr);
ASSERT_EQ(1,strcmp("mail",addr->protocol));
ASSERT_EQ(1,strcmp("ru",addr->protocol));
}

TEST(addr, domain_set_check_incorrect1){
const char * text = "ftp://index.html";
address addr;
int answer = domain(text,strlen(text),&addr);
ASSERT_EQ(answer,0);
}


TEST(addr, domain_set_check_incorrect2){
const char * text = "blablabfgfdgfdgla";
address addr;
int answer = set_domain(text,strlen(text),&addr);
ASSERT_EQ(answer,0);
}

//==============================================================



int main(int argc, char**argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();

}
