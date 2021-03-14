//
// Created by slava on 13.03.2021.
//

#include "address.h"

struct address{
    char *protocol;
    char *top_level_domain;
    char *sub_domains;
    char *url;
};
typedef struct address address;


int protocol_validate(const char * protocol){
    if(protocol != NULL){
        if(strcmp(protocol, "http") == 0) return 1;
        if(strcmp(protocol, "https") == 0) return 1;
        if(strcmp(protocol, "ftp") == 0) return 1;
        if(strcmp(protocol, "mailto") == 0) return 1;
        if(strcmp(protocol, "file") == 0) return 1;
        if(strcmp(protocol, "news") == 0) return 1;
        if(strcmp(protocol, "telnet")==0)return 1;
        return 0;
    } else return 0;
}

int set_protocol(const char *input,size_t size_,address * addr){
    if(input != NULL && addr!= NULL){
        int pr_size = 0;
        while (pr_size+2 < size_  & input[pr_size] != ':' ) {
            pr_size++;
        }
        if (pr_size > size_-3) return 0;
		if ( input[pr_size + 1] != '/' || input[pr_size + 2] != '/') return 0;
        if(addr->protocol = (char *) malloc(pr_size + 1)){
            memcpy(addr->protocol, input, pr_size);
            addr->protocol[pr_size] = '\0';
            return protocol_validate(addr->protocol);
        } else return 0;

    } else return 0;
}


int set_url(const char *input,size_t size_,address * addr){
    if(input != NULL && addr!= NULL){
        int url_size = 0;
        size_t start_index = strlen(addr->protocol) + 3; // <protocol> + sizeof('://')
        while (input[start_index] != '/' & input[start_index] != '\0') {
            ++start_index;
        }
        if (start_index == size_) return 0;

        url_size = size_ - start_index;

        if(addr->url = (char *) malloc(url_size + 1)){
            memcpy(addr->url, input + (size_ - url_size), url_size);
            addr->url[url_size] = '\0';
            return 1;
        } else return 0;

    } else return 0;
}

int set_domain(const char *input,size_t size_,address * addr){
    if(input != NULL && addr!= NULL){
        int dom_size = 0;
        int domains_number = 0;
        //interval between <protocol prefix> and url
        size_t start_index = strlen(addr->protocol) + 3; // length of: <protocol prefix> + "://"
        size_t end_index = size_ - strlen(addr->url) - 1;

        for (size_t i = start_index; i < end_index; ++i) {
            if (input[i] == '.') domains_number++;
        }
        if (domains_number == 0 || input[start_index] == '.') return 0;

        while (input[end_index] != '.') {
            dom_size++;
            end_index--;
        }

        addr->top_level_domain = (char *) malloc(dom_size + 2);
        addr->sub_domains = (char *)malloc(end_index - start_index + 1);
        if(addr->top_level_domain && addr->sub_domains){
            memcpy(addr->top_level_domain, input + end_index, dom_size+1);
            addr->top_level_domain[dom_size+1] = '\0';
            memcpy(addr->sub_domains, input + start_index, end_index - start_index);
            addr->sub_domains[end_index - start_index] = '\0';
            return 1;
        } else return 0;
        //TODO validation
    } else return 0;
}


address* parse(const char*input){
    if(input!= NULL){
        int size_ = strlen(input);
        if(size_>0){
            address *addr = (address *) malloc(sizeof(struct address));
            addr->protocol = NULL;
            addr->url=NULL;
            addr->sub_domains = NULL;
            addr->top_level_domain = NULL;
            //get protocol
            if (!set_protocol(input, size_, addr)){
				 address_free(addr);
				 //free(addr);
				 return NULL;
			 }
            // get url
            if (!set_url(input, size_, addr)){
				 address_free(addr);
				 //free(addr);
				 return NULL;
			 }
            //get domain
            if (!set_domain(input, size_, addr)){
				 address_free(addr);
				 //free(addr);
				 return NULL;
			 }
            return addr;
        } else return NULL;
    } else return NULL;
}

void address_free(address * addr){
    
    if(addr!=NULL) {
		if(addr->protocol!=NULL) free(addr->protocol);
		addr->protocol = NULL;
		if(addr->url!=NULL) free(addr->url);
		addr->url = NULL;
		if(addr->sub_domains!=NULL) free(addr->sub_domains);
		addr->sub_domains=NULL;
		if(addr->top_level_domain !=NULL) free(addr->top_level_domain);
		addr->top_level_domain=NULL;
		free(addr);
		addr = NULL;
	}
}

void print_info(address * addr){
    if(addr != NULL){
        printf("Protocol: %s\n", addr->protocol);
        printf("Sub-Domain: %s\n", addr->sub_domains);
        printf("Top-Domain: %s\n", addr->top_level_domain);
        printf("Url: %s\n", addr->url);
        //printf("%d\n%d\n%d\n%d\n",strcmp("http",addr->protocol),strcmp("mail",addr->sub_domains),strcmp(".ru",addr->top_level_domain),strcmp("/index.html",addr->url));
    }
}
