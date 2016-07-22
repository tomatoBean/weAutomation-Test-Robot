using System;

namespace autoTestCmdCtrlConsole
{
	
	/*  Message 0.0:  Request Command Message */
	enum  Msg_Head_0_0 : byte
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
	}

	/*  Message 0.1:  Response Command Message */
	enum Message_Head_0_1: byte
	{
		Message_Head_0_1_None = 0,
		Message_Head_0_1_Bit0_Reserved = 0,            // 0000 0000
		Message_Head_0_1_Bit1_Reserved = 0,           // 0000 0001
		Message_Head_0_1_Bit2_Reserved = 0,           // 0000 0010
		Message_Head_0_1_Bit3_Reserved = 0,               // 0000 0100
		Message_Head_0_1_Bit4_Reserved = 0,           // 0000 1000
		Message_Head_0_1_OperationCommandFlag = 32,    // 0001 0000
		Message_Head_0_1_TargetTypeFlag = 64,          // 0010 0000
		Message_Head_0_1_ResponseMessageFlag = 128,    // 0100 0000
		Message_Head_0_1_All = Message_Head_0_1_OperationCommandFlag | Message_Head_0_1_TargetTypeFlag | Message_Head_0_1_ResponseMessageFlag
	};

	/*  Message 0.2:  Response Data Message */
	enum Message_Head_0_2 : byte
	{
		Message_Head_0_2_None = 0,
		Message_Head_0_2_Bit0_Reserved = 0,            // 0000 0000
		Message_Head_0_2_Bit1_Reserved = 0,           // 0000 0001
		Message_Head_0_2_Bit2_Reserved = 0,           // 0000 0010
		Message_Head_0_2_Bit3_Reserved = 0,               // 0000 0100
		Message_Head_0_2_Bit4_Reserved = 0,            // 0000 1000
		Message_Head_0_2_OperationCommandFlag = 32,    // 0001 0000
		Message_Head_0_2_TargetTypeFlag = 64,          // 0010 0000
		Message_Head_0_2_ResponseMessageFlag = 128,    // 0100 0000
		Message_Head_0_2_All = Message_Head_0_2_OperationCommandFlag | Message_Head_0_2_TargetTypeFlag | Message_Head_0_2_ResponseMessageFlag
	};

	/*  Message Body:  Request Command Message */
	enum Message_Body_Command : byte
	{
		Message_Command_Calibrate    =   0xAF,
		Message_Command_Reboot    =   0xB0,
		Message_Command_Poweroff  =   0xB1,
		Message_Command_Auto_Mode =   0xB2,
		Message_Command_Manual_Mode = 0xB3,
		Message_Command_Board_List = 0xB4,

		Message_Data_Request_Temperature = 0xB5,
		Message_Data_Request_Voltage = 0xB6,
		Message_Data_Request_Current = 0xB7,
		Message_Data_Request_CV      = 0xB8,
		Message_Data_Request_SIMULATED_CV = 0xB9,
		Message_Data_Request_SIMULATED_Voltage = 0xBA,
		Message_Data_Request_SIMULATED_Current = 0xBB,
		Message_Data_Request_SIMULATED_Temperature = 0xBC,

		Message_Command_Status_Success = 0x1,
		Message_Command_Status_Failure = 0x2,
		Message_Command_None      = 0x3,

		Message_Data_All_Channels = 0xFF,

		Message_Data_Correct = 0x1,
		Message_Data_Incorrect = 0x2,
		Message_Data_None = 0x3
	}


}

