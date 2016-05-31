using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace autoTestCmdCtrlConsole
{
	
	class MainClass
	{
		public static void printMenu()
		{
			Console.WriteLine ("Supported command list:");
			Console.WriteLine ();
			Console.WriteLine ("Poweroff -- poweroff the specified board.");
			Console.WriteLine ("Reboot   -- reboot the specified board.");
			Console.WriteLine ("Get      -- get the functional value.");
			Console.WriteLine ("Set      -- set the functional value.");
			Console.WriteLine ("Board    -- list the main controller board and slave controller board supported in system.");
			Console.WriteLine ("Connect  -- connect the main controller board.");
			Console.WriteLine ("Help     -- show the supported command list.");
			Console.WriteLine ();
			//Console.WriteLine ("Get [Function] [CH#] -- get the functional parameter value in the specified channel.");
		}
		public static void Main (string[] args)
		{
			Console.WriteLine ("Welcome Baoxing CLI Console as testing interface.");

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
			                //Console.WriteLine("      " + line);
		                    if(line == "help" || line == "?")
			                           printMenu();
	                        else
				                       Console.WriteLine ("Error command format. to see help.");
		 
	                 }     
	       } while (line != null);   
			


		}
	}
}
