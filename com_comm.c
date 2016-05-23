#include <errno.h>
#include <stdio.h>

#include "com_comm.h"

// main controller card to secondary card
void initialize_jobs(int flag);
void send_downstream_message(const char *msg_data, unsigned char msg_len);
void receive_downstream_message(char*msg_data, unsigned char *msg_len);


extern void preSend_openPort(void);
extern void writePort(const char *msg, unsigned char msgLen);

extern void preReceive_openPort(void);
extern void readPort(char *msg, unsigned char *msgLen);


void initialize_jobs(int flag)
{
   if(flag == SEND_MSG_FLAG)
     preSend_openPort();
   else if(flag == RECEIVE_MSG_FLAG)
     preReceive_openPort();
   else
     perror("Err.specified direction flag\n");
}


void send_downstream_message(const char *msg_data, unsigned char msg_len)
{
  writePort(msg_data, msg_len);
}

void receive_downstream_message(char*msg_data, unsigned char *msg_len)
{
  readPort(msg_data, msg_len);
}

