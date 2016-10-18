//COMMENTS: system project (section 2)
//DATE:2016-04-06
//NAME: Danfeng Wang, ID: 104489084;

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

void child(int);

int main(int argc, char *argv[]) 
{
    int sd,pid, client, portNumber, status;
    socklen_t len;
    struct sockaddr_in servAdd;

    if (argc != 2) 
    {
        printf("Call model: %s <Port Number>\n", argv[0]);
        exit(0);
    }
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        fprintf(stderr, "Cannot create socket\n");
        exit(1);
    }
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    sscanf(argv[1], "%d", &portNumber);
    servAdd.sin_port = htons((uint16_t) portNumber);
    bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
    listen(sd, 5);
    while(1)
    {   
        client = accept(sd,(struct sockaddr *)NULL, NULL);
        printf("Got a client\n");
        if((pid=fork())>0)
        {
            close(client);
            waitpid(0,&status,WNOHANG);
        }
        else
        {
            child(client);
            close(client);
        }

    }
}
void child(int sd)
{   
    char message[1000000];
    int n;
    char *p;
    int  backup;
 

        while(1)
        {   
            n=read(sd, message,255);
            if(n==-1)
            {
            
                backup=dup(STDOUT_FILENO);
                dup2(sd,STDOUT_FILENO);
                printf("ERROR: Read the client message failed.\n");
                dup2(backup,STDOUT_FILENO);
                exit(0);
            }
            else if(n==0)
            {
              
                printf("Client has quit, now the server child will quit.\n");
                printf("Waiting for a new client...\n");
                exit(0);
            }
            else
            {
                message[n-1]='\0';
                backup=dup(STDOUT_FILENO);
                dup2(sd,STDOUT_FILENO);
                system(message);
                memset( message,0,sizeof(message));
                dup2(backup,STDOUT_FILENO);
            }       
        }

}

