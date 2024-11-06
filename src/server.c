#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //init the server and clinet sockets
    int server_socket, client_socket;

    //create the server socket
    //server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //bind the server socket to the port
    //bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    //grab incoming connections
    listen(server_socket, 5);

    //loop waiting for connections
    while(1) {
        //accept the connection
        client_socket = accept(server_socket, NULL, NULL);

        //fork a child process to handle the connection
        if(fork() == 0) {
            //execute the given shell command and return it to stdout and stderr
            //dup2(client_socket, 1);
            //dup2(client_socket, 2);
            //execve("/bin/sh", NULL, NULL);
        }

        //close the connection
        close(client_socket);
    }

    return 0;
}
