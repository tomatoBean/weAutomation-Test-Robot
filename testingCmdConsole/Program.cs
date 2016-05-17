using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace clientProtocolDefinition
{
    class Program
    {

        public const string serverIP = "192.168.100.88";
        public const Int32 port = 13000;
        public static NetworkStream sessionStream = null;
        public static TcpClient client = null;
        public static byte[] data = null;
        public static Int32 bytes; 

        public static void establishConnection_InitSession()
        {
            data = new byte[256];
            Console.WriteLine("Handshakes with server ...");
            client = new TcpClient(serverIP, port);

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
        public static void sendCmd_RequestMessage(Message_Body_Command command)
        {

            data = new byte[GlobalAutoTestID.cmdMessage_RebootTotalLength + 1];

            if (command == Message_Body_Command.Message_Command_Reboot)
            {

                /* headMsg */
                data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
                data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
                data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_SystemCommandFlag;

                /* fill in fields of body message  */
                data[1] = GlobalAutoTestID.mainControllerBoardNumber;
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
                data[1] = GlobalAutoTestID.mainControllerBoardNumber;
                data[2] = (byte)Message_Body_Command.Message_Command_Poweroff;
                data[3] = 0;
                data[4] = 0;

                sessionStream.Write(data, 0, data.Length);
                Console.WriteLine("Sent: Poweroff Command finished.");
            }

        }

        public static void acquireCmd_ResponseMessage(Message_Body_Command command, out byte cmdStatus)
        {
            // Read the first session message of the TcpServer ack bytes.
            bytes = sessionStream.Read(data, 0, data.Length);
            // Analyze the server response message
            Console.WriteLine("Command Message head type: {0}", data[0]);

            cmdStatus = 0;
            if (command == Message_Body_Command.Message_Command_Reboot && data[2] == (byte)Message_Body_Command.Message_Command_Reboot)
            {
                if(data[3] == (byte)Message_Body_Command.Message_Command_Status_Success)
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
            else
                cmdStatus= (byte)Message_Body_Command.Message_Command_None;
        }

        static void Main(string[] args)
        {
         
            Console.WriteLine("********************************************************************");

            Console.WriteLine("**************** Command Line Console on PC Unit ********************");

            Console.WriteLine("********************************************************************");
            Console.WriteLine();

            try
            {

                establishConnection_InitSession();
               // sendCmd_RebootTargetBoard();
                Console.Read();


                while (true)
                {
                    string inputStr;
                    int option;

                    Console.WriteLine("*****************************************************");
                    Console.WriteLine("****************    Command Set Menu   **************");
                    Console.WriteLine("*****************************************************");
                    Console.WriteLine("1. Reboot; 2. Poweroff;");

                    Console.Write("Command Message Option： ");
                    Console.Read();
                    inputStr = Console.ReadLine();
                    Console.WriteLine("Your choice： {0}", inputStr);

#if true

                    int.TryParse(inputStr, out option);
                    byte mystatus;

                    switch (option)
                    {
                        case 1:
                            Console.WriteLine("Selected option: reboot target.");
                            preSendCmdMessage();
                            //sendCmd_RebootTargetBoard();
                            sendCmd_RequestMessage(Message_Body_Command.Message_Command_Reboot);
                            acquireCmd_ResponseMessage(Message_Body_Command.Message_Command_Reboot, out mystatus);
                            if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Success)
                                Console.WriteLine("Command Execution Success.");
                            if(mystatus == (byte)Message_Body_Command.Message_Command_Status_Failure)
                                Console.WriteLine("Command Execution Failure.");
                            postSendCmdMessage();
                            break;
                        case 2:

                            Console.WriteLine("Selected option: poweroff target.");
                            preSendCmdMessage();
                            //sendCmd_RebootTargetBoard();
                            sendCmd_RequestMessage(Message_Body_Command.Message_Command_Poweroff);
                            acquireCmd_ResponseMessage(Message_Body_Command.Message_Command_Poweroff, out mystatus);
                            if (mystatus == (byte)Message_Body_Command.Message_Command_Status_Success)
                                Console.WriteLine("Command Execution Success.");
                            if (mystatus == (byte)Message_Body_Command.Message_Command_Status_Failure)
                                Console.WriteLine("Command Execution Failure.");
                            postSendCmdMessage();
                            break;
                        default:
                            break;
                    }
#endif

                    Console.WriteLine("\n Press Enter to continue...");
                    Console.WriteLine();
                    Console.WriteLine();
                    Console.Read();

                    
                }
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
            }
            finally
            {
                // Close everything.
                if(sessionStream != null)
                    sessionStream.Close();
                if(client != null)
                    client.Close();
            }

            Console.WriteLine("\n Press Enter to continue...");
            Console.Read();
            
        }
    }
}
