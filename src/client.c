#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;

  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(1298);

  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

  // This will let the client keep looping and asking for commands until quit is given.
  int commandLoop = 1;
  while(commandLoop){
    //grab commands to be sent to the server
    printf("Enter a command: ");
    fgets(buffer, 1024, stdin);

    if(strcmp("quit", buffer) == 0){
      commandLoop = 0;
      printf("Closing socket connection...\n ");

    } else {
      //send the command to the server
    send(clientSocket, buffer, 1024, 0);

    /*---- Read the message from the server into the buffer ----*/
    recv(clientSocket, buffer, 1024, 0);

    /*---- Print the received message ----*/
    printf("Data received: %s",buffer);  
    }
  }
  
  return 0;
}
