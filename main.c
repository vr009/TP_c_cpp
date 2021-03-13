#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct address{
    char protocol[15];
    char top_level_domain[10];
    char sub_domains[65];
    char url[65];
};
typedef struct address address;

int get_protocol(const char *input,size_t size_,address * addr){
    int pr_size = 0;
    while (input[pr_size]!=':' & pr_size < size_){
        pr_size++;
    }
    if(pr_size == size_) return 0;
    //TODO validation
    strncpy(addr->protocol,input,pr_size);
    return 1;
}


int get_url(const char *input,size_t size_,address * addr){
    int url_size = 0;
    size_t start_index=strlen(addr->protocol)+3; // <protocol> + sizeof('://')
    while(input[start_index]!= '/' & input[start_index]!= '\0'){
        ++start_index;
    }
    if(start_index == size_) return 0;

    for(size_t i = start_index; i < size_;++i){
        addr->url[i-start_index] = input[i];
    }
    return 1;
}

int get_domain(const char *input,size_t size_,address * addr){
    int dom_size = 0;
    int domains_number = 0;
    size_t start_index = strlen(addr->protocol)+3; // <protocol prefix> + "://"
    size_t end_index = size_ - strlen(addr->url)-1;

    for(size_t i = start_index; i < end_index;++i){
        if(input[i] == '.') domains_number++;
    }
    if(domains_number == 0 || input[start_index] == '.') return 0;


    while (input[end_index]!='.'){
        dom_size++;
        end_index--;
    }
    for (size_t i = end_index; i <= end_index+dom_size;++i){
        addr->top_level_domain[i-end_index] = input[i];
    }

    for (size_t i = start_index; i < end_index; ++i) {
        addr->sub_domains[i-start_index] = input[i];
    }

    //TODO validation
    return 1;
}


address* parse(const char*input){
    int size_ = strlen(input);
    address *addr = (address*)malloc(sizeof(struct address));
    //get protocol
    if(!get_protocol(input,size_,addr)) return NULL;
    // get url
    if(!get_url(input,size_,addr)) return NULL;
    //get domain
    if(!get_domain(input,size_,addr)) return NULL;
    return addr;
}


void print_info(address * addr){
    printf("Protocol: %s\n", addr->protocol);
    printf("Sub-Domain: %s\n",addr->sub_domains);
    printf("Top-Domain: %s\n",addr->top_level_domain);
    printf("Url: %s\n",addr->url);
}

int main(int argc, char**argv){
    char input[100];
    gets(input);
    address* temp = parse(input);
    print_info(temp);
    return 0;
}
