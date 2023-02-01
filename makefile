all:
	gcc -c Server.c 
	gcc -c Client.c
	gcc -o Client Client.o
	gcc -o Server Server.o

clean: 
	rm -f Server Server.o Client Client.o clientSideLog.txt DSSLog.txt

