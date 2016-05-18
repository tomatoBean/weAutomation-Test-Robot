


    #define INIT_SESSION_LENGTH 4  /* header removed  */
    #define COMMAND_MESSAGE_REBOOT_TOTAL_LENGTH 	3 /* header + data area  */
    #define COMMAND_MESSAGE_POWEROFF_TOTAL_LENGTH 	3 /* header + data area  */

    #define DATA_MESSAGE_RESPONSE_TOTAL_LENGTH 		128 /* header + data area  */

    #define INIT_SESSION_MESSAGE_HEAD_TYPE1     0xA0
    #define INIT_SESSION_MESSAGE_HEAD_TYPE2     0xA1


    #define COMMAND_SESSION_MESSAGE_REBOOT     		0xB0
    #define COMMAND_SESSION_MESSAGE_POWEROFF     	0xB1
    #define COMMAND_SESSION_MESSAGE_AUTO_MODE     	0xB2
    #define COMMAND_SESSION_MESSAGE_MANUAL_MODE     	0xB3

    #define DATA_REQUEST_MESSAGE_TEMPERATURE   		0xB4
    #define DATA_REQUEST_MESSAGE_VOLTAGE     		0xB5
    #define DATA_REQUEST_MESSAGE_CURRENT     		0xB6


    #define DATA_REQUEST_ALL_CHANNELS     		0xFF



/*  Message 0.0:  Request Message */
enum Message_Head_0_0
{
	Message_Head_0_0_None = 0,
	Message_Head_0_0_TemperatureFlag = 1,        // 0000 0000
	Message_Head_0_0_CurrentFlag = 2,   	    // 0000 0001
	Message_Head_0_0_VoltageFlag = 4, 	    // 0000 0010
	Message_Head_0_0_TargetChannelFlag = 8,      // 0000 0100
	Message_Head_0_0_WorkModeFlag = 16,          // 0000 1000
	Message_Head_0_0_SystemCommandFlag = 32,     // 0001 0000
	Message_Head_0_0_TargetTypeFlag = 64,        // 0010 0000
	Message_Head_0_0_RequestMessageFlag = 128,   // 0100 0000
	Message_Head_0_0_CV_Test_ALL = Message_Head_0_0_CurrentFlag | Message_Head_0_0_VoltageFlag | Message_Head_0_0_TargetChannelFlag | Message_Head_0_0_WorkModeFlag | Message_Head_0_0_SystemCommandFlag | Message_Head_0_0_TargetTypeFlag | Message_Head_0_0_RequestMessageFlag,
	Message_Head_0_0_All = Message_Head_0_0_TemperatureFlag | Message_Head_0_0_CurrentFlag | Message_Head_0_0_VoltageFlag | Message_Head_0_0_TargetChannelFlag | Message_Head_0_0_WorkModeFlag | Message_Head_0_0_SystemCommandFlag | Message_Head_0_0_TargetTypeFlag | Message_Head_0_0_RequestMessageFlag
};


/*  Message 0.1:  Response Command Message */
enum Message_Head_0_1
{
	Message_Head_0_1_None = 0,
	Message_Head_0_1_Bit0_Reserved = 0,            // 0000 0000
	Message_Head_0_1_Bit1_Reserved = 0,   	      // 0000 0001
	Message_Head_0_1_Bit2_Reserved = 0, 	      // 0000 0010
	Message_Head_0_1_Bit3_Reserved = 0,     	      // 0000 0100
	Message_Head_0_1_Bit4_Reserved = 0,           // 0000 1000
	Message_Head_0_1_OperationCommandFlag = 32,    // 0001 0000
	Message_Head_0_1_TargetTypeFlag = 64,          // 0010 0000
	Message_Head_0_1_ResponseMessageFlag = 128,    // 0100 0000
//	Message_Head_0_1_All = Message_Head_0_1_OperationCommandFlag | Message_Head_0_1_TargetTypeFlag | Message_Head_0_1_ResponseMessageFlag
};


/*  Message 0.2:  Response Data Message */
enum Message_Head_0_2
{
	Message_Head_0_2_None = 0,
	Message_Head_0_2_Bit0_Reserved = 0,            // 0000 0000
	Message_Head_0_2_Bit1_Reserved = 0,   	      // 0000 0001
	Message_Head_0_2_Bit2_Reserved = 0, 	      // 0000 0010
	Message_Head_0_2_Bit3_Reserved = 0,     	      // 0000 0100
	Message_Head_0_2_Bit4_Reserved = 0,            // 0000 1000
	Message_Head_0_2_OperationCommandFlag = 32,    // 0001 0000
	Message_Head_0_2_TargetTypeFlag = 64,          // 0010 0000
	Message_Head_0_2_ResponseMessageFlag = 128,    // 0100 0000
//	Message_Head_0_2_All = Message_Head_0_2_OperationCommandFlag | Message_Head_0_2_TargetTypeFlag | Message_Head_0_2_ResponseMessageFlag
};


