using System;

namespace autoTestCmdCtrlConsole
{
	public class GlobalAutoTestID
	{
			public const byte mainControllerBoardNumber = 0xF1;
			public const byte slaveFirstControllerBoardNumber = 0xF2;
			public const byte slaveSecondControllerBoardNumber = 0xF3;

			public const byte slaveControllerTempBoardBaseNumber = 0xB1;

			public const byte swImageMajorVersion = 0x2;
			public const byte swImageMinorVersion = 0x1;

			public const byte initSessionMessageHeadType1 = 0xA0;
			public const byte initSessionMessageHeadType2 = 0xA1;

			public const byte cmdMessage_RebootTotalLength = 4;
			public const byte cmdMessage_PoweroffTotalLength = 4;
			public const byte dataMessage_TempTotalLength = 128;
	}
}

