#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "com_comm.h"



int main()
{
  char buffer[128];

  memset(buffer, 0, sizeof(buffer));	
  // send message

  buffer[0] = 0x2;
  buffer[1] = 0x4;
  buffer[2] = 0x6;

  initialize_jobs(SEND_MSG_FLAG);

  send_downstream_message(buffer, 3);

  return 0;
}
