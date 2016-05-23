#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>        
#include <unistd.h>


        
//#define BAUDRATE B115200 
#define BAUDRATE B9600 
//#define MODEMDEVICE "/dev/ttyUSB1"
#define MODEMDEVICE "/dev/pts/17"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
 
void preSend_openPort(void);
void writePort(char *msg, unsigned char msgLen);


static FILE *file;
static unsigned char fileLen;
static int fd=0;
static struct termios oldtp, newtp;

void writePort(char *msg, unsigned char msgLen)
{
	int n = 0;
	int count = 0;

	printf("Start send\n");

	while (count < msgLen) { 

		n = write(fd, &msg[count], 1);

		if (n == -1) switch(errno)  {
			 case EAGAIN: /* sleep() */ 
			    continue;
			 default: goto quit;
		}
		if (n ==0) break;

		//test_n = write(test_fd, &buffer[count], 1);
		printf(" %d bytes sent:  %#x \n", n, msg[count]);

		count++;
	 } 

	quit:
	   close (fd);

}

 
#if 0
void  writePort(void)
{
	unsigned char n = 0;
	unsigned char test_n = 0;
	unsigned char count = 0;
	int i=0;

	file = fopen( "sample.txt", "r" );

	//get file size
	fseek(file, 0, SEEK_END);
	fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = (char *)malloc(fileLen + 1);
	memset(buffer, 0, (size_t)(fileLen + 1));

	//read file contents
//fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
	fread(buffer, fileLen, 1, file);
	printf("fileLen=%d\n", fileLen);
	fclose(file);

	printf("Start send\n");

	// send only 1 byte
	// working by verification
	//n = write(fd, &buffer[count], 1);

	for(i=0; i<fileLen; i++)
        {
		printf("\nSend value=%#x\n", buffer[i] );
		n = write(fd, &buffer[i], 1);
	}	

	quit:
	   close (fd);

}

#endif


void preSend_openPort(void)
{
        fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY |O_NDELAY );
	printf("Open port:  %d\n",fd);
         if (fd <0)
         {
         perror(MODEMDEVICE);         }
                                                                                
         fcntl(fd,F_SETFL,0);
        tcgetattr(fd,&oldtp); /* save current serial port settings */
        // tcgetattr(fd,&newtp); /* save current serial port settings */
         bzero(&newtp, sizeof(newtp));


        // bzero(&oldtp, sizeof(oldtp));

         //newtp.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
         newtp.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
         newtp.c_oflag = IGNPAR | ICRNL;
         newtp.c_iflag = 0;                                                                        
         newtp.c_lflag = ICANON;                                                                    

         newtp.c_cc[VINTR]    = 0;     /* Ctrl-c */
         newtp.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
         newtp.c_cc[VERASE]   = 0;     /* del */
         newtp.c_cc[VKILL]    = 0;     /* @ */
         //newtp.c_cc[VEOF]     = 4;     /* Ctrl-d */
         newtp.c_cc[VEOF]     = 0;     /* Ctrl-d */
         newtp.c_cc[VTIME]    = 0;     /* inter-character timer unused */
         //newtp.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
         newtp.c_cc[VMIN]     = 0;     /* blocking read until 1 character arrives */
         newtp.c_cc[VSWTC]    = 0;     /* '\0' */
         newtp.c_cc[VSTART]   = 0;     /* Ctrl-q */
         newtp.c_cc[VSTOP]    = 0;     /* Ctrl-s */
         newtp.c_cc[VSUSP]    = 0;     /* Ctrl-z */
         newtp.c_cc[VEOL]     = 0;     /* '\0' */
         newtp.c_cc[VREPRINT] = 0;     /* Ctrl-r */
         newtp.c_cc[VDISCARD] = 0;     /* Ctrl-u */
         newtp.c_cc[VWERASE]  = 0;     /* Ctrl-w */
         newtp.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
         newtp.c_cc[VEOL2]    = 0;     /* '\0' */
                                                                                
                                                                                
   //     tcflush(fd, TCIFLUSH);
//	tcsetattr(fd,TCSANOW,&newtp);
}


#if 0
void main()
{
 

	preSend_openPort();
	sleep(1);
	writePort();

}
 
#endif
