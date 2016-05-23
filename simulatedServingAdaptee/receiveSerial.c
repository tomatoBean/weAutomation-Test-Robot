#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>        
#include <stdlib.h> 
#include <unistd.h>

//#define BAUDRATE B115200 
#define BAUDRATE B9600 
//#define MODEMDEVICE "/dev/ttyUSB0"/*UART NAME IN PROCESSOR*/
#define MODEMDEVICE "/dev/pts/18"/*UART NAME IN PROCESSOR*/
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1


void preReceive_openPort(void);
void readPort(char *msg, unsigned char *msgLen);

static int fd=0, n;
static int cnt, size, s_cnt;
static unsigned char *var;
static struct termios oldtp, newtp;
static FILE *file;
 
void readPort(char *msg, unsigned char *msgLen)
{
	unsigned char buff;
	static int index =0;
	int log_fd=0;
	int ret;

	log_fd = open("serialLog.txt", O_RDWR | O_NOCTTY |O_NDELAY );

	while (1) { 	
		/* read 1 byte from serial port */
		n = read(fd, &buff, 1);
		msg[index] = buff;
	        if (n == -1) switch(errno) {
		 case EAGAIN: /* sleep() */ 
		    continue;
		  
		 default: goto quit;
		 }

		 if (n ==0) break;
		// logging file
		ret=write(log_fd, &buff, 1);
		index++;
		printf("%dth byte: %#x \n", index, buff);	
	 }

	*msgLen = index;

	quit:
	   fclose (file);
	   close (log_fd);
}

void preReceive_openPort(void)
{
         
	 fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY |O_NDELAY );
	 printf("Open port :%d\n",fd);
         if (fd <0)
         {
         	perror(MODEMDEVICE);
 
         }
                                                                                
         fcntl(fd,F_SETFL,0);
         tcgetattr(fd,&oldtp); /* save current serial port settings */
    //     tcgetattr(fd,&newtp); /* save current serial port settings */
         bzero(&newtp, sizeof(newtp));
  //       bzero(&oldtp, sizeof(oldtp));
                                                                                
         newtp.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
                                                                                
         newtp.c_iflag = IGNPAR | ICRNL;
                                                                                
         newtp.c_oflag = 0;                                                                        
         newtp.c_lflag = ICANON;                                                                    

         newtp.c_cc[VINTR]    = 0;     /* Ctrl-c */
         newtp.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
         newtp.c_cc[VERASE]   = 0;     /* del */
         newtp.c_cc[VKILL]    = 0;     /* @ */
        // newtp.c_cc[VEOF]     = 4;     /* Ctrl-d */
         newtp.c_cc[VTIME]    = 0;     /* inter-character timer unused */
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
   	                                                                                                                                             
//	  tcflush(fd, TCIFLUSH);
//	 tcsetattr(fd,TCSANOW,&newtp);

}
 

#if 0

int  main()
{
 
	preReceive_openPort();

	readPort();

        return 0;
}

#endif

