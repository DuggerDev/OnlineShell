#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int welcomeSocket, newSocket, valread;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;

    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(1298);

    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*---- Bind the address struct to the socket ----*/
    bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if (listen(welcomeSocket, 5) == 0)
        printf("Listening\n");
    else
        printf("Error\n");

    //if the buffer contains quit then close the socket
    if(strcmp("quit", buffer) == 0){
        close(welcomeSocket);
        return 0;
    }

    /*---- Accept call creates a new socket for the incoming connection ----*/
    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage,
                       &addr_size);

    int loopBreakout = 0;
    while(1){
        pid_t pid;
        pid = fork();

        loopBreakout++;

        //if fork() returns 0, then it is the child process
        if(pid == 0){
            //close the welcome socket
            close(welcomeSocket);
            //read the args from the client and execvp
            valread = read(newSocket, buffer, 1024);

            //break the buffer into tokens/args[] where there is a space and then add a null terminator
            //didnt know i needed a null terminator so this helped: https://stackoverflow.com/questions/6274166/execvp-arguments
            //also got help from dr altarazi on this
            char *args[1024];
            char *token = strtok(buffer, " ");
            int i = 0;
            while(token != NULL){
                args[i] = token;
                i++;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            //printf("Data received: %s\n", buffer);
            //print args 0 for debug
            //printf("args[0]: %s\n", args[0]);
            //print args for debug
            //printf("args: %s\n", args);

            //block the execvp output from going to stdout and stderr
            //found an exeample here: https://stackoverflow.com/questions/1720535/practical-examples-use-dup-or-dup2
            dup2(newSocket, STDOUT_FILENO);
            dup2(newSocket, STDERR_FILENO); //apparently both of these have to be done before you use execvp

            //use execvp to run the arguments
            execvp(args[0], args);
            
            //send execvp output to the client, apparently just closing the socket does this we dont have to send anything
            close(newSocket);
            return 0;
        }
        if(loopBreakout == 5){
            break;
        }
    }

    return 0;
}
