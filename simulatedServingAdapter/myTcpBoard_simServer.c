

    /************tcpserver.c************************/

    /* header files needed to use the sockets API */

    /* File contain Macro, Data Type and Structure */

    /***********************************************/

    #include <stdio.h>

    #include <stdlib.h>

    #include <string.h>

    #include <sys/time.h>

    #include <sys/types.h>

    #include <sys/socket.h>

    #include <netinet/in.h>

    #include <errno.h>

    #include <unistd.h>

    #include <arpa/inet.h>

    #include "proto_format.h"




    /* BufferLength is 100 bytes */

    #define BufferLength 	100

    /* Server port number */

    /* #define SERVPORT 3111  */
    #define SERVPORT 13000

    #define SOFTWARE_VERSION_MAJOR_NUMBER  0x2
    #define SOFTWARE_VERSION_MINOR_NUMBER  0x1

    #define MAIN_CONTROLLER_BOARD_NUMBER   0xF1    

    #define SLAVE_CONTROLLER_TEMPERATURE_BOARD_CHANNEL0    0x1    

    /* index:  ith device as address */
    #define sysdev_temperature_number(ptrDevNo, index)                                  \
    do {                                                                                \
	*ptrDevNo = SLAVE_CONTROLLER_TEMPERATURE_BOARD_CHANNEL0 + index;            \
    } while (0)




void initSession_AcknowledgementMessage(int acceptedSocketFD)
{

    unsigned char buffer[INIT_SESSION_LENGTH+1];
    unsigned char boardNumber = MAIN_CONTROLLER_BOARD_NUMBER;
    unsigned char index = 0;

    int  totalCount = 0;
    
    memset(buffer, 0, sizeof(buffer));
    /* message head */
    buffer[index] = INIT_SESSION_MESSAGE_HEAD_TYPE1;
    /* message body length */
    buffer[++index] = INIT_SESSION_LENGTH;
    /* message: software version */
    buffer[++index] = SOFTWARE_VERSION_MINOR_NUMBER;
    buffer[++index] = SOFTWARE_VERSION_MAJOR_NUMBER;
    /* message: hardware part number */
    buffer[++index] = boardNumber;
   
    totalCount = write(acceptedSocketFD, (void *)buffer, sizeof(buffer));
    printf("Sending initial session: actual %d bytes.\n", totalCount);

}

/*
   Reboot command
   Poweroff Command
   Auto/Manual (Work Mode) Command
*/

void sendCmd_RebootTargetBoard(int acceptedSocketFD)
{
   unsigned char buffer[COMMAND_MESSAGE_REBOOT_TOTAL_LENGTH+1];
   int  totalCount = 0;

   memset(buffer, 0, sizeof(buffer));
   /* headMsg */
   buffer[0] = Message_Head_0_0_RequestMessageFlag;
   buffer[0] |= Message_Head_0_0_TargetTypeFlag;
   buffer[0] |= Message_Head_0_0_SystemCommandFlag;
 
   /* fill in fields of body message  */
   buffer[1] = MAIN_CONTROLLER_BOARD_NUMBER;
   buffer[2] = COMMAND_SESSION_MESSAGE_REBOOT;


   totalCount = write(acceptedSocketFD, (void *)buffer, sizeof(buffer));
   printf("Sending reboot command message: actual %d bytes.\n", totalCount);

}


void analyzeCmd_RebootTargetBoard(int acceptedSocketFD)
{
   unsigned char buffer[COMMAND_MESSAGE_REBOOT_TOTAL_LENGTH+1];
   int  rc, totalCount = 0;

   memset(buffer, 0, sizeof(buffer));

   /*  receive command message */
    rc = read(acceptedSocketFD, &buffer[totalCount], (BufferLength - totalCount));

    if(rc < 0)

    {

	perror("Server-read() error");

	close(acceptedSocketFD);

	//exit (-1);

    }

    else if (rc == 0)

    {

	printf("Client program has issued a close()\n");

	close(acceptedSocketFD);

	//exit(-1);

    }

    else

    {
	int i;

	totalCount += rc;

	printf("Server-read() is OK, reboot command size=%d\n", totalCount);
	printf("Got reboot message from the client with format:  \n");

	for(i=0; i<sizeof(buffer); i++)
		printf(" %#x ", buffer[i]);

	printf("\n");
    }

    
    /* to analyze message content */
    if(buffer[0]&Message_Head_0_0_RequestMessageFlag)
    {
      if(buffer[0]&Message_Head_0_0_TargetTypeFlag)
        printf(" Board type=%#x ", buffer[1]);
      if(buffer[0]&Message_Head_0_0_SystemCommandFlag)
      {
	 if(buffer[2]&COMMAND_SESSION_MESSAGE_REBOOT)
		printf(" Reboot command field=%#x ", buffer[2]);
         printf("\n\n");
      }	

    }
    else
	perror("Message type incorrect.");

}




int main()

{

    /* Variable and structure definitions. */

    int mySocketFD, acceptSocketFD, rc, length = sizeof(int);

    int totalcnt = 0, on = 1;

    char temp;

    char buffer[BufferLength];

    struct sockaddr_in serveraddr;

    struct sockaddr_in their_addr;

     

    fd_set read_fd;

    struct timeval timeout;

    timeout.tv_sec = 15;

    timeout.tv_usec = 0;

     
    int address = 0;
    


    /* testing usage */
    //sysdev_temperature_number(&address, 2);
    //printf("actual device addr: %d\n ", address);



    /* The socket() function returns a socket descriptor */

    /* representing an endpoint. The statement also */

    /* identifies that the INET (Internet Protocol) */

    /* address family with the TCP transport (SOCK_STREAM) */

    /* will be used for this socket. */

    /************************************************/

    /* Get a socket descriptor */

    if((mySocketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)

    {

	    perror("Server-socket() error");

	    /* Just exit */

	    exit (-1);

    }

    else

	    printf("Server-socket() is OK\n");

     

    /* The setsockopt() function is used to allow */

    /* the local address to be reused when the server */

    /* is restarted before the required wait time */

    /* expires. */

    /***********************************************/

    /* Allow socket descriptor to be reusable */

    if((rc = setsockopt(mySocketFD, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on))) < 0)

    {

	    perror("Server-setsockopt() error");

	    close(mySocketFD);

	    exit (-1);

    }

    else

	    printf("Server-setsockopt() is OK\n");

     

    /* bind to an address */

    memset(&serveraddr, 0x00, sizeof(struct sockaddr_in));

    serveraddr.sin_family = AF_INET;

    serveraddr.sin_port = htons(SERVPORT);

    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

     

    printf("Using %s, listening at %d\n", inet_ntoa(serveraddr.sin_addr), SERVPORT);

     

    /* After the socket descriptor is created, a bind() */

    /* function gets a unique name for the socket. */

    /* In this example, the user sets the */

    /* s_addr to zero, which allows the system to */

    /* connect to any client that used port 3005. */

    if((rc = bind(mySocketFD, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0)

    {

	    perror("Server-bind() error");

	    /* Close the socket descriptor */

	    close(mySocketFD);

	    /* and just exit */

	    exit(-1);

    }

    else

        printf("Server-bind() is OK\n");

     

    /* The listen() function allows the server to accept */

    /* incoming client connections. In this example, */

    /* the backlog is set to 10. This means that the */

    /* system can queue up to 10 connection requests before */

    /* the system starts rejecting incoming requests.*/

    /*************************************************/

    /* Up to 10 clients can be queued */

    if((rc = listen(mySocketFD, 10)) < 0)

    {

        perror("Server-listen() error");

        close(mySocketFD);

        exit (-1);

    }

    else

        printf("Server-Ready for client connection...\n");

     

    /* The server will accept a connection request */

    /* with this accept() function, provided the */

    /* connection request does the following: */

    /* - Is part of the same address family */

    /* - Uses streams sockets (TCP) */

    /* - Attempts to connect to the specified port */

    /***********************************************/

    /* accept() the incoming connection request. */

    int sin_size = sizeof(struct sockaddr_in);

    if((acceptSocketFD = accept(mySocketFD, (struct sockaddr *)&their_addr, &sin_size)) < 0)

    {

	perror("Server-accept() error");

	//close(mySocketFD);

	//exit (-1);

    }

    else

	printf("Server-accept() is OK\n");



    /*client IP*/

    printf("Server-new socket, acceptSocketFD is OK...\n");

    printf("Got connection from the client: %s\n", inet_ntoa(their_addr.sin_addr));


    /* Talking to client with acknowledge message of initial session format */
    printf("\n\nTaling to client with server-new socket is OK...\n");
    initSession_AcknowledgementMessage(acceptSocketFD);
    printf("\n\n");



    /*  server keeps talking line with connected client */
    while(1)
    {

	    int sin_size = sizeof(struct sockaddr_in);
	    if((acceptSocketFD = accept(mySocketFD, (struct sockaddr *)&their_addr, &sin_size)) < 0)

	    {

		perror("Server-accept() error");
		//close(mySocketFD);
		//exit (-1);

	    }

	    else

		printf("Server-accept() is OK\n");


	    /* The select() function allows the process to */

	    /* wait for an event to occur and to wake up */

	    /* the process when the event occurs. In this */

	    /* example, the system notifies the process */

	    /* only when data is available to read. */

	    /***********************************************/

	    /* Wait for up to 15 seconds on */

	    /* select() for data to be read. */

	    FD_ZERO(&read_fd);

	    FD_SET(acceptSocketFD, &read_fd);


	    /* Wait for command */
	    rc = select(acceptSocketFD+1, &read_fd, NULL, NULL, &timeout);

	    if((rc == 1) && (FD_ISSET(acceptSocketFD, &read_fd)))

	    {

		    {
			    /* When select() indicates that there is data */

			    /* available, use the read() function to read */

			    /* 100 bytes of the string that the client sent. */

			    /* read() command from client */


			    analyzeCmd_RebootTargetBoard(acceptSocketFD);

		     

		    }

	    }

	    else if (rc < 0)

	    {

		perror("Server-select() error");

		//close(mySocketFD);

		close(acceptSocketFD);

		exit(-1);

	    }

	    /* rc == 0 */

	    else

	    {

		printf("Server-select() timed out.\n");

		//close(mySocketFD);

		close(acceptSocketFD);

		exit(-1);

	    }

     

     

	    /* Echo some bytes of string, back */

	    /* to the client by using the write() */

	    /* function. */

	    /************************************/

	    /* write() some bytes of string, */

	    /* back to the client. */


	    printf("Finish the one-time session...\n\n");
     
    }

    /* When the data has been sent, close() */

    /* the socket descriptor that was returned */

    /* from the accept() verb and close() the */

    /* original socket descriptor. */

    /*****************************************/

    /* Close the connection to the client and */

    /* close the server listening socket. */

    /******************************************/

    if(acceptSocketFD)
    	close(acceptSocketFD);
    if(mySocketFD)
    	close(mySocketFD);


    exit(0);


    return 0;


}

