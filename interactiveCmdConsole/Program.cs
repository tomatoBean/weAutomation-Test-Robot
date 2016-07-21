using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace autoTestCmdCtrlConsole
{
	
	class MainClass
	{

		// parameters for udp probe messages
		private static UInt32 blockingTimes = 0;
		private static byte[] helloBuffer = System.Text.Encoding.Default.GetBytes("BXGISNULL");
		// to prevent bindings of multi-time address info
		private static UdpClient rcvClient = new UdpClient(new IPEndPoint(IPAddress.Any, 4001));
		private static IPEndPoint rcvEndpoint = new IPEndPoint(IPAddress.Any, 0);
		private static UdpClient udpClient = new UdpClient(new IPEndPoint(IPAddress.Any, 0));
		private static IPEndPoint udpEndpoint = new IPEndPoint(IPAddress.Parse("255.255.255.255"), 4000);

		public static string serverIP = "192.168.100.10";
		public const Int32 port = 13000;
		public static NetworkStream sessionStream = null;
		public static TcpClient client = null;
		public static byte[] data = null;
		//public static sbyte[] data = null;
		public static Int32 bytes = 0;

		public static bool CompareByteArray(byte[] b1, byte[] b2)
		{
			if (b1.Length != b2.Length)
				return false;
			for (int i = 0; i < b1.Length; ++i)
				if (b1[i] != b2[i])
					return false;
			return true;
		}


		public static bool probeServer()
		{
			// UdpClient client = new UdpClient(new IPEndPoint(IPAddress.Any, 0));
			// IPEndPoint endpoint = new IPEndPoint(IPAddress.Parse("255.255.255.255"), 4000);
			byte[] buf = Encoding.Default.GetBytes("BXAVS");
			Thread t = new Thread(new ThreadStart(RecvThread));
			t.IsBackground = true;
			t.Start();

			while (blockingTimes < 3)
			{
				// broadcast out
				udpClient.Send(buf, buf.Length, udpEndpoint);
				// count times of broadcast
				blockingTimes++;
				// short delay for ack message blocking
				Thread.Sleep(1000);

			}

			// decide what should return
			if (CompareByteArray(helloBuffer, System.Text.Encoding.Default.GetBytes("BXGISNULL")))
				return false;
			else if (CompareByteArray(helloBuffer, System.Text.Encoding.Default.GetBytes("BXAVSACK")))
				return true;
			else
				return false;
		}

		// udp message reception
		static void RecvThread()
		{

			// UdpClient client = new UdpClient(new IPEndPoint(IPAddress.Any, 4001));
			// IPEndPoint endpoint = new IPEndPoint(IPAddress.Any, 0);

			// while (true)
			// {
			helloBuffer = rcvClient.Receive(ref rcvEndpoint);
			//string msg = Encoding.Default.GetString(helloBuffer);
			// Got that message, terminate probe action
			blockingTimes = 6;

			//   }

		}
			
		public static void preSendCmdMessage()
		{
			Console.WriteLine("Connection with server:  Open command sesssion.");
			client = new TcpClient(serverIP, port);
			// Send the message to the connected TcpServer. 
			sessionStream = client.GetStream();
		}

		public static void postSendCmdMessage()
		{
			Console.WriteLine("Close command sesssion.");
			sessionStream.Close();
			client.Close();
		}


		// Interface to pass down command
		public static void sendCmd_RequestMessage(Message_Body_Command command, byte targetBoardNum)
		{

			data = new byte[GlobalAutoTestID.cmdMessage_RebootTotalLength + 1];

			if (command == Message_Body_Command.Message_Command_Board_List)
			{

				/* only headMsg */
				/* special target type:  bit6 = 0 */
				/* presently it is reserved. */
				data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_SystemCommandFlag;

				/* fill in fields of body message  */
			//	data[1] = GlobalAutoTestID.mainControllerBoardNumber;
			//	data[2] = (byte)Message_Body_Command.Message_Command_Reboot;
			//	data[3] = 0;
			//	data[4] = 0;

				sessionStream.Write(data, 0, data.Length);
				Console.WriteLine("Sent: Reboot Command finished.");
			}
			if (command == Message_Body_Command.Message_Command_Reboot)
			{

				/* headMsg */
				data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_SystemCommandFlag;

				/* fill in fields of body message  */
				//data[1] = GlobalAutoTestID.mainControllerBoardNumber; // default
				data[1] = targetBoardNum;
				data[2] = (byte)Message_Body_Command.Message_Command_Reboot;
				data[3] = 0;
				data[4] = 0;

				sessionStream.Write(data, 0, data.Length);
				Console.WriteLine("Sent: Reboot Command finished.");
			}
			else if (command == Message_Body_Command.Message_Command_Poweroff)
			{
				/* headMsg */
				data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_SystemCommandFlag;

				/* fill in fields of body message  */
				//data[1] = GlobalAutoTestID.mainControllerBoardNumber;
				data[1] = targetBoardNum;
				data[2] = (byte)Message_Body_Command.Message_Command_Poweroff;
				data[3] = 0;
				data[4] = 0;

				sessionStream.Write(data, 0, data.Length);
				Console.WriteLine("Sent: Poweroff Command finished.");
			}
			// Temperateure request
			else if (command == Message_Body_Command.Message_Data_Request_Temperature)
			{
				/* headMsg */
				data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetChannelFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TemperatureFlag;

				/* fill in fields of body message  */
				data[1] = GlobalAutoTestID.slaveControllerTempBoardBaseNumber;
				data[2] = (byte)Message_Body_Command.Message_Data_Request_Temperature;
				data[3] = (byte)Message_Body_Command.Message_Data_All_Channels;

				sessionStream.Write(data, 0, data.Length);
				Console.WriteLine("Sent: Temp Data Request Command finished.");
			}
			// Simulated Voltage and Current request
			else if (command == Message_Body_Command.Message_Data_Request_SIMULATED_CV)
			{
				/* headMsg */
				data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetChannelFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_VoltageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_CurrentFlag;
				/* fill in fields of body message  */
				data[1] = GlobalAutoTestID.mainControllerBoardNumber;
				data[2] = (byte)Message_Body_Command.Message_Data_Request_SIMULATED_CV;
				data[3] = (byte)Message_Body_Command.Message_Data_All_Channels;

				sessionStream.Write(data, 0, data.Length);
				Console.WriteLine("Sent: Simulated CV Data Request Command finished.");
			}
			// Simulated Voltage
			else if (command == Message_Body_Command.Message_Data_Request_SIMULATED_Voltage)
			{
				/* headMsg */
				data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetChannelFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_VoltageFlag;
				/* fill in fields of body message  */
				data[1] = GlobalAutoTestID.mainControllerBoardNumber;
				data[2] = (byte)Message_Body_Command.Message_Data_Request_SIMULATED_Voltage;
				data[3] = (byte)Message_Body_Command.Message_Data_All_Channels;

				sessionStream.Write(data, 0, data.Length);
				Console.WriteLine("Sent: Simulated Voltage Data Request Command finished.");
			}
			// Simulated Current
			else if (command == Message_Body_Command.Message_Data_Request_SIMULATED_Current)
			{
				/* headMsg */
				data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetChannelFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_CurrentFlag;
				/* fill in fields of body message  */
				data[1] = GlobalAutoTestID.mainControllerBoardNumber;
				data [2] = (byte)Message_Body_Command.Message_Data_Request_SIMULATED_Current;
				data[3] = (byte)Message_Body_Command.Message_Data_All_Channels;

				sessionStream.Write(data, 0, data.Length);
				Console.WriteLine("Sent: Simulated Current Data Request Command finished.");
			}
			// Simulated Temperature
			else if (command == Message_Body_Command.Message_Data_Request_SIMULATED_Temperature)
			{
				/* headMsg */
				data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetChannelFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TemperatureFlag;
				/* fill in fields of body message  */
				data[1] = GlobalAutoTestID.mainControllerBoardNumber;
				data[2] = (byte)Message_Body_Command.Message_Data_Request_SIMULATED_Temperature;
				data[3] = (byte)Message_Body_Command.Message_Data_All_Channels;

				sessionStream.Write(data, 0, data.Length);
				Console.WriteLine("Sent: Simulated Temperature Data Request Command finished.");
			}

			// Voltage and Current request
			else if (command == Message_Body_Command.Message_Data_Request_CV)
			{
				/* headMsg */
				data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetChannelFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_VoltageFlag;
				data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_CurrentFlag;
				/* fill in fields of body message  */
				data[1] = GlobalAutoTestID.mainControllerBoardNumber;
				data[2] = (byte)Message_Body_Command.Message_Data_Request_CV;
				data[3] = (byte)Message_Body_Command.Message_Data_All_Channels;

				sessionStream.Write(data, 0, data.Length);
				Console.WriteLine("Sent: CV Data Request Command finished.");
			}
		}

		public static void acquireCmd_ResponseMessage(Message_Body_Command command, out byte cmdStatus)
		{

			data = new byte[GlobalAutoTestID.dataMessage_TempTotalLength + 1];
			// Read the first session message of the TcpServer ack bytes.
			bytes = sessionStream.Read(data, 0, data.Length);
			// Analyze the server response message
			Console.WriteLine("Command Message head type: {0}", data[0]);

			cmdStatus = 0;
			if (command == Message_Body_Command.Message_Command_Reboot && data[2] == (byte)Message_Body_Command.Message_Command_Reboot)
			{
				if (data[3] == (byte)Message_Body_Command.Message_Command_Status_Success)
				{
					//Console.WriteLine("Command execution success.");
					cmdStatus = (byte)Message_Body_Command.Message_Command_Status_Success;
				}

				if (data[3] == (byte)Message_Body_Command.Message_Command_Status_Failure)
				{
					//Console.WriteLine("Command execution failure.");
					cmdStatus = (byte)Message_Body_Command.Message_Command_Status_Failure;
				}
			}
			else if (command == Message_Body_Command.Message_Command_Poweroff && data[2] == (byte)Message_Body_Command.Message_Command_Poweroff)
			{
				Console.WriteLine("Selected option: poweroff target.");
				if (data[3] == (byte)Message_Body_Command.Message_Command_Status_Success)
				{
					//Console.WriteLine("Command execution success.");
					cmdStatus = (byte)Message_Body_Command.Message_Command_Status_Success;
				}

				if (data[3] == (byte)Message_Body_Command.Message_Command_Status_Failure)
				{
					//Console.WriteLine("Command execution failure.");
					cmdStatus = (byte)Message_Body_Command.Message_Command_Status_Failure;
				}
			}
			else if (command == Message_Body_Command.Message_Data_Request_Temperature)
			{
				int totalChannels = data[2];
				Console.WriteLine("Total channels: {0}", data[2]);

				for(int i=0; i<totalChannels; i++)
				{

					Console.WriteLine("CH {0}#, Temp Value: {1}", data[3 + 2 * i], data[4 + 2 * i]);
				}

			}
			else if (command == Message_Body_Command.Message_Data_Request_SIMULATED_Temperature)
			{
				int totalChannels = data[2];
				Console.WriteLine("Total channels: {0}", data[2]);

				for(int i=0; i<totalChannels; i++)
				{

					Console.WriteLine("CH {0}#, Temperature Value: {1}", data[3 + 2 * i], data[4 + 2 * i]);
				}

			}
			else if (command == Message_Body_Command.Message_Data_Request_SIMULATED_Voltage)
			{
				int totalChannels = data[2];
				Console.WriteLine("Total channels: {0}", data[2]);

				for(int i=0; i<totalChannels; i++)
				{

					Console.WriteLine("CH {0}#, Direction: {1}, Voltage Value: {2}", data[3 + 3 * i], data[4 + 3*i], data[5 + 3 * i]);
				}

			}
			else if (command == Message_Body_Command.Message_Data_Request_SIMULATED_Current)
			{
				int totalChannels = data[2];
				Console.WriteLine("Total channels: {0}", data[2]);

				for(int i=0; i<totalChannels; i++)
				{

					Console.WriteLine("CH {0}#, Direction: {1}, Current Value: {2}", data[3 + 3 * i], data[4 + 3*i], data[5 + 3 * i]);
				}
			}
			else if (command == Message_Body_Command.Message_Data_Request_SIMULATED_CV)
			{
				int totalChannels = data[2];
				Console.WriteLine("Total channels: {0}", data[2]);

				for (int i = 0; i < totalChannels; i++)
				{

					Console.WriteLine("CH {0}#, Direction:{1},Voltage:{2}, Direction:{3},Current: {4}", data[3 + 5 * i], data[4 + 5 * i], data[5 + 5 * i], data[6 +5*i], data[7 + 5*i]);
				}

			}
			else
				cmdStatus = (byte)Message_Body_Command.Message_Command_None;
		}
			
		public static bool IsIPAddress(string ip)         
		{

			if (string.IsNullOrEmpty(ip) || ip.Length < 7 || ip.Length > 15) return false;

			string regformat = @"^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$";

			Regex regex = new Regex(regformat, RegexOptions.IgnoreCase);
			return regex.IsMatch(ip);
		}

		public static bool checkInputCmdSyntax(string cmdLine)
		{
			bool retBool = true;
			string [] cmdParts = cmdLine.Split(' ');

			// verify help command
			if (cmdParts [0] == "Help" || cmdParts [0] == "?" ) {
				if (cmdParts.Length == 1) { // only Connect without parameters
					//printCorrectCmdFormat("Connect");
					printCmdMainMenu();
					retBool = true;
				} else { 

					switch (cmdParts [1]) {
					case "Get":
						printCorrectCmdFormat("Get");
						break;
					case "Set":
						printCorrectCmdFormat("Set");
						break;
					case "Connect":
						printCorrectCmdFormat("Connect");
						break;
					case "Reboot":
						printCorrectCmdFormat("Reboot");
						break;
					case "Poweroff":
						printCorrectCmdFormat("Poweroff");
						break;
					case "Blist":
						printCorrectCmdFormat("Blist");
						break;
					default:
						break;
					}

				}

			}
		
			// verify connect command
			if (cmdParts [0] == "Connect" ) {
				if (cmdParts.Length == 1) { // only Connect without parameters
					printCorrectCmdFormat("Connect");
					retBool = false;
				} else { 
					
					bool flag = IsIPAddress (cmdParts [1]);
					if (!flag) {
						Console.WriteLine ("Error IP Addr.");
						retBool = false;
					}
					else
						// to save server ip address
						serverIP = cmdParts [1]; 
				}

			}
		
			// verify reboot command	
			if (cmdParts [0] == "Reboot") {
				
				string tempStr1 = "0x" + Convert.ToString (GlobalAutoTestID.mainControllerBoardNumber, 16);
				string tempStr2 = "0x" + Convert.ToString (GlobalAutoTestID.slaveFirstControllerBoardNumber, 16);
				string tempStr3 = "0x" + Convert.ToString (GlobalAutoTestID.slaveSecondControllerBoardNumber, 16);

			//	Console.WriteLine ("cmdParts[1]={0}, tempStr={1}", cmdParts[1], tempStr);
				if(!string.Equals( cmdParts[1], tempStr1)
					&& !string.Equals(cmdParts[1], tempStr2) 
					&& !string.Equals(cmdParts[1], tempStr3))
				{
					Console.WriteLine ("Wrong Target Board Type.");
					retBool = false;
				}
			}

			// verify poweroff command	
			if (cmdParts [0] == "Poweroff") {

				string tempStr1 = "0x" + Convert.ToString (GlobalAutoTestID.mainControllerBoardNumber, 16);
				string tempStr2 = "0x" + Convert.ToString (GlobalAutoTestID.slaveFirstControllerBoardNumber, 16);
				string tempStr3 = "0x" + Convert.ToString (GlobalAutoTestID.slaveSecondControllerBoardNumber, 16);

				//	Console.WriteLine ("cmdParts[1]={0}, tempStr={1}", cmdParts[1], tempStr);
				if(!string.Equals( cmdParts[1], tempStr1)
					&& !string.Equals(cmdParts[1], tempStr2) 
					&& !string.Equals(cmdParts[1], tempStr3))
				{
					Console.WriteLine ("Wrong Target Board Type.");
					retBool = false;
				}
			}

			// verify Get command
			if (cmdParts [0] == "Get") {
				if (cmdParts.Length != 4) { // less or more parameters
					printCorrectCmdFormat ("Get");
					retBool = false;
				} else { 
					if (cmdParts [1] == "V" || cmdParts [1] == "R") { 
							//byte channel = Byte.Parse(cmdParts[2], System.Globalization.NumberStyles.HexNumber);
							//int channel = Int32.Parse(cmdParts[2], System.Globalization.NumberStyles.HexNumber);
						int channel = Convert.ToInt32(cmdParts[2]);
						//Console.WriteLine("channel={0} ...", channel);
						if (channel >= 0 && channel <= 255) { // special 0xFF for all channels 
							if (cmdParts [3] == "A" || cmdParts [3] == "V" || cmdParts [3] == "C" || cmdParts [3] == "T") { 
								retBool = true;									
							} else { // wrong Voltage/Current/Temperature
								printCorrectCmdFormat ("Get");
								retBool = false;
							}
						} else { // wrong check channel number
							printCorrectCmdFormat ("Get");
							retBool = false;
						}
					} else { // wrong virtual mode or reality
						printCorrectCmdFormat ("Get");
						retBool = false;
					}
				}
			}

				
			return retBool;
				
		}

		public static void establishConnection_InitSession(string targetServerIP)
		{
			data = new byte[256];

			Console.WriteLine("Handshakes with server ...");

			client = new TcpClient(targetServerIP, port);

			// Read the first session message of the TcpServer ack bytes.
			sessionStream = client.GetStream();
			bytes = sessionStream.Read(data, 0, data.Length);

			// Analyze the server response message
			Console.WriteLine("Message head type of initial session: {0}", data[0]);
			Console.WriteLine("Message data length: {0}", data[1]);
			Console.WriteLine("Software version: {0}.{1}", data[3], data[2]);
			Console.WriteLine("Main controller number: {0}", data[4]);

			sessionStream.Close();
			client.Close();
		}

		public static void printHelpMenu()
		{
			Console.WriteLine ("######################################################################");
			Console.WriteLine ();
			Console.WriteLine ("######  Welcome to Baoxing CLI Console as testing interface.  #######");
			Console.WriteLine ();
			Console.WriteLine ("######################################################################");
			Console.WriteLine ();
			Console.WriteLine ("This is the interactive testing workbench from Baoxing Electric Group .");
			Console.WriteLine ();
		}
			
		public static void printCmdMainMenu()
		{
			Console.WriteLine ("Supported command list:");
			Console.WriteLine ();
			Console.WriteLine ("Autoip -- automatically acquire the server IP.");
			Console.WriteLine ("Poweroff -- poweroff the specified board.");
			Console.WriteLine ("Reboot   -- reboot the specified board.");
			Console.WriteLine ("Get      -- get the functional value.");
			Console.WriteLine ("Set      -- set the functional value.");
			Console.WriteLine ("Blist    -- list the main controller board and slave controller board supported in system.");
			Console.WriteLine ("Connect  -- connect with the main controller board.");
			Console.WriteLine ("Help     -- show the supported command list.");
			Console.WriteLine ();

			//Console.WriteLine ("Get [Function] [CH#] -- get the functional parameter value in the specified channel.");
		}

		public static void printCorrectCmdFormat(string cmdStr)
		{
			switch (cmdStr) {
			case "Blist":
				printBoardList();
				break;
			case "Autoip":
				negotiateBoardServer ();
				break;
			case "Connect":
				Console.WriteLine ("Connect [Server IP Addr]");
				break;
			case "Reboot":
				Console.WriteLine ("Reboot [Specified Target Board]");
				break;
			case "Poweroff":
				Console.WriteLine ("Poweroff [Specified Target Board]");
				break;
			case "Get":
				Console.WriteLine ("Get [V/R] [CH#] [V/C/T/A] ");
				Console.WriteLine ("V - Virtual value; R - Real value; 255 - All channels.");
				Console.WriteLine ("V - Voltage; C - Current; T - Board Temperature; A - All supported paramterized values.");
				break;
			case "Set":
				Console.WriteLine ("Set [Object]  [S/R]");
				Console.WriteLine ("Object: route0 - upstream communication channel; route1 - downstream communication channel.");
				Console.WriteLine ("S - Serial; R - RS485.");
				break;
			default:
				break;
			}
		}
		public static void negotiateBoardServer()
		{
			// initialize to prevent mis-judge
			helloBuffer = System.Text.Encoding.Default.GetBytes("BXGISNULL");
			blockingTimes = 0;
			// probe server existence
			bool retval = probeServer();
			if (retval)
			{
				System.Console.WriteLine ("The background server on board is already running.");
				// initialize the network TCP connections.
				//establishConnection_InitSession(serverIP);
				System.Console.WriteLine ("Connecting to master board service with success.");
				System.Console.WriteLine ("server ip ={0}",  rcvEndpoint.Address.ToString ());

			}
			else
			{
				System.Console.WriteLine ("The specified server of target board is not found.");
			}
			System.Console.WriteLine ();
		}
		public static void printBoardList()
		{
			Console.WriteLine ("Supported target board ID list,");
			Console.WriteLine ("ID 0x{0}: main controller board;", Convert.ToString(GlobalAutoTestID.mainControllerBoardNumber,16));
			Console.WriteLine ("ID 0x{0}, ID 0x{1}: slave controller board;", Convert.ToString(GlobalAutoTestID.slaveFirstControllerBoardNumber,16), 
				Convert.ToString(GlobalAutoTestID.slaveSecondControllerBoardNumber,16));
			Console.WriteLine ();
		}

		public static void Main (string[] args)
		{
			printHelpMenu ();

			string line;

			do { 
				
		            Console.Write(">>");
		            line = Console.ReadLine();

	                string [] parts = line.Split(' ');
			 
	                foreach (string word in  parts)
	                {
		                   Console.WriteLine(word);
		            }
			 
	                if (line != null) 
		            {        
							bool correctCmdFlag = false;
			                
							// help command
							if(parts[0] == "Help" || parts[0] == "?")
									checkInputCmdSyntax(line);
							else if(parts[0] == "Autoip")
							{
								negotiateBoardServer();
								//printBoardList();
							}        
							else if(parts[0] == "Blist")
							{
								printBoardList();
							}
							// get command
							else if(parts[0] == "Get")
							{
								correctCmdFlag = checkInputCmdSyntax(line);
								if(correctCmdFlag)
								{
									Console.WriteLine("Get command executed.");
									switch(parts [1])
									{
										case "V": // simulated
											if(parts[3] == "T") // temperature
											{
												byte mystatus;
												preSendCmdMessage();
												sendCmd_RequestMessage(Message_Body_Command.Message_Data_Request_SIMULATED_Temperature, GlobalAutoTestID.mainControllerBoardNumber);
												acquireCmd_ResponseMessage(Message_Body_Command.Message_Data_Request_SIMULATED_Temperature, out mystatus);
												if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Success)
													Console.WriteLine("Temperature Request Command Execution Success.");
												if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Failure)
													Console.WriteLine("Temperature Request Command Execution Failure.");
												postSendCmdMessage();
											}
											if(parts[3] == "C") // current
											{
												byte mystatus;
												preSendCmdMessage();
												sendCmd_RequestMessage(Message_Body_Command.Message_Data_Request_SIMULATED_Current, GlobalAutoTestID.mainControllerBoardNumber);
												acquireCmd_ResponseMessage(Message_Body_Command.Message_Data_Request_SIMULATED_Current, out mystatus);
												if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Success)
													Console.WriteLine("Voltage Request Command Execution Success.");
												if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Failure)
													Console.WriteLine("Voltage Request Command Execution Failure.");
												postSendCmdMessage();

											}
											if(parts[3] == "V")  // voltage
											{
												byte mystatus;
												preSendCmdMessage();
												sendCmd_RequestMessage(Message_Body_Command.Message_Data_Request_SIMULATED_Voltage, GlobalAutoTestID.mainControllerBoardNumber);
												acquireCmd_ResponseMessage(Message_Body_Command.Message_Data_Request_SIMULATED_Voltage, out mystatus);
												if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Success)
													Console.WriteLine("Voltage Request Command Execution Success.");
												if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Failure)
													Console.WriteLine("Voltage Request Command Execution Failure.");
												postSendCmdMessage();
											}
											if(parts[3] == "A")  // current and voltage
											{
												byte mystatus;
												preSendCmdMessage();
												sendCmd_RequestMessage(Message_Body_Command.Message_Data_Request_SIMULATED_CV, GlobalAutoTestID.mainControllerBoardNumber);
												acquireCmd_ResponseMessage(Message_Body_Command.Message_Data_Request_SIMULATED_CV, out mystatus);
												if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Success)
													Console.WriteLine("Command Execution Success.");
												if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Failure)
													Console.WriteLine("Command Execution Failure.");
												postSendCmdMessage();
											}
 											break;
										case "R": // real
											break;
										default:
											break;
									}
								}
							}
							// connect command
							else if(parts[0] == "Connect")
							{
									correctCmdFlag = checkInputCmdSyntax(line);
									if(correctCmdFlag)
											establishConnection_InitSession(parts[1]);
							}
							else if(parts[0] == "Reboot")
							{
									correctCmdFlag = checkInputCmdSyntax(line);
									if(correctCmdFlag)
									{
										byte mystatus;

										/* remove "0x"  */
										string  bdNumStr = parts[1].Substring(2, parts[1].Length-2); 

										byte bdNum = Byte.Parse(bdNumStr, System.Globalization.NumberStyles.HexNumber);

										Console.WriteLine("to reboot specified target.");
										preSendCmdMessage();
										//sendCmd_RebootTargetBoard();
										sendCmd_RequestMessage(Message_Body_Command.Message_Command_Reboot, bdNum);
										acquireCmd_ResponseMessage(Message_Body_Command.Message_Command_Reboot, out mystatus);
										if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Success)
											Console.WriteLine("Command Execution Success.");
										if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Failure)
											Console.WriteLine("Command Execution Failure.");
										postSendCmdMessage();
									}
							}
							else if(parts[0] == "Poweroff")
							{
								correctCmdFlag = checkInputCmdSyntax(line);
								if(correctCmdFlag)
								{
									byte mystatus;

									/* remove "0x"  */
									string  bdNumStr = parts[1].Substring(2, parts[1].Length-2); 

									byte bdNum = Byte.Parse(bdNumStr, System.Globalization.NumberStyles.HexNumber);

									Console.WriteLine("to power-off specified target.");
									preSendCmdMessage();
									//sendCmd_RebootTargetBoard();
									sendCmd_RequestMessage(Message_Body_Command.Message_Command_Poweroff, bdNum);
									acquireCmd_ResponseMessage(Message_Body_Command.Message_Command_Poweroff, out mystatus);
									if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Success)
										Console.WriteLine("Command Execution Success.");
									if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Failure)
										Console.WriteLine("Command Execution Failure.");
									postSendCmdMessage();
								}
							}
	                        else
				                    Console.WriteLine ("Error command format. to see help.");
		 
	                 }     


	       } while (line != null);   

		}
	}
}
