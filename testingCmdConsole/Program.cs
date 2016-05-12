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

        public const string serverIP = "192.168.100.86";
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

        public static void sendCmd_RebootTargetBoard()
        {
          
            data = new byte[GlobalAutoTestID.cmdMessage_RebootTotalLength + 1];

            Console.WriteLine("Connection with server for reboot command sesssion.");
            client = new TcpClient(serverIP, port);

            /* headMsg */
            data[0] = (byte)Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
            data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
            data[0] |= (byte)Msg_Head_0_0.Message_Head_0_0_SystemCommandFlag;

            /* fill in fields of body message  */
            data[1] = GlobalAutoTestID.mainControllerBoardNumber;
            data[2] = (byte)Message_Body_Command.Message_Command_Reboot;
            data[3] = 0;
            data[4] = 0;

            // Send the message to the connected TcpServer. 
            sessionStream = client.GetStream();
            sessionStream.Write(data, 0, data.Length);
            Console.WriteLine("Sent: Reboot Command finished.");

            sessionStream.Close();
            client.Close();
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

                    switch (option)
                    {
                        case 1:
                            Console.WriteLine("Selected option: reboot target.");
                            sendCmd_RebootTargetBoard();
                            break;
                        case 2:
                            Console.WriteLine("Selected option: poweroff target.");
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
