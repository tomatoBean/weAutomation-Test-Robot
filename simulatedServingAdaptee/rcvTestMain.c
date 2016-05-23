#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "com_comm.h"




int main()
{
  char buffer[128];
  unsigned char size;
  int i=0;

  memset(buffer, 0, sizeof(buffer));	
  // send message

  initialize_jobs(RECEIVE_MSG_FLAG);

  receive_downstream_message(buffer, &size);
  printf("received msg size: %d, ", size);
  for(i=0; i<size; i++)
  {
    printf(" %#x ", buffer[i]); 
  }

  return 0;
}
