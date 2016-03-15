#ifndef __HTML_H__
#define __HTML_H__

char * fgets_or_exit(char* buffer,int size,FILE* stream);
int parse_http_request (const char* request_line ,http_request* request );
void skip_headers ( FILE* client );
void send_status (FILE* client ,int code,const char * reason_phrase, int size_message);
void send_response (FILE* client ,int code,const char* reason_phrase,const char* message_body);

#endif