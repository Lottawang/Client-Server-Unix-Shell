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
    FILE* fp;

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
                exit(0);
            }
            else
            {
                message[n-1]='\0';

                char *cmd[255];
                int i=0;
                p=strtok(message, " ");

                while(p)
                {
                    cmd[i]=p;
                    i++;
                    p=strtok(NULL, " ");
                }

                int pid1=fork();
                if(pid1== -1)
                {
                    printf( "ERROR:failed to fork\n");
                    exit(1);
                }
                else if (pid1 == 0)
                {
                   
                    dup2(sd,STDOUT_FILENO);

                    if(execvp(cmd[0],cmd)== -1)
                    {
                        
                        printf( "ERROR:exec failed\n" );
                        exit(1);
                    }
                }
                else
                {   
                    int myStatus;
                    waitpid(pid1, &myStatus, 0);
                    dup2(backup,STDOUT_FILENO);          
                }
            }       
        }

}

