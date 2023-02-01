//Author Bailey Heinen, Kyle Strozinsky

#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <pthread.h>

void process1(int Conn1, int Sock1, int ConnSocket); //helper method for the first process 
void process2(int Conn2, int Sock2, int ConnSocket); //helper method for the second process 
void process3(int Conn3, int Sock3, int ConnSocket); //helper method for the third process

 int pipeFileWriter[2]; //pipe communication that will be written to the file
 int pipeMPTCP[2]; //MPTCP tracker
 int seqNumber = 0; //this is a global counter to count the number of sequences that we are on 
 int pipeData[3][2]; //this is going to be used for data communication

//helper method for file writing
void process1(int Conn1, int Sock1, int ConnSocket) {

 short messageBuffer[8];
 int seqNumber_1 = 0;
 char loacalPipe[10];

 while(1) {

	read(pipeData[0][0], &seqNumber, sizeof(seqNumber)); //read the data sequence number

   read(pipeData[0][0], loacalPipe, sizeof(loacalPipe)); //read 4 bytes from the local pipe 
   printf("Thread 1 recieved: %s\n", loacalPipe); //show the user what is happening in real time

   bzero(messageBuffer, sizeof(messageBuffer));

   messageBuffer[0] = seqNumber; //get the earlier sequence number
   messageBuffer[1] = seqNumber_1; //get the current sequence number 
   seqNumber++; //increament both of the sequence number because we have used them 
   seqNumber_1++; 
   
   messageBuffer[2] = seqNumber; //get the next seequence number
   messageBuffer[3] = seqNumber_1;
   seqNumber++;
   seqNumber_1++;
   
   messageBuffer[4] = seqNumber; //and the next
   messageBuffer[5] = seqNumber_1;
   seqNumber++;
   seqNumber_1++;
   
   messageBuffer[6] = seqNumber; //all the way to 4
   messageBuffer[7] = seqNumber_1;
   seqNumber++;
   seqNumber_1++;
   
      
    int i = 0;
    char fileWriter[50] = "";
    char convert[10] = "";

    while(i < 8) {
    
    bzero(convert, sizeof(convert)); //turn the number into a string format
    sprintf(convert, "%d, ", messageBuffer[i]);
    strcat(fileWriter, convert); //concat all of the required information
    i++;
   }
	  
   write(ConnSocket, messageBuffer, sizeof(messageBuffer)); //write to the control socket
   write(Conn1, loacalPipe, 4); //we can only send 4 bytes at a time
   write(pipeMPTCP[1], &seqNumber, sizeof(seqNumber)); //update the sequence number
   write(pipeFileWriter[1], fileWriter, sizeof(fileWriter)); //update the pipe writer
   
   fflush(stdout);
 }
}
//helper method for file writing
void process2(int Conn2, int Sock2, int ConnSocket) {

 short messageBuffer[8];
 int seqNumber_2 = 0;
 char localPipe[10];

 while(1) {

   read(pipeData[1][0], &seqNumber, sizeof(seqNumber));//read the data sequence number
   read(pipeData[1][0], localPipe, sizeof(localPipe));//read 4 bytes from the local pipe
   printf("Thread 2 recieved: %s\n", localPipe); //show the user what is going in real time
   
   bzero(messageBuffer, sizeof(messageBuffer));

   messageBuffer[0] = seqNumber;//get the earlier sequence number
   messageBuffer[1] = seqNumber_2;//get the current sequence number 
   seqNumber++;//increament both of the sequence number because we have used them
   seqNumber_2++;
   
   messageBuffer[2] = seqNumber;//get the next seequence number
   messageBuffer[3] = seqNumber_2;
   seqNumber++;
   seqNumber_2++;
   
   messageBuffer[4] = seqNumber;//and the next
   messageBuffer[5] = seqNumber_2;
   seqNumber++;
   seqNumber_2++;
   
   messageBuffer[6] = seqNumber;//all the way to 4
   messageBuffer[7] = seqNumber_2;
   seqNumber++;
   seqNumber_2++;
   
   
   int i = 0;
   char convert[10] = "";
   char fileWriter[50] = "";

   while(i < 8) {

    bzero(convert, sizeof(convert));//turn the number into a string format
    sprintf(convert, "%d, ", messageBuffer[i]);
    strcat(fileWriter, convert); //concat all of the required information
    i++;
   }
	
   write(ConnSocket, messageBuffer, sizeof(messageBuffer));//write to the control socket
   write(Conn2, localPipe, 4);//we can only send 4 bytes at a time
   write(pipeMPTCP[1], &seqNumber, sizeof(seqNumber));//update the sequence number
   write(pipeFileWriter[1], fileWriter, sizeof(fileWriter));//update the pipe writer
   
   fflush(stdout);
 }

}
//helper method for file writing
void process3(int Conn3, int Sock3, int ConnSocket) {

    short massageBuffer[8];
    int seqNumber_3 = 0;
    char localPipe[10];
	
 while(1) {
	 
	 read(pipeData[2][0], &seqNumber, sizeof(seqNumber));//read the data sequence number
   read(pipeData[2][0], localPipe, sizeof(localPipe));//read 4 bytes from the local pipe 
   printf("Thread 3 recieved: %s\n", localPipe); //show the user what is happening at real time 
   
   bzero(massageBuffer, sizeof(massageBuffer));

   massageBuffer[0] = seqNumber;//get the earlier sequence number
   massageBuffer[1] = seqNumber_3;//get the current sequence number 
   seqNumber++;//increament both of the sequence number because we have used them
   seqNumber_3++;
   
   massageBuffer[2] = seqNumber;//get the next seequence number
   massageBuffer[3] = seqNumber_3;
   seqNumber++;
   seqNumber_3++;
   
   massageBuffer[4] = seqNumber;//and the next
   massageBuffer[5] = seqNumber_3;
   seqNumber++;
   seqNumber_3++;
   
   massageBuffer[6] = seqNumber;//all the way to 4
   massageBuffer[7] = seqNumber_3;
   seqNumber++;
   seqNumber_3++;
   
   int i = 0;
   char fileWriter[50] = "";
   char convert[10] = "";

   while(i < 8) {

    bzero(convert, sizeof(convert));//turn the number into a string format
    sprintf(convert, "%d, ", massageBuffer[i]);
    strcat(fileWriter, convert);//concat all of the required information
    i++;
   }

   write(ConnSocket, massageBuffer, sizeof(massageBuffer));//write to the control socket
   write(Conn3, localPipe, 4);//we can only send 4 bytes at a time
   write(pipeMPTCP[1], &seqNumber, sizeof(seqNumber));//update the sequence number
   write(pipeFileWriter[1], fileWriter, sizeof(fileWriter));//update the pipe writer
   
   fflush(stdout);
 }
}

main() {
	
 char fullMessageBuffer[992] = ""; //this is going to hold our whole message that we are going to send to server
 //we have 992 because that is the size of bytes that we are sending

 char writeToPipe1[10]; //this is our data communication buffer
 char writeToPipe2[10];
 char writeToPipe3[10];
 char mainPipeWriter[50] = "";
 

 FILE *file; //this will be our file for storing data

 int socketControl;//this var will be used to control our socket
 int connControl;//this var will be used to control our connection

 int socket1Descript; //this will be used for our socket descriptors
 int conn1Descript;
 int conn1Length;

 int socket2Descript;
 int conn2Descript;
 int conn2Length;

 int socket3Descript;
 int conn3Descript;
 int conn3Length; //this is the end of our socket descriptors

 // Server and client addresses
 struct sockaddr_in sAddr;
 struct sockaddr_in cAddr;

 struct sockaddr_in server1;
 struct sockaddr_in client1;

 struct sockaddr_in server2;
 struct sockaddr_in client2;

 struct sockaddr_in server3;
 struct sockaddr_in client3;
 
 // Track PID for each process so we can kill it at the end
 int process1PID;
 int process2PID;
 int process3PID;

 // Buffer for sending / receiving
 char buffer[1024];


 char digits[10] = "0123456789"; //all of the required numbers
 char lowercase[26] = "abcdefghijklmnopqrstuvwxyz"; //all of the lower case 
 char uppercase[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // all of the upper case 
 
 // Create the file
 file = fopen("clientSideLog.txt", "w+");

 
 int k = 0;

 while(k < 3) {

   pipe(pipeData[k]); //pipe set up
   k++;

 }
 
 pipe(pipeMPTCP); //pipe set up
 pipe(pipeFileWriter);

socketControl = socket(AF_INET, SOCK_STREAM, 0); //create the socket for the control

if(socketControl == -1) { //if the socket descriptor == -1 then we would that it failed
  
  printf("Control socket creation error\n"); //need to print an error the the user
  exit(0);
  
}

bzero(&sAddr, sizeof(sAddr));


sAddr.sin_family = AF_INET; //set the IP family to AF_NET
sAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //set the correct IP address
sAddr.sin_port = htons(3000); //set the port to 3000

if (connect(socketControl, (struct sockaddr*)&sAddr, sizeof(sAddr)) != 0) {  //try and connect the socket control
  
  printf("Control socket failed\n"); //print if the connection fails
  exit(0);

}



socket1Descript = socket(AF_INET, SOCK_STREAM, 0); //create the socket 1

if(socket1Descript == -1) { 
  
  printf("Socket 1 error\n"); //print if the socket creation failed
  exit(0);

}


server1.sin_family = AF_INET; //set the family to af_net
server1.sin_addr.s_addr = inet_addr("127.0.0.1"); //set the correct IP 
server1.sin_port = htons(3100); //set the port to 3100 

// Bind the socket
if((bind(socket1Descript, (struct sockaddr *)&server1, sizeof(server1))) < 0) { //try and bind socket 1
  
  printf("Socket 1 bind error\n"); //print if the bind failed
  exit(0);

}

if (listen(socket1Descript, 5) != 0) { //listen to socket 1
  
  printf("Socket 1 listen error\n"); //print if we are unable to listen
  exit(0);

}

conn1Length = sizeof(client1); //get the length of the client

printf("Waiting to accept connection 1\n");


conn1Descript = accept(socket1Descript, (struct sockaddr *)&client1, &conn1Length); //accept connection 1
if (conn1Descript < 0 ) {
  
  printf("Connection 1 was not accepted \n"); //print if we are not able to accept the connection
  exit(0);

}

 if((process1PID = fork()) == 0) {
   
   process1(conn1Descript, socket1Descript, socketControl); //get the child process
 }
 
 
socket2Descript = socket(AF_INET, SOCK_STREAM, 0); //create socket 2

if(socket2Descript == -1) { 
  
  printf("Socket 2  error\n"); //show this error if the creation failed
  exit(0);

}

server2.sin_family = AF_INET; //set the IP family
server2.sin_addr.s_addr = inet_addr("127.0.0.1"); //set the correct IP address 
server2.sin_port = htons(3200); //set the port to 3200


if((bind(socket2Descript, (struct sockaddr *)&server2, sizeof(server2))) < 0) { //we need to bind the second socket
  
  printf("Socket 2 bind error\n"); //print if the bind fails 
  exit(0);

  }


if (listen(socket2Descript, 5) != 0) { //listen to socket 2
  
  printf("Socket 2 not listening \n"); //print if the socket is not able to listen
  exit(0);

}

conn2Length = sizeof(client2); //get the client length 

printf("Waiting to accept connection 2\n");


conn2Descript = accept(socket2Descript, (struct sockaddr *)&client2, &conn2Length); //accept the second connection
if (conn2Descript < 0 ) {
  
  printf("Connection 2 was not accepted \n");
  exit(0);

}

 
 if((process2PID = fork()) == 0) { //fork the second connection
   process2(conn2Descript, socket2Descript, socketControl);
 }
 

socket3Descript = socket(AF_INET, SOCK_STREAM, 0); //create the third process
if(socket3Descript == -1) {
  
  printf("Third socket was not able to be created \n"); //print if we could not create the socket 
  exit(0);

}

server3.sin_family = AF_INET; //assign the family
server3.sin_addr.s_addr = inet_addr("127.0.0.1"); //set the correct IP address
server3.sin_port = htons(3300); //set the port to 3300


if((bind(socket3Descript, (struct sockaddr *)&server3, sizeof(server3))) < 0) { //try and bind the socket
  
  printf("Socket 3 bind failed\n"); //print if the bind failed
  exit(0);
  
}

if (listen(socket3Descript, 5) != 0) { //listen for the third connection
  printf("Socket 3 is not listening \n"); //print if the socket is not able to listen 
  exit(0);
}

conn3Length = sizeof(client3); //get the length of the third client

printf("Waiting to accept connection 3\n"); //tell the user that we are waiting on the client


conn3Descript = accept(socket3Descript, (struct sockaddr *)&client3, &conn3Length); //accept the third connection

if (conn3Descript < 0 ) {
  
  printf("Was not able to accept the 3 connection \n"); //tell the user that we did not accept the connection
  exit(0);

}


 if((process3PID = fork()) == 0) { //fork the last connection
   
   process3(conn3Descript, socket3Descript, socketControl);

 }

 
bzero(buffer, 1024);

 int i = 0;

 while(i < 16) { //populate our message

   strcat(fullMessageBuffer, digits); 
   strcat(fullMessageBuffer, lowercase);
   strcat(fullMessageBuffer, uppercase);
   i++;

}

write(pipeMPTCP[1], &seqNumber, sizeof(seqNumber)); //this is fixing a bug where were are not writing our pipes at the start 
write(pipeFileWriter[1], mainPipeWriter, sizeof(mainPipeWriter)); 

i = 0;

 while( i < 992) {
	 
   read(pipeMPTCP[0], &seqNumber, sizeof(seqNumber)); //read from thread 
   write(pipeData[0][1], &seqNumber, sizeof(seqNumber)); //thread 1
   
   read(pipeFileWriter[0], mainPipeWriter, sizeof(mainPipeWriter)); //this is for our writing pipe 
   
   fprintf(file, mainPipeWriter); //print the info that we are giving to the file 
   fprintf(file, "\n"); //add a new line
   
   if(i + 3 < 992) { //special condition when we are reaching the end of our bytes 
  //set them manually
   writeToPipe1[0] = fullMessageBuffer[i]; 
   writeToPipe1[1] = fullMessageBuffer[i+1];
   writeToPipe1[2] = fullMessageBuffer[i+2];
   writeToPipe1[3] = fullMessageBuffer[i+3];

   write(pipeData[0][1], writeToPipe1, sizeof(writeToPipe1)); //write to the pipe
   usleep(2500);

   }
   
	  // Read the counter update from thread 1
      read(pipeMPTCP[0], &seqNumber, sizeof(seqNumber));
	  
	  // Write the counter update to thread 2
	  write(pipeData[1][1], &seqNumber, sizeof(seqNumber));
	  
	  // Read the sequence numbers we need to write to the file
	  read(pipeFileWriter[0], mainPipeWriter, sizeof(mainPipeWriter));
	  
	  // Write to the log file
      fprintf(file, mainPipeWriter);
	  fprintf(file, "\n");
	  

   if(i + 7 < 992) { // Specific condidtion near the end of the message
    // set manually
    writeToPipe2[0] = fullMessageBuffer[i+4];
    writeToPipe2[1] = fullMessageBuffer[i+5];
    writeToPipe2[2] = fullMessageBuffer[i+6];
    writeToPipe2[3] = fullMessageBuffer[i+7];

    write(pipeData[1][1], writeToPipe2, sizeof(writeToPipe2)); // Write to the pipe
    usleep(2500);
   } 


   read(pipeMPTCP[0], &seqNumber, sizeof(seqNumber)); //write to the thread 2
   

   write(pipeData[2][1], &seqNumber, sizeof(seqNumber)); //update the counter to thread 2
   
   read(pipeFileWriter[0], mainPipeWriter, sizeof(mainPipeWriter)); 
   

   fprintf(file, mainPipeWriter); //write to the log file
   fprintf(file, "\n");


   if(i + 11 < 992) { // Specific condition
     // Set Manually
     writeToPipe3[0] = fullMessageBuffer[i+8];
     writeToPipe3[1] = fullMessageBuffer[i+9];
     writeToPipe3[2] = fullMessageBuffer[i+10];
     writeToPipe3[3] = fullMessageBuffer[i+11];

     write(pipeData[2][1], writeToPipe3, sizeof(writeToPipe3)); // Write to the pipe
	 usleep(2500);
   }
  fflush(stdout); 
  i+= 12;
 }


 kill(process1PID, SIGKILL); //destroy the processes
 kill(process2PID, SIGKILL);
 kill(process3PID, SIGKILL);


 close(socketControl); //destroy the connection
 close(socket1Descript);
 close(socket2Descript);
 close(socket3Descript);
 close(pipeMPTCP);
 close(pipeFileWriter);
 close(pipeData);
 
 // Close the file
 fclose(file);

 exit(0);

}
