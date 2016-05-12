using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace clientProtocolDefinition
{
    class GlobalAutoTestID
    {
        public const byte mainControllerBoardNumber = 0xF1;
        public const byte swImageMajorVersion = 0x2;
        public const byte swImageMinorVersion = 0x1;

        public const byte initSessionMessageHeadType1 = 0xA0;
        public const byte initSessionMessageHeadType2 = 0xA1;

        public const byte cmdMessage_RebootTotalLength = 3;
        public const byte cmdMessage_PoweroffTotalLength = 3;

    }

    enum  Msg_Head_0_0
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
    enum Message_Head_0_1
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
    enum Message_Head_0_2
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


    enum Message_Body_Command
    {
        Message_Command_Reboot    =   0xB0,
        Message_Command_Poweroff  =   0xB1,
        Message_Command_Auto_Mode =   0xB2,
        Message_Command_Manual_Mode = 0xB3
    }
}
