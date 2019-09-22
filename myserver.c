#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include "parser.h"
#define PORT 8080 

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    while (1) 
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                        (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } else {
            if ( fork()==0) {
                valread = read( new_socket , buffer, 1024); 
                printf("%s\n", buffer ); 
                printf("Length %ld\n", strlen(buffer));
                char * mesg = (char*)malloc(strlen(buffer));
                strcpy(mesg, buffer);

                Request current;
                Response response;
                char *current_response = (char*)malloc(10000);

                parse_request(mesg, &current);
                print_request(&current);
                current_response = create_response(&current, &response);
                send(new_socket , current_response , strlen(current_response) , 0 ); 
                printf("Hello message sent\n"); 
                exit(0);
            } else {
                close(new_socket);
            }
        }
    }
    // valread = read( new_socket , buffer, 1024); 
    // printf("%s\n", buffer ); 
    // printf("Length %ld\n", strlen(buffer));
    // char * mesg = (char*)malloc(strlen(buffer));
    // strcpy(mesg, buffer);

    // Request current;
	// Response response;
    // char *current_response = (char*)malloc(10000);

    // parse_request(mesg, &current);
    // print_request(&current);
	// current_response = create_response(&current, &response);
    // send(new_socket , current_response , strlen(current_response) , 0 ); 
    // printf("Hello message sent\n"); 
    return 0; 
} 