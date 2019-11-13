//client fifo

#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define PERMS 0666
char fname[256];
int main()
{
ssize_t n;
char buff[512];
int readfd,writefd;
printf("Trying to Connect to Server..\n");
writefd = open(FIFO1, O_WRONLY, 0); 	
readfd = open(FIFO2, O_RDONLY, 0);
printf("Connected..\n");
printf("Enter the filename to request from server: ");
scanf("%s",fname);
write(writefd, fname, strlen(fname));
printf("Waiting for Server to reply..\n");
while((n=read(readfd,buff,512))>0)
write(1,buff,n);
close(readfd);
close(writefd);
return 0;
}

//server fifo
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define PERMS 0666
char fname[256];

int main( ) {
int readfd, writefd, fd;
ssize_t n;
char buff[512];
if (mkfifo(FIFO1, PERMS)<0)
printf("Cant Create FIFO Files\n");
if (mkfifo(FIFO2, PERMS)<0)
printf("Cant Create FIFO Files\n");
printf("Waiting for connection Request..\n");
readfd =open(FIFO1, O_RDONLY, 0);
writefd=open(FIFO2, O_WRONLY, 0);
printf("Connection Established..\n");
read(readfd, fname, 255);
printf("Client has requested file %s\n",fname);
if ((fd=open(fname,O_RDWR))<0) {
strcpy(buff,"File does not exist..\n");
write(writefd, buff, strlen(buff));
} else {
while((n=read(fd, buff,512))>0)
write(writefd, buff, n);
}
close(readfd); unlink(FIFO1);
close(writefd); unlink(FIFO2);
}
