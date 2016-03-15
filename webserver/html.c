#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "html.h"

char * fgets_or_exit(char* buffer,int size,FILE* stream){
  if(fgets(buffer,size,stream)==NULL){
    perror("fprintf");
    exit(NULL);
  }
  return buffer;
}

enum http_method {
  HTTP_GET ,
  HTTP_UNSUPPORTED ,
};
typedef struct
{
  enum http_method method ;
  int major_version ;
  int minor_version ;
  char * url ;
} http_request ;

int parse_http_request (const char* request_line ,http_request* request ){
//request-line= method SP request-target SP HTTP-version CRLF
  return 1;
}

void skip_headers ( FILE* client ) {
   char* buffer = malloc(5000*sizeof(char));
  while(strcmp(fgets_or_exit(buffer,300,client),"\r\n")!=0 || strcmp(fgets_or_exit(buffer,300,client),"\n")!=0){}
}

void send_status (FILE* client ,int code,const char * reason_phrase, int size_message){
//status-line = HTTP-version SP status-code SP reason-phrase CRLF
  char * status-line = "HTTP/1.1 "+code+" "+reason_phrase+"\r\nContent-Length : "+size_message;
   if(fprintf(file,status-line)<0){
          perror("fprintf1");
        }
}

void send_response (FILE* client ,int code,const char* reason_phrase,const char* message_body){
  send_status(client,code,reason_phrase,sizeof(message_body));

}