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
        public static void sendCmd_RebootTargetBoard(NetworkStream sessionStream)
        {
            Byte[] data = new Byte[GlobalAutoTestID.cmdMessage_RebootTotalLength + 1];

            //string message = "This is macbook client side.";
            // Translate the passed message into ASCII and store it as a Byte array.
            //Byte[] data = System.Text.Encoding.ASCII.GetBytes(message);
            // Buffer to store the response bytes.
            //data = new Byte[256];

#if false
            /* headMsg */
            data[0] = Msg_Head_0_0.Message_Head_0_0_RequestMessageFlag;
            data[0] |= Msg_Head_0_0.Message_Head_0_0_TargetTypeFlag;
            data[0] |= Msg_Head_0_0.Message_Head_0_0_SystemCommandFlag;

            /* fill in fields of body message  */
            data[1] = GlobalAutoTestID.mainControllerBoardNumber;
            data[2] = Message_Body_Command.Message_Command_Reboot;
#endif

            // Send the message to the connected TcpServer. 
            sessionStream.Write(data, 0, data.Length);
            Console.WriteLine("Sent: Reboot Command finished.");
        }


        static void Main(string[] args)
        {
            NetworkStream sessionStream = null;
            TcpClient client = null;
            Int32 bytes;

            Console.WriteLine("/********************************************************************/");

            Console.WriteLine("**************** Command Line Console on PC Unit ********************");

            Console.WriteLine("/********************************************************************/");
            Console.WriteLine();


            try
            {
                // Create a TcpClient.
                // Note, for this client to work you need to have a TcpServer 
                // connected to the same address as specified by the server, port
                // combination.
                Int32 port = 13000;
                string message= "This is macbook client side.";
                
                // Translate the passed message into ASCII and store it as a Byte array.
                Byte[] data = System.Text.Encoding.ASCII.GetBytes(message);

                while (true)
                {
                    client = new TcpClient("192.168.100.84", port);
                    // Get a client stream for reading and writing.
                    //  Stream stream = client.GetStream();
                   

                    Console.WriteLine("Connecting to server ... ...");
                    Console.WriteLine();
                    Console.WriteLine();


                    sessionStream = client.GetStream();

                    // Analyze the server response message
                    /////////////////////////////////////////////
           
                    // Buffer to store the response bytes.
                    data = new Byte[256];
                    
                    // Read the first session message of the TcpServer ack bytes.
                    bytes = sessionStream.Read(data, 0, data.Length);
                    Console.WriteLine("Message head type of initial session: {0}",   data[0]);
                    Console.WriteLine("Message data length: {0}", data[1]);
                    Console.WriteLine("Software version: {0}.{1}", data[3], data[2]);
                    Console.WriteLine("Main controller number: {0}", data[4]);
                    Console.WriteLine();

                    /////////////////////////////////////////////


                    // Send the message to the connected TcpServer. 
                    sessionStream.Write(data, 0, data.Length);

                    Console.WriteLine("Sent: {0}", message);

                    // Receive the TcpServer.response.

                    // String to store the response ASCII representation.
                    String responseData = String.Empty;
                    // Buffer to store the response bytes.
                    data = new Byte[256];

                    // Read the first batch of the TcpServer response bytes.
                    bytes = sessionStream.Read(data, 0, data.Length);
                    responseData = System.Text.Encoding.ASCII.GetString(data, 0, bytes);
                    Console.WriteLine("Received: {0}", responseData);

                    Console.WriteLine("\n Press Enter to continue...");
                    Console.WriteLine();
                    Console.WriteLine();
                    Console.Read();

                    // Close everything.
                    if (sessionStream != null)
                        sessionStream.Close();
                    if (client != null)
                        client.Close();

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
