#include <errno.h>

static enum {
    SEND_MSG_FLAG = 1,  
    RECEIVE_MSG_FLAG = 2	
} DIR_FLAG;



// main controller card to secondary card
void initialize_jobs(int flag);
void send_downstream_message(const char *msg_data, unsigned char msg_len);
void receive_downstream_message(char*msg_data, unsigned char *msg_len);

