#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define Request Request
#define Response Response
#define parse_request parse_request
#define print_request print_request
#define LINE_MAX_LENGTH 80
#define DOCUMENT_MAX_LENGTH 65536

typedef struct Request {
        char *method;
        char *uri; 
        char *protocol;
        char *content_type;
        char *cache;
        char *token;
        char *user_agent;
        char *accept;
        char *host;
        char *encoding;
        char *content_length;
        char *connection;
        char *body;

    } Request;

typedef struct Response {
        char *status;
        char *content_type;
        char *content_length;
        char *body;

    } Response;

extern void parse_request(char *mesg, Request *parsed_mesg);
extern void print_request(Request *current);
extern char * create_response(Request *current, Response *response);
