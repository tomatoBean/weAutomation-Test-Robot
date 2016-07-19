

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

    #include <unistd.h>

    #include <time.h>


    #include "proto_format.h"

    #include "../simulatedServingAdaptee/com_comm.h"

    #include <pthread.h>


    /* BufferLength is 100 bytes */

    #define BufferLength 	100

    /* Server port number */

    /* #define SERVPORT 3111  */
    #define SERVPORT 13000

    #define SOFTWARE_VERSION_MAJOR_NUMBER  0x2
    #define SOFTWARE_VERSION_MINOR_NUMBER  0x1

    #define MAIN_CONTROLLER_BOARD_NUMBER   0xF1    
    #define SLAVE_CONTROLLER_VOLTAGE_CURRENT_BOARD_NUMBER   0xF2    

    #define SLAVE_CONTROLLER_TEMPERATURE_BOARD_CHANNEL0    0x1    

    #define UDP_PORT	4000
    #define UDP_PORT1	4001


    /* index:  ith device as address */
    #define sysdev_temperature_number(ptrDevNo, index)                                  \
    do {                                                                                \
	*ptrDevNo = SLAVE_CONTROLLER_TEMPERATURE_BOARD_CHANNEL0 + index;            \
    } while (0)


    static enum  {
      SUCCESS = 1,
      FAILURE = 2
    } STATUS;

    static enum  {
      SIMULATED = 1,
      REAL = 2
    } MODE;

    static enum  {
      UDP_PROBE_RESPONSE_THREAD = 0,
      TCP_XXX_RESPONSE_THREAD = 1
    } SERVER_THREAD;



static  int acceptSocketFD = 0;;

// main controller card to secondary card
extern void initialize_jobs(int flag);
extern void send_downstream_message(const char *msg_data, unsigned char msg_len);
extern void receive_downstream_message(char*msg_data, unsigned char *msg_len);


 
#define NUM_THREADS 2
 
/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
  int tid;
  double stuff;
} thread_data_t;
 


// broadcast
void broadcast()
{

	char BROAD_CONTENT[] = "BXAVSACK";

	int inet_sock, so_broadcast = 1;
	struct sockaddr_in adr_bc;
	if ((inet_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	    perror("Broadcast UDP created socket error");
	if (setsockopt(inet_sock, SOL_SOCKET, SO_BROADCAST, &so_broadcast,
	    sizeof(so_broadcast)) < 0)
	{
	    perror("Broadcast UDP set socket error");
	    close(inet_sock);
	    return;
	}

	adr_bc.sin_family = AF_INET;
	adr_bc.sin_port = htons(UDP_PORT1);
	adr_bc.sin_addr.s_addr = inet_addr("255.255.255.255");

	if (sendto(inet_sock, BROAD_CONTENT, strlen(BROAD_CONTENT), 0,
	    (struct sockaddr *)&adr_bc, sizeof(adr_bc))< 0)
	{
	    perror("Broadcast send error!");
	    close(inet_sock);
	    return;
	}
	close(inet_sock);
}


// listen on port for client broadcast
int listen_udp_port(void)
{
	printf("Now start listen udp func which port is 4000.\n");

	int inet_sock, socklen, so_reuseaddr = 1;
	char data[128];
	char buff[128];
	struct sockaddr_in addr, from;

	socklen = sizeof(addr);

	//建立socket
	if ((inet_sock = socket(AF_INET, SOCK_DGRAM, 0))< 0)
	    perror("Listen UDP created socket error");

	if (setsockopt(inet_sock, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr,
	    sizeof(so_reuseaddr)) < 0)
	{
	    perror("Listen UDP set socket error");
	    close(inet_sock);
	    return 1;
	}
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(UDP_PORT);
	addr.sin_family = AF_INET;

	if (bind(inet_sock, (struct sockaddr *)&addr, sizeof addr)<0)
	{
	    perror("Listen UDP bind socket error");
	    close(inet_sock);
	    return 2;
	}
	int len;
	for (;;)
	{
	    len = recvfrom(inet_sock, data, 127, 0, (struct sockaddr *)&from,
		(socklen_t*)&socklen);
	    if(len < 0)
	    {
		perror("Listen UDP send error");
		close(inet_sock);
		return 3;
	    }
	    else
	    {
		broadcast();

	       //getpeermac(inet_sock, buff);
		data[len] = '\0';
		printf("收到来自%s:%d的消息:%s\n\r",
		   inet_ntoa(from.sin_addr), ntohs(from.sin_port), data);
		memset(data, 0, 128);
	    }
	    memset(&from, 0, sizeof(from));
	}
	close(inet_sock);

	return 0;
}



/* thread function */
void *thr_func(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;
 
//  while(1)
 // {
	sleep(1);
	switch(data->tid)
	{
	  // thread #0
	  case 0:
	    listen_udp_port();
	    printf("called from thr_func, udp response thread, id: %d\n", data->tid);
	    break;
	 // thread #1
          case 1:
            break;
	  default:
	   break;
	}
 // }

  pthread_exit(NULL);
}
 


void respondCmd_upstreamTargetBoard(int acceptedSocketFD,  int command, int flag);


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
    printf("\nSending initial session: actual %d bytes.\n\n", totalCount);

}

/*
   Reboot command
   Poweroff Command
   Auto/Manual (Work Mode) Command
   Data request command
*/

void receiveCmd_passdownTargetBoard(int acceptedSocketFD)
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

	printf("Server-read() is OK, command size=%d\n", totalCount);
	printf("Got command message from the client with format:  \n");

	for(i=0; i<sizeof(buffer); i++)
		printf(" %#x ", buffer[i]);

	printf("\n");
    }

    
    /* to analyze message content */
    if(buffer[0]&Message_Head_0_0_RequestMessageFlag)
    {
      if(buffer[0]&Message_Head_0_0_TargetTypeFlag)
        printf(" Board type=%#x ", buffer[1]);

      if(buffer[0]&Message_Head_0_0_SystemCommandFlag) // target command 
      {
		/* judge the command type */
		if(buffer[2] == COMMAND_SESSION_MESSAGE_REBOOT)
		{
		    printf("\n Reboot command field=%#x ", buffer[2]);
                	printf("\nSend downstream command message.\n");
			printf("\nwaiting..target....response....\n");
			usleep(2000);
                	printf("\nOkay, got the feedback, then..\n");
			respondCmd_upstreamTargetBoard(acceptSocketFD, COMMAND_SESSION_MESSAGE_REBOOT, FAILURE);
		    //send_downstream_message(buffer);
		    //receive_downstream_message(buffer);
		    printf("\n\n");    
		}
		else if (buffer[2] == COMMAND_SESSION_MESSAGE_POWEROFF)
		{
		    printf("\n Poweroff command field=%#x ", buffer[2]);
                	printf("\nSend downstream command message.\n");
			printf("\nwaiting..target....response....\n");
			usleep(2000);
                	printf("\nOkay, got the feedback, then..\n");
			respondCmd_upstreamTargetBoard(acceptSocketFD, COMMAND_SESSION_MESSAGE_POWEROFF, 	SUCCESS);
		    //send_downstream_message(buffer);
		    //receive_downstream_message(buffer);
		    printf("\n\n");    

		}
      }	
      else  // data request
      {
		if (buffer[2] == DATA_REQUEST_MESSAGE_TEMPERATURE && buffer[3] == DATA_REQUEST_ALL_CHANNELS)
		{
		        printf("\n Data request type field=%#x ", buffer[2]);
                	printf("\nSend downstream data request TEMP message for all channels.\n");
			printf("\nwaiting..target....response....\n");
			usleep(2000);
                	printf("\nOkay, got the temperature data, then..\n");
			respondCmd_upstreamTargetBoard(acceptSocketFD, DATA_REQUEST_MESSAGE_TEMPERATURE, SUCCESS);
		        //send_downstream_message(buffer);
		        //receive_downstream_message(buffer);
		        printf("\n\n");    
		}
		else if (buffer[2] == DATA_REQUEST_MESSAGE_SIMULATED_CV && buffer[3] == DATA_REQUEST_ALL_CHANNELS)
		{
			printf("\n Data request type field=%#x ", buffer[2]);
                	printf("\nSend downstream data request simulated CV message for all channels.\n");
			printf("\nwaiting..target....response....\n");
			usleep(2000);
                	printf("\nOkay, got the simulated CV data, then..\n");
			respondCmd_upstreamTargetBoard(acceptSocketFD, DATA_REQUEST_MESSAGE_SIMULATED_CV, SUCCESS);
		        //send_downstream_message(buffer);
		        //receive_downstream_message(buffer);
		        printf("\n\n");    

		}
      }

    }
    else
	perror("Message type incorrect.");

}


// helper function
void respondCmd_upstreamTargetBoard(int acceptedSocketFD,  int command, int flag)
{

    //unsigned char buffer[COMMAND_MESSAGE_REBOOT_TOTAL_LENGTH+1];
    unsigned char buffer[DATA_MESSAGE_RESPONSE_TOTAL_LENGTH];
    unsigned char boardNumber = MAIN_CONTROLLER_BOARD_NUMBER;
    unsigned char index = 0;

    int  totalCount = 0;
    
    memset(buffer, 0, sizeof(buffer));
    /* headMsg */
   buffer[0] = Message_Head_0_1_ResponseMessageFlag;
   buffer[0] |= Message_Head_0_0_TargetTypeFlag;
   buffer[0] |= Message_Head_0_0_SystemCommandFlag;
 
    /* fill in fields of body message  */
   sysdev_temperature_number(&boardNumber, 0);
   buffer[1] = boardNumber;
   if(command == COMMAND_SESSION_MESSAGE_REBOOT)
   {
   	buffer[2] = COMMAND_SESSION_MESSAGE_REBOOT;
   	buffer[3] = flag;
   }
   else if(command == COMMAND_SESSION_MESSAGE_POWEROFF)
   {
   	buffer[2] = COMMAND_SESSION_MESSAGE_POWEROFF;
   	buffer[3] = flag;
   }
   else if(command == DATA_REQUEST_MESSAGE_TEMPERATURE)
   {
   	buffer[2] = 24;

	buffer[3] = 0x1; //CH #01
	buffer[4] = 15;
	buffer[5] = 0x2; //CH #02
	buffer[6] = 18;
	buffer[7] = 0x3; //CH #03
	buffer[8] = 15;
	buffer[9] = 0x4; //CH #04
	buffer[10] = 15;
	buffer[11] = 0x5; //CH #05
	buffer[12] = 15;
	buffer[13] = 0x6; //CH #06
	buffer[14] = 15;
	buffer[15] = 0x7; //CH #07
	buffer[16] = 15;
	buffer[17] = 0x8; //CH #08
	buffer[18] = 15;
	buffer[19] = 0x9; //CH #09
	buffer[20] = 15;
	buffer[21] = 10; //CH #10
	buffer[22] = 15;
	buffer[23] = 11; //CH #11
	buffer[24] = 15; 
	buffer[25] = 12; //CH #12
	buffer[26] = 15;
	buffer[27] = 13; //CH #13
	buffer[28] = 15;
	buffer[29] = 14; //CH #14
	buffer[30] = 15;
	buffer[31] = 15; //CH #15
	buffer[32] = 15;
	buffer[33] = 16; //CH #16
	buffer[34] = 15;
	buffer[35] = 17; //CH #17
	buffer[36] = 15;
	buffer[37] = 18; //CH #18
	buffer[38] = 15;
	buffer[39] = 19; //CH #19
	buffer[40] = 15;
	buffer[41] = 20; //CH #20
	buffer[42] = 15;
	buffer[43] = 21; //CH #21
	buffer[44] = 15;
	buffer[45] = 22; //CH #22
	buffer[46] = 15;
	buffer[47] = 23; //CH #23
	buffer[48] = 15;
	buffer[49] = 24; //CH #24
	buffer[50] = 25;
   }
   // voltage and current
   else if(command == DATA_REQUEST_MESSAGE_SIMULATED_CV)
   {
	int i, count;
	unsigned char voltage, current;

	srand((int)time(0));
        voltage=1+(int)(20.0*rand()/(RAND_MAX+1.0));
        current=1+(int)(5.0*rand()/(RAND_MAX+1.0));

   	count=buffer[2] = 24;

	for(i=0; i<count; i++)
	{
	   buffer[3+3*i] = i+1; //CH #01
	   buffer[4+3*i] = voltage;
           buffer[5+3*i] = current;		 	
	   printf("response data value:  channel=%d, voltage=%d, current=%d\n", buffer[3+3*i], buffer[4+3*i], buffer[5+3*i]);
	}
   }

   else if(command == DATA_REQUEST_MESSAGE_CV)
   {
   	buffer[2] = 24;
   }
   // only voltage
   else if(command == DATA_REQUEST_MESSAGE_VOLTAGE)
   {
   }
   // only current
   else if(command == DATA_REQUEST_MESSAGE_CURRENT)
   {

   }

   totalCount = write(acceptedSocketFD, (void *)buffer, sizeof(buffer));
   printf("Sending command response message: actual %d bytes.\n\n\n", totalCount);

}


// message request interface
void requestCmd_downstreamTargetBoardBySerialPort(int command)
{

    unsigned char buffer[DATA_MESSAGE_RESPONSE_TOTAL_LENGTH];
    unsigned char boardNumber = SLAVE_CONTROLLER_VOLTAGE_CURRENT_BOARD_NUMBER;
    unsigned char index = 0;

    int  totalCount = 0;
    
   memset(buffer, 0, sizeof(buffer));

    /* headMsg */
   buffer[0] = Message_Head_1_0_RequestMessageFlag;
   buffer[0] |= Message_Head_1_0_TargetTypeFlag;
   buffer[0] |= Message_Head_1_0_SystemCommandFlag;
 
 
   if(command == DATA_REQUEST_ALL_CHANNELS)
   {
	   /* fill in fields of body message  */
	   buffer[1] = boardNumber; 
	   buffer[2] = Message_Body_1_0_CV_AllCmdFlag;
	   buffer[2] |= Message_Body_1_0_All_Channels;
   }
   // only current
   else if(command == DATA_REQUEST_MESSAGE_CURRENT)
   {

   }

   //totalCount = write(acceptedSocketFD, (void *)buffer, sizeof(buffer));
   printf("Sending downstream message of command response: actual %d bytes.\n\n\n", totalCount);

}



int main()

{

    /* Variable and structure definitions. */

    int mySocketFD, rc, length = sizeof(int);

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
    


    
    /* testing interface */
    //initialize_jobs();
    //send_downstream_message(NULL);
    //receive_downstream_message(NULL);

    /* testing usage */
    //sysdev_temperature_number(&address, 2);
    //printf("actual device addr: %d\n ", address);

    /* create threaded services firstly  */
    pthread_t thr[NUM_THREADS];
    int i, retc;
    /* create a thread_data_t argument array */
    thread_data_t thr_data[NUM_THREADS];
 
    /* create threads */
    for (i = 0; i < NUM_THREADS; ++i) {
	    thr_data[i].tid = i;
	    if ((retc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
		      fprintf(stderr, "error: pthread_create, retc: %d\n", retc);
	      return EXIT_FAILURE;
    }
  }


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


    //send_downstream_message(NULL,0);
    //receive_downstream_message(NULL,0);

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

			    receiveCmd_passdownTargetBoard(acceptSocketFD);


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

