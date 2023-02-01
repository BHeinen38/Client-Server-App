//Author Bailey Heinen, Kyle Strozinsky


#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>

main() {

 short DSS[8];
 char message[1024] = "";

 int socketControl; //this var will be used to control our socket
 int connControl; //this var will be used to control our connection
 
 int socket1Descript; 
 int conn1Descript;
 int conn1Length;

 int socket2Descript;
 int conn2Descript; 
 int conn2Length;

 int socket3Descript;
 int conn3Descript; 
 int conn3Length;

 struct sockaddr_in sAddr; //this will be used as our server address
 struct sockaddr_in cAddr; //this will be used as our client address

 struct sockaddr_in server1;
 struct sockaddr_in client1;

 struct sockaddr_in server2;
 struct sockaddr_in client2;

 struct sockaddr_in server3;
 struct sockaddr_in client3;


 int pipestatus;
 char pipe_read[10];

 char* clientIP = "127.0.0.1"; //this is our client IP
 int clientLen; //this will hold the client address length

 // Buffer for sending / receiving messages
 char byteBuffer1[4];
 char byteBuffer2[4];
 char byteBuffer3[4];

 char messageBuf1[4];
 char messageBuf2[4];
 char messageBuf3[4];

 int firstFD[2];
 int secondFD[2];
 int thirdFD[2];

 // Setup pipe
 pipestatus = pipe(firstFD);
 pipe(secondFD);
 pipe(thirdFD);


 if (pipestatus == -1) {
   printf("Pipe was not able to be created \n");
   exit(0);
 }
 
	fcntl(firstFD[0], F_SETFL, O_NONBLOCK); //set the correct flags on each file descriptor
	
	fcntl(secondFD[0], F_SETFL, O_NONBLOCK);
	
	fcntl(thirdFD[0], F_SETFL, O_NONBLOCK);




// Create control socket
socketControl = socket(AF_INET, SOCK_STREAM, 0); //create our socket control and use the correct descriptor
if(socketControl == -1) {
  
  printf("There was an error creating the control socket \n"); //this will only fail if the socket connection failed 
  exit(0); 

}

sAddr.sin_family = AF_INET; //set the familt as AF_INET
sAddr.sin_addr.s_addr = inet_addr(clientIP); //set the server address to the client
sAddr.sin_port = htons(3000); //set the port to 3000

// Bind the socket
if((bind(socketControl, (struct sockaddr *)&sAddr, sizeof(sAddr))) < 0) {
  
  printf("Control socket was not binded %d\n", errno); //if the bind is less than 0 than we will know that the bind failed
  exit(0);

}

if (listen(socketControl, 5) != 0) { //listen to the socket
  
  printf("Control is not listening \n"); //this will only print if the socket fails
  exit(0);

}

clientLen = sizeof(cAddr); //using the length var to hold the length of the client

printf("Waiting to the control connection \n"); //just show the user that the server is waiting

connControl = accept(socketControl, (struct sockaddr *)&cAddr, &clientLen); //accept the control connection

if (connControl < 0 ) {
  
  printf("was not able to accept the control socket \n"); //if connControl is < 0 then we know that it failed and need to notify the user
  exit(0);

}

// Create TCP socket 1
socket1Descript = socket(AF_INET, SOCK_STREAM, 0);
if(socket1Descript == -1) {
  
  printf("Socket 1 was not created \n"); // Socket creation failed if it comes out to -1
  exit(0);

}
usleep(5000);

server1.sin_family = AF_INET;
server1.sin_addr.s_addr = inet_addr(clientIP); // Set address to the client
server1.sin_port = htons(3100); // set port to 31000


if (connect(socket1Descript, (struct sockaddr*)&server1, sizeof(server1)) != 0) { //creation of tcp_socket_1
  
  printf("Socket 1 connection failed\n"); // if connect does not return 0 then the process failed
  exit(0);

}


socket2Descript = socket(AF_INET, SOCK_STREAM, 0); //creation 
if(socket2Descript == -1) {
  
  printf("Socket 2 was not created \n"); // if -1 is returned socket creation failed
  exit(0);

}
usleep(5000);

server2.sin_family = AF_INET;
server2.sin_addr.s_addr = inet_addr(clientIP); // Set address to client
server2.sin_port = htons(3200); // Set port to 3200

// Connect to client TCP 2
if (connect(socket2Descript, (struct sockaddr*)&server2, sizeof(server2)) != 0) {
  
  printf("Socket 2 connection failed\n"); // if connect returns 0 then the connection failed
  exit(0);

}

// Create TCP socket 3
socket3Descript = socket(AF_INET, SOCK_STREAM, 0);
if(socket3Descript == -1) {
  
  printf("Socket 3 was not created \n"); // if -1 then the socket creation failed 
  exit(0);

}
usleep(5000);

// Assign IP and port for server
server3.sin_family = AF_INET;
server3.sin_addr.s_addr = inet_addr(clientIP); // set address to client 
server3.sin_port = htons(3300); // set port to 3300

// Connect to client TCP 3
if (connect(socket3Descript, (struct sockaddr*)&server3, sizeof(server3)) != 0) {
 
  printf("Socket 3 connection failed\n"); // if 0 is returned connection failed 
  exit(0);

}

int dataFlow = 0;
int i;

pid_t pids[3]; //keep track of our processes

while(dataFlow < 3) { //only work with four bytes at a time
  
  pids[dataFlow] = fork();
  
  if(pids[dataFlow] < 0) { //check to see the status of the fork 
    
    perror("fork failed"); //if the fork fails you will see this error
    exit(1);
  }
  if(pids[dataFlow] == 0) { //this will be child process for our data 
    
    switch(dataFlow){ //switch based off the data information

      case 0:

        while(i = read(socket1Descript, messageBuf1, 4)){ //read data in the first message buffer
         
          write(firstFD[1], messageBuf1, sizeof(messageBuf1)); //write data to the buffer
          printf("data dataFlow 1: %s\n", messageBuf1); //print this so that the user can see that you wrote to the buffer
        }
        exit(0);

      case 1:

        while(i = read(socket2Descript, messageBuf2, 4)){ //read the data in buffer 2
          
          write(secondFD[1], messageBuf2, sizeof(messageBuf2)); //write the data into the new buffer
          printf("data dataFlow 2: %s\n", messageBuf2); //show the user that you got the data
        }
        exit(0);

      case 2:

        while(i = read(socket3Descript, messageBuf3, 4)){ //read the data in buffer two
          
          write(thirdFD[1], messageBuf3, sizeof(messageBuf3)); //write the data
          printf("data dataFlow 3: %s\n", messageBuf3); //show the user that you got this data
        }
        exit(0);
    }
  } else { //this is the partent process of the fork
    //Do nothing and pass to the child
  }
  dataFlow++; //increment
}

sleep(1);


FILE *file = fopen("DSSLog.txt", "w"); //we are going to be writing our results to this file

while(i = read(connControl, DSS, sizeof(DSS))) { //while we are reading from our connection continue
  int i = 0;

  read(firstFD[0], byteBuffer1, 4);  //read the first file descriptor
  byteBuffer1[4] = '\0'; //set the last byte as null

  strcat(message, byteBuffer1); //concat the message

  printf("PIPE 1: %s\n", byteBuffer1); //print pipe one information 
  
  while(i < 8) { 
    fprintf(file, "%d, ", DSS[i]); //populate the file
    i++;
  }
  fprintf(file, "\n"); //print a new line when we are finished populating

  bzero(DSS, sizeof(DSS));
  read(connControl, DSS, sizeof(DSS)); //read the information 
  read(secondFD[0], byteBuffer3, 4); //read the second file descriptor


  byteBuffer3[4] = '\0'; //set the last byte as null
  strcat(message, byteBuffer3); //concat the message
  
  printf("PIPE 2: %s\n", byteBuffer3); //print the second pipe information
  i = 0;

  while(i < 8) {

    fprintf(file, "%d, ", DSS[i]); //populate the file with pipe 2 information 
    i++;
  }

  fprintf(file, "\n"); //print a new line when pipe 2 finishes 

  bzero(DSS, sizeof(DSS));
  read(connControl, DSS, sizeof(DSS)); //read the current pipe info 
  read(thirdFD[0], byteBuffer2, 4); //read the pipe three info 

  byteBuffer2[4] = '\0'; //set the last byte as null 
  strcat(message, byteBuffer2); //concat the message information 
  
  printf("PIPE 3: %s\n", byteBuffer2); //tell the user that we are working on pipe 3
  
  i = 0;

  while(i < 8) {
    
    fprintf(file, "%d, ", DSS[i]); //populate the file with pipe 3
    i++;

  }
  fprintf(file, "\n"); //add a new line to the file

  usleep(5000);
  
  bzero(DSS, sizeof(DSS)); //zero everything 
  bzero(byteBuffer1, sizeof(byteBuffer1));
  bzero(byteBuffer3, sizeof(byteBuffer3));
  bzero(byteBuffer2, sizeof(byteBuffer2));
}
fclose(file); //close the file 

printf("Message %s\n", message); //finally we are able to print the message that we are sending between client and server


close(socketControl); //close all of the connections
close(socket1Descript);
close(socket2Descript);
close(socket3Descript);
close(firstFD[1]);
close(secondFD[1]);
close(thirdFD[1]);
close(pipestatus);
fflush(stdout);

}