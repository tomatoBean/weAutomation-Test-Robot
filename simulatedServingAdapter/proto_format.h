
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
    #define DATA_REQUEST_MESSAGE_CV     		0xB7
    #define DATA_REQUEST_MESSAGE_SIMULATED_CV  		0xB8


    #define DATA_REQUEST_ALL_CHANNELS     		0xFF



/*  Message 0.0:  Request Message */
enum Message_Head_0_0
{
	Message_Head_0_0_None = 0,		// 0000 0000
	Message_Head_0_0_TemperatureFlag = 1,   // 0000 0001
	Message_Head_0_0_CurrentFlag = 2,   	   // 0000 0010
	Message_Head_0_0_VoltageFlag = 4, 	    // 0000 0100
	Message_Head_0_0_TargetChannelFlag = 8,      // 0000 1000
	Message_Head_0_0_WorkModeFlag = 16,          // 0001 0000
	Message_Head_0_0_SystemCommandFlag = 32,     // 0010 0000
	Message_Head_0_0_TargetTypeFlag = 64,        // 0100 0000
	Message_Head_0_0_RequestMessageFlag = 128,   // 0100 0000
	Message_Head_0_0_CV_Test_ALL = Message_Head_0_0_CurrentFlag | Message_Head_0_0_VoltageFlag | Message_Head_0_0_TargetChannelFlag | Message_Head_0_0_WorkModeFlag | Message_Head_0_0_SystemCommandFlag | Message_Head_0_0_TargetTypeFlag | Message_Head_0_0_RequestMessageFlag,
	Message_Head_0_0_All = Message_Head_0_0_TemperatureFlag | Message_Head_0_0_CurrentFlag | Message_Head_0_0_VoltageFlag | Message_Head_0_0_TargetChannelFlag | Message_Head_0_0_WorkModeFlag | Message_Head_0_0_SystemCommandFlag | Message_Head_0_0_TargetTypeFlag | Message_Head_0_0_RequestMessageFlag
};


/*  Message 0.1:  Response Command Message */
enum Message_Head_0_1
{
	Message_Head_0_1_None = 0,		// 0000 0000
	Message_Head_0_1_Bit0_Reserved = 0,            // 0000 0001
	Message_Head_0_1_Bit1_Reserved = 0,   	      // 0000 0010
	Message_Head_0_1_Bit2_Reserved = 0, 	      // 0000 0100
	Message_Head_0_1_Bit3_Reserved = 0,          // 0000 1000
	Message_Head_0_1_Bit4_Reserved = 0,           // 0001 0000
	Message_Head_0_1_OperationCommandFlag = 32,   // 0010 0000
	Message_Head_0_1_TargetTypeFlag = 64,          // 0100 0000
	Message_Head_0_1_ResponseMessageFlag = 128,    // 1000 0000
//	Message_Head_0_1_All = Message_Head_0_1_OperationCommandFlag | Message_Head_0_1_TargetTypeFlag | Message_Head_0_1_ResponseMessageFlag
};


/*  Message 0.2:  Response Data Message */
enum Message_Head_0_2
{
	Message_Head_0_2_None = 0,		   // 0000 0000
	Message_Head_0_2_Bit0_Reserved = 0,            // 0000 0001
	Message_Head_0_2_Bit1_Reserved = 0,   	      // 0000 0010
	Message_Head_0_2_Bit2_Reserved = 0, 	      // 0000 0100
	Message_Head_0_2_Bit3_Reserved = 0,     	      // 0000 1000
	Message_Head_0_2_Bit4_Reserved = 0,            // 0001 0000
	Message_Head_0_2_OperationCommandFlag = 32,    // 0010 0000
	Message_Head_0_2_TargetTypeFlag = 64,          // 0100 0000
	Message_Head_0_2_ResponseMessageFlag = 128,    // 1000 0000
//	Message_Head_0_2_All = Message_Head_0_2_OperationCommandFlag | Message_Head_0_2_TargetTypeFlag | Message_Head_0_2_ResponseMessageFlag
};


/*  Message Header 1.0:  Request Message Header */
enum Message_Head_1_0
{
	Message_Head_1_0_None = 0,           	// 0000 0000
	Message_Head_1_0_Bit0_Reserved = 0,         // 0000 0001
	Message_Head_1_0_CurrentFlag = 2,   	    // 0000 0010
	Message_Head_1_0_VoltageFlag = 4, 	    // 0000 0100
	Message_Head_1_0_TargetChannelFlag = 8,      // 0000 1000
	Message_Head_1_0_Bit4_Reserved = 16,          // 0001 0000
	Message_Head_1_0_SystemCommandFlag = 32,     // 0010 0000
	Message_Head_1_0_TargetTypeFlag = 64,        // 0100 0000
	Message_Head_1_0_RequestMessageFlag = 128,   // 1000 0000
	Message_Head_1_0_CV_Test_ALL = Message_Head_1_0_CurrentFlag | Message_Head_1_0_VoltageFlag | Message_Head_1_0_TargetChannelFlag | Message_Head_1_0_SystemCommandFlag | Message_Head_1_0_TargetTypeFlag | Message_Head_1_0_RequestMessageFlag,
	Message_Head_1_0_All = Message_Head_1_0_CurrentFlag | Message_Head_1_0_VoltageFlag | Message_Head_1_0_TargetChannelFlag | Message_Head_1_0_SystemCommandFlag | Message_Head_1_0_TargetTypeFlag | Message_Head_1_0_RequestMessageFlag
};

/*  Message Body 1.0:  Request Message Body */
/* Bit0 ~ Bit2: reboot, poweroff, ....command   */
/* Bit3 ~ Bit7: channel number, particularly all set bits means all channels */
enum Message_Body_1_0
{
	Message_Body_1_0_RebootCmdFlag = 0x1,      // 0000 0001
	Message_Body_1_0_PoweroffCmdFlag = 0x2,      // 0000 0010
	Message_Body_1_0_Bit2_Reserved = 0x3, 	    // 0000 0011
	Message_Body_1_0_CV_AllCmdFlag = 0x7,	   // 0000 0111
	Message_Body_1_0_All_Channels = 0xF8,	    // 1111 1000	
};



/*  Message 1.1:  Response Command Message */
/*  1st byte for board type; 2nd byte for execution status */
enum Message_Head_1_1
{
	Message_Head_1_1_None = 0, 		// 0000 0000
	Message_Head_1_1_Bit0_Reserved = 1,         // 0000 0001
	Message_Head_1_1_CurrentFlag = 2,   	    // 0000 0010
	Message_Head_1_1_VoltageFlag = 4, 	    // 0000 0100
	Message_Head_1_1_TargetChannelFlag = 8,      // 0000 1000
	Message_Head_1_1_Bit4_Reserved = 16,          // 0001 0000
	Message_Head_1_1_SystemCommandFlag = 32,     // 0010 0000
	Message_Head_1_1_TargetTypeFlag = 64,        // 0100 0000
	Message_Head_1_1_ResponseMessageFlag = 128,   // 1000 0000
	Message_Head_1_1_All = Message_Head_1_1_ResponseMessageFlag | Message_Head_1_1_TargetTypeFlag | Message_Head_1_1_SystemCommandFlag
};

/*  Message Body 1.0:  Request Message Body */
/* Byte 0: Target Board Type  */
/* Byte 1: execution status of command reboot, poweroff,...... */
/* Set bit for success, clear bit for failure */
enum Message_Body_1_1
{
	Message_Body_1_1_None = 0, 		// 0000 0000
	Message_Body_1_1_Poweroff_Status = 1,         // 0000 0001
	Message_Body_1_1_Reboot_Status = 2,   	    // 0000 0010
	Message_Body_1_1_Bit2_Reserved = 0, 	    // 0000 0100
};


/*  Message 1.2:  Response Data Message */
enum Message_Head_1_2
{
	Message_Head_1_2_None = 0,           	// 0000 0000
	Message_Head_1_2_Bit0_Reserved = 0,         // 0000 0001
	Message_Head_1_2_CurrentFlag = 2,   	    // 0000 0010
	Message_Head_1_2_VoltageFlag = 4, 	    // 0000 0100
	Message_Head_1_2_TargetChannelFlag = 8,      // 0000 1000
	Message_Head_1_2_Bit4_Reserved = 16,          // 0001 0000
	Message_Head_1_2_SystemCommandFlag = 32,     // 0010 0000
	Message_Head_1_2_TargetTypeFlag = 64,        // 0100 0000
	Message_Head_1_2_ResponseMessageFlag = 128,   // 1000 0000

	Message_Head_1_2_All = Message_Head_1_2_ResponseMessageFlag | Message_Head_1_2_TargetTypeFlag | Message_Head_1_2_SystemCommandFlag | Message_Head_1_2_TargetChannelFlag // CV of all channels
};


