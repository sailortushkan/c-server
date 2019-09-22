#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"

void parse_request(char *mesg, Request *parsed_mesg) {
    if (strchr(mesg,'{')) {
        char * pch;
        pch = strchr(mesg,'{');
        parsed_mesg->body = (char*)malloc(strlen(pch));
        strcpy(parsed_mesg->body, pch);
    }   
    char * temp;
    temp = strtok (mesg, " \n\r");
    while (temp != NULL) {
        for(int i = 0; i <= strlen(temp) - 1; i++){
            temp[i] = tolower(temp[i]);
        }            
        if (strcmp(temp, "post") == 0) {
        parsed_mesg->method = temp;
        parsed_mesg->uri = strtok (NULL, " \n\r");
        parsed_mesg->protocol = strtok (NULL, " \n\r");
        }
        if (strcmp(temp, "get") == 0) {
        parsed_mesg->method = temp;
        parsed_mesg->uri = strtok (NULL, " \n\r");
        parsed_mesg->protocol = strtok (NULL, " \n\r");
        }        
        if (strcmp(temp, "content-type:") == 0) {
        parsed_mesg->content_type = strtok (NULL, " \n\r");
        }
        if (strcmp(temp, "content-length:") == 0) {
        parsed_mesg->content_length = strtok (NULL, " \n\r");
        }
        if (strcmp(temp, "connection:") == 0) {
        parsed_mesg->connection = strtok (NULL, " \n\r");
        }      
        temp = strtok(NULL, " \n\r" );
        if (temp == NULL)
            break;
    }   
}    

void print_request(Request *current) {
    if (strcmp(current->method,"get") == 0) {
        puts(current->method);
        puts(current->uri);
        puts(current->protocol);
        puts(current->connection);
    } else if (strcmp(current->method,"post") == 0) {
        puts(current->method);
        puts(current->uri);
        puts(current->protocol);
        puts(current->content_length);
        puts(current->connection);
        puts(current->body);
    }
} 

char *create_response(Request *request, Response *response) {   
    FILE * incoming;
    char buf[LINE_MAX_LENGTH];
    char doc[DOCUMENT_MAX_LENGTH];
    
    if (strcmp(request->method, "post") == 0) {
        if (strcmp(request->uri, "/echo") == 0 && (strcmp(request->content_type, "application/json") == 0)) {
            response->status = "HTTP/1.1 200 OK";
            response->content_type = request->content_type;
            response->content_length = request->content_length;
            response->body = request->body;
        } else {
            response->status = "HTTP/1.1 404 Not Found";
            response->content_type = "text/html";
            incoming = fopen("public/404.html", "r");
            while (fgets(buf, LINE_MAX_LENGTH, incoming) != NULL) 
                strcat(doc, buf);
            fclose(incoming);   
            strcat(response->body, doc);            
        }          
     }  
    
    if (strcmp(request->method, "get") == 0) {
            char file_address[1024] = "public";
            strcat(file_address, request->uri);
            incoming = fopen(file_address, "r");
            if (incoming) {
                while (fgets(buf, LINE_MAX_LENGTH, incoming) != NULL) 
                    strcat(doc, buf);
                fclose(incoming);  
                response->body = (char*)malloc(strlen(doc));
                strcpy(response->body, doc);
                response->status = "HTTP/1.1 200 OK";
                response->content_type = "text/html";
        } else {
            response->status = "HTTP/1.1 404 Not Found";
            response->content_type = "text/html";
            incoming = fopen("public/404.html", "r");
            while (fgets(buf, LINE_MAX_LENGTH, incoming) != NULL) 
                strcat(doc, buf);
            fclose(incoming);   
            response->body = (char*)malloc(strlen(doc));
            strcpy(response->body, doc);
        }                  
     }    

    int length_int = strlen(response->body);
    char length_str[1024];
    sprintf(length_str, "%d", length_int);
    printf("\n\n%s\n\n",length_str);
    response->content_length = length_str;
        
    char str_response[1024] = {};
    char *result = (char*)malloc(10000);
    result = str_response;
    strcat (str_response, response->status);
    strcat (str_response, "\n");
    strcat (str_response, "content-type: ");
    strcat (str_response, response->content_type);
    strcat (str_response, "\n");
    strcat (str_response, "content-length: ");
    strcat (str_response, response->content_length);
    strcat (str_response, "\n\n");
    strcat (str_response, response->body);
    return result;
}
// int main()
// {
//     FILE * incoming;
//     char buf[LINE_MAX_LENGTH];
//     char doc[DOCUMENT_MAX_LENGTH];

//     incoming = fopen("postexample", "r");
//     while (fgets(buf, LINE_MAX_LENGTH, incoming) != NULL) 
//         strcat(doc, buf);
//     fclose(incoming);   
 
//     return 0;
// }