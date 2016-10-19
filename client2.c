//NAME: DANFENG WANG
//DATE: 2016-03-28


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    char message[1000000],ch;
    int sd, n, portNumber,l,pid;
    struct sockaddr_in servAdd;

  if(argc != 3)
  {
      printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
      exit(0);
  }
  if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
      fprintf(stderr, "Cannot create socket\n");
      exit(1);
  }
    servAdd.sin_family = AF_INET;
    sscanf(argv[2], "%d", &portNumber);
    servAdd.sin_port = htons((uint16_t)portNumber);

    if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr)<0)
    {
      fprintf(stderr, "inet_pton() has failed\n");
      exit(2);
    }

    if(connect(sd,(struct sockaddr *)&servAdd, sizeof(servAdd))<0)
    {
      fprintf(stderr, "connect() has failed,exiting\n");
      exit(3);
    }
        while(1)
      { 
        memset(message,0,sizeof(message));
        fprintf(stderr, "\nPlease Enter your command(Press CTR-D to quit)>> ");
        if((n=read(0,message,255))<0)
        {
          close(sd);
          printf("Read the input command failed.\n");
        }
        else if (n==0)
        {
             printf("The client will quit.\n");
             close(sd);
             //exit(0);
             kill(getpid(),SIGQUIT);
        }
        else if(n>0)
        {
          printf("reveiving message: %s\n", message);
          write(sd,message,n);
        }

        memset(message, 0, sizeof(message));
        int n1=read (sd,message,1000000);
        printf("received response from server:\n%s", message);
      }
      close(sd);
}
