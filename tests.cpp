#include "gtest/gtest.h"
#include "gmock/gmock.h"
extern "C"{
#include "address.h"

}
<<<<<<< HEAD

//===========COMPLEX SET======================================
TEST(addr, all){
const char * text = "http://mail.ru/index.html";
address * addr = parse(text);
int answer = get_protocol(text,strlen(text),addr);
ASSERT_EQ(answer,1);
ASSERT_EQ("http",addr->protocol);
ASSERT_EQ(".ru",addr->top_level_domain);
ASSERT_EQ("mail",addr->sub_domains);
ASSERT_EQ("/index.html",addr->url);
address_free(addr);
}

TEST(addr, all2){
const char * text = "ftp:/mail.ru/index.html";
address * addr = parse(text);
ASSERT_EQ(text,NULL);
ASSERT_EQ(answer,0);
address_free(addr);
}

TEST(addr, all2){
const char * text = "ftp://mail.ruindex.html";
address * addr = parse(text);
ASSERT_EQ(text,NULL);
ASSERT_EQ(answer,0);
address_free(addr);
}


TEST(addr, all2){
const char * text = "https://yandex.ru/search/dr.txt";
address * addr = parse(text);

ASSERT_EQ(addr->protocol,"https");
ASSERT_EQ(addr->top_level_domain,".ru");
ASSERT_EQ(addr->sub_domains,"yandex");
ASSERT_EQ(addr->url,"/search/dr.txt");
address_free(addr);
}



//=============PROTOCOL SET==================================
TEST(addr, protocol_set_check){
const char * text = "http://mail.ru/index.html";
address addr;
int answer = set_protocol(text,strlen(text),&addr);
ASSERT_EQ(answer,1);
ASSERT_EQ("http",addr->protocol);
}

TEST(addr, protocol_set_check){
const char * text = "ftp://ma/index.html";
address addr;
int answer = set_protocol(text,strlen(text),&addr);
ASSERT_EQ(answer,1);
ASSERT_EQ("ftp",addr->protocol);
}

TEST(addr, protocol_set_check){
const char * text = "fp://mail.ru/index.html";
address addr;
int answer = set_protocol(text,strlen(text),&addr);
ASSERT_EQ(answer,0);
}


TEST(addr, protocol_set_check){
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
ASSERT_EQ("/index.html",addr->protocol);
}

TEST(addr, url_set_check_correct){
const char * text = "ftp://google.com/stat/index.html";
address addr;
int answer = set_url(text,strlen(text),&addr);
ASSERT_EQ(answer,1);
ASSERT_EQ("/stat/index.html",addr->url);
}

TEST(addr, url_set_check_incorrect){
const char * text = "ftp://index.html";
address addr;
int answer = set_url(text,strlen(text),&addr);
ASSERT_EQ(answer,0);
}


TEST(addr, url_set_check_incorrect){
const char * text = "blablabla";
address addr;
int answer = set_url(text,strlen(text),&addr);
ASSERT_EQ(answer,0);
}
//============DOMAIN SET================================
TEST(addr, url_set_check_correct){
const char * text = "http://www.google.com/index.html";
address addr;
int answer = set_domain(text,strlen(text),&addr);
ASSERT_EQ(answer,1);
ASSERT_EQ("www.google",addr->protocol);
ASSERT_EQ(".com",addr->protocol);
}

TEST(addr, url_set_check_correct){
const char * text = "ftp://mail.ru/stat/index.html";
address addr;
int answer = set_domain(text,strlen(text),&addr);
ASSERT_EQ("mail",addr->protocol);
ASSERT_EQ("ru",addr->protocol);
}

TEST(addr, url_set_check_incorrect){
const char * text = "ftp://index.html";
address addr;
int answer = domain(text,strlen(text),&addr);
ASSERT_EQ(answer,0);
}


TEST(addr, url_set_check_incorrect){
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
