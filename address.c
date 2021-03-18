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

//==================PROTOCOL=PARSING===========================================
int protocol_is_valid(const char * protocol){
    if(protocol != NULL){
        if(strcmp(protocol, "http") == 0) return 1;
        if(strcmp(protocol, "https") == 0) return 1;
        if(strcmp(protocol, "ftp") == 0) return 1;
        if(strcmp(protocol, "mailto") == 0) return 1;
        if(strcmp(protocol, "file") == 0) return 1;
        if(strcmp(protocol, "news") == 0) return 1;
        if(strcmp(protocol, "telnet") == 0)return 1;
        return 0;
    } else return 0;
}

char* parse_protocol(const char *input, size_t size_){
    if(input != NULL && size_!= 0){

        size_t pr_size = 0;
        while ((pr_size+2 < size_)  & (input[pr_size] != ':') ) {
            pr_size++;
        }

        if (pr_size > size_-3) return 0;
		if ( input[pr_size + 1] != '/' || input[pr_size + 2] != '/') return 0;

		char * temp = (char *) malloc(pr_size + 1);

		if(temp!=NULL){
            memcpy(temp, input, pr_size);
            temp[pr_size] = '\0';
        }

		return temp;

    } else return NULL;
}

int set_protocol(const char *input,size_t size_, address * addr){
    if(input != NULL && addr != NULL){
        if(addr->protocol != NULL) free(addr->protocol);

        addr->protocol = (char*)malloc(size_+1);
        memcpy(addr->protocol, input, size_);
        addr->protocol[size_] = '\0';
        return 1;
    }else
        return 0;
}

//======================URL==============================================

char * parse_url(const char *input,size_t size_, size_t start_index){
    if(input != NULL && size_ - start_index > 0){

        size_t url_size = size_ - start_index;
        char * temp = (char*) malloc(url_size+1);

        if(temp != NULL){
            memcpy(temp, input+start_index, url_size);
            temp[url_size] = '\0';
        }

        return temp;
    } else return NULL;
}

int set_url(const char *input, size_t size_, address * addr){
    if(input != NULL && addr!= NULL){

        addr->url = (char *) malloc(size_ + 1);

        if(addr->url!=NULL){
            memcpy(addr->url, input, size_);
            addr->url[size_] = '\0';
            return 1;
        } else return 0;

    } else return 0;
}

//======================DOMAIN================================================

char * parse_domain(const char *input,size_t size_, size_t start_index){
    size_t end_index = start_index;

    while (input[end_index] != '/' && end_index != size_ - 1){
        ++end_index;
    }
    size_t dom_size = end_index - start_index;

    char * temp = (char*)malloc(dom_size + 1);
    memcpy(temp, input + start_index, dom_size);
    temp[dom_size] = '\0';

    return temp;
}



int set_domain(const char *input, size_t size_, address * addr){
    if(input != NULL && addr!= NULL){
        int domains_number = 0;

        for (size_t i = 0; i < size_; ++i) {
            if (input[i] == '.') domains_number++;
        }


        if (domains_number == 0 || input[size_] == '.') return 0;

        size_t end_index = size_;
        size_t top_domain_size = 0;
        while (input[end_index] != '.') {
            ++top_domain_size;
            --end_index;
        }

        addr->top_level_domain = (char *) malloc( top_domain_size+ 2);
        if(addr->top_level_domain != NULL){

            memcpy(addr->top_level_domain, input + end_index, top_domain_size+1);
            addr->top_level_domain[top_domain_size+1] = '\0';

        } else return 0;


        addr->sub_domains = (char *)malloc(end_index + 1);
        if(addr->sub_domains != NULL){

            memcpy(addr->sub_domains, input, end_index);
            addr->sub_domains[end_index] = '\0';

            return 1;
        } else return 0;

    } else return 0;
}

//=================PARSER=====================================

address* parse(const char*input){
    if(input!= NULL){

        size_t size_ = strlen(input);
        if(size_>0){

            address *addr = (address *) malloc(sizeof(struct address));

            addr->protocol = NULL;
            addr->url=NULL;
            addr->sub_domains = NULL;
            addr->top_level_domain = NULL;

            //set protocol
            char * temp_protocol = parse_protocol(input,size_);

            if(protocol_is_valid(temp_protocol)){
                size_t pr_size = strlen(temp_protocol);
                set_protocol(temp_protocol, pr_size, addr);
                free(temp_protocol);
            } else{
                address_free(addr);
                return NULL;
            }

            //set domain
            char * temp_domain = parse_domain(input, size_, strlen(addr->protocol)+3);
            if(set_domain(temp_domain, strlen(temp_domain), addr) == 0){
                address_free(addr);
                return NULL;
            }
            free(temp_domain);

            // set url
            size_t url_index = strlen(addr->protocol)+ strlen("://") + strlen(addr->sub_domains) + strlen(addr->top_level_domain) ;
            char * temp_url = parse_url(input,size_,url_index);
            if (set_url(temp_url, strlen(temp_url), addr) != 1){
                address_free(addr);
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
    }
}
