#include "help.h"
#include <sys_req.h>
#include <string.h>


// This file will be for the help command, listing out each of the commands Name, Synopsis and Description.
// Created by David Clark on 8/25/23

void help(void) {

	char first_line[] = "MPX Project Help Page\n\n";

	// Outputting first line
	sys_req(WRITE, COM1, first_line, strlen(first_line));

	// String for the name of Version method
	char version_name[] = "\033[0;32mName\n\tVersion\033[0;37m - Displays the current version of MPX as well as the date it was most recently updated\n\n";

	// Outputting the Name for the version command
	sys_req(WRITE, COM1, version_name, strlen(version_name));

	// String for the description of version command
	char version_description[] = "\033[0;34mDescription\n\t\033[0;37mThis command holds the main purpose of displaying the current working version of the MPX compiler.\n\tThe current version is expressed in the form of R#.? where # stands for the current module being worked on\n\tand ? stands for the numbered update to the development branch. For example version R3.2 would represent\n\tmodule 3 being currently worked on, and the second major commit to the Development branch.\n\n\n";

	// Outputting the Description for the version command
	sys_req(WRITE, COM1, version_description, strlen(version_description));

	// String for name of shutdown method
	char shutdown_name[] = "\033[0;32mName\n\tShutdown\033[0;37m - Will shut down the operating system\n\n";

	// Outputs the name for the shutdown command
	sys_req(WRITE, COM1, shutdown_name, strlen(shutdown_name));

	// String for description of shutdown command
	char shutdown_description[] = "\033[0;34mDescription\n\t\033[0;37mThis command will halt all processes currently running within the operating system followed by the\n\toperating system itself shutting down\n\n\n";

	// Outputs the description for shutdown command
	sys_req(WRITE, COM1, shutdown_description, strlen(shutdown_description));

	// String for name of the settime command
	char settime_name[] = "\033[0;32mName\n\tSetTime\033[0;37m - Will set the time on the operating system.\n\n";

	// Outputs the name for settime command
	sys_req(WRITE, COM1, settime_name, strlen(settime_name));

	// String for arguments of settime command
	char settime_args[] = "\033[0;36mArguments\n\t\033[0;37mThree user inputs of 'hh', 'mm', 'ss'\n\n";

	// Outputs the args for settime command
	sys_req(WRITE, COM1, settime_args, strlen(settime_args));

	// String for the description of settime command
	char settime_description[] = "\033[0;34mDescription\n\t\033[0;37mFirst user prompt will ask for the hours in the form of 'hh' (Ex. 13).\n\tThe second prompt will ask the user for the minutes in the form of 'mm' (Ex. 45). The final\n\tprompt will ask the user for the seconds in the form 'ss' (Ex. 29).\n\n\n";
	
	// Outputs the settime description
	sys_req(WRITE, COM1, settime_description, strlen(settime_description));
	
	// String for name of getTime command
	char gettime_name[] = "\033[0;32mName\n\tGetTime\033[0;37m - Will get the current time on the operating system.\n\n";
	
	// Outputs the name for gettime command
	sys_req(WRITE, COM1, gettime_name, strlen(gettime_name));
	
	// String for description of the gettime command
	char gettime_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will get the current time saved on the operating system, whether it's the default\n\ttime when launched or the changed time if the user has previously changed it and display it to the user.\n\n\n";
	
	// Outputs the description for gettime command
	sys_req(WRITE, COM1, gettime_desc, strlen(gettime_desc));
	
	// String for name of the setdate command
	char setdate_name[] = "\033[0;32mName\n\tSetDate\033[0;37m - Will set the date on the operating system.\n\n";

	// Outputs the name for setdate command
	sys_req(WRITE, COM1, setdate_name, strlen(setdate_name));

	// String for arguments of setdate command
	char setdate_args[] = "\033[0;36mArguments\n\t\033[0;37mThree user inputs of 'mm', 'dd', 'yy'\n\n";

	// Outputs the args for setdate command
	sys_req(WRITE, COM1, setdate_args, strlen(setdate_args));

	// String for the description of setdate command
	char setdate_description[] = "\033[0;34mDescription\n\t\033[0;37mFirst user prompt will ask for the month in the form of 'mm' (Ex. 05).\n\tThe second prompt will ask the user for the day in the form of 'dd' (Ex. 28). The final\n\tprompt will ask the user for the year in the form 'yy' (Ex. 05).\n\n\n";

	// Outputs the description of setdate command
	sys_req(WRITE, COM1, setdate_description, strlen(setdate_description));
	
	// String for name of getdate command
	char getdate_name[] = "\033[0;32mName\n\tGetDate\033[0;37m - Will get the current date on the operating system.\n\n";
	
	// Outputs the name for gettime command
	sys_req(WRITE, COM1, getdate_name, strlen(getdate_name));
	
	// String for description of the gettime command
	char getdate_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will get the current date saved on the operating system, whether it's the default\n\tdate when launched or the changed date if the user has previously changed it and display it to the user.\n\n\n";
	
	// Outputs the description for gettime command
	sys_req(WRITE, COM1, getdate_desc, strlen(getdate_desc));
	
	// Load R3
	char loadr3_name[] = "\033[0;32mName\n\tLoadR3\033[0;37m - Will load in processes from processes.h.\n\n";
	sys_req(WRITE, COM1, loadr3_name, strlen(loadr3_name));
	char loadr3_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command takes no user input and will load in any processes within the processes.h file\n\tand will load and queue them into a ready state. It will also save the context for each process.\n\n\n";
	sys_req(WRITE, COM1, loadr3_desc, strlen(loadr3_desc));
	
	// Delete PCB
	char deletePCB_name[] = "\033[0;32mName\n\tDeletePCB\033[0;37m - Will delete a PCB based on name given by user.\n\n";
	sys_req(WRITE, COM1, deletePCB_name, strlen(deletePCB_name));
	char deletePCB_args[] = "\033[0;36mArguments\n\t\033[0;37mUser input of the name of the PCB to be deleted\n\n";
	sys_req(WRITE, COM1, deletePCB_args, strlen(deletePCB_args));
	char deletePCB_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will take in user input for a name and will remove the PCB with that given name\n\tfrom the system (name must be of a PCB that exists).\n\n\n";
	sys_req(WRITE, COM1, deletePCB_desc, strlen(deletePCB_desc));
	
	// Block PCB
	char blockPCB_name[] = "\033[0;32mName\n\tBlockPCB\033[0;37m - Will move a PCB based on name to the blocked state given by user.\n\n";
	sys_req(WRITE, COM1, blockPCB_name, strlen(blockPCB_name));
	char blockPCB_args[] = "\033[0;36mArguments\n\t\033[0;37mUser input of the name of the PCB to be moved to blocked\n\n";
	sys_req(WRITE, COM1, blockPCB_args, strlen(blockPCB_args));
	char blockPCB_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will take in user input for a name and will move the PCB with that given name\n\tinto the blocked state (PCB can't be a system process and must not be in blocked state).\n\n\n";
	sys_req(WRITE, COM1, blockPCB_desc, strlen(blockPCB_desc));
	
	// Unblock PCB
	char unblockPCB_name[] = "\033[0;32mName\n\tUnblockPCB\033[0;37m - Will move a PCB based on name to the unblocked/ready state given by user.\n\n";
	sys_req(WRITE, COM1, unblockPCB_name, strlen(unblockPCB_name));
	char unblockPCB_args[] = "\033[0;36mArguments\n\t\033[0;37mUser input of the name of the PCB to be moved to unblocked/ready\n\n";
	sys_req(WRITE, COM1, unblockPCB_args, strlen(unblockPCB_args));
	char unblockPCB_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will take in user input for a name and will move the PCB with that given name\n\tinto the unblocked/ready state (PCB must be in blocked state).\n\n\n";
	sys_req(WRITE, COM1, unblockPCB_desc, strlen(unblockPCB_desc));
	
	// Suspend PCB
	char suspendPCB_name[] = "\033[0;32mName\n\tSuspendPCB\033[0;37m - Will move a PCB based on name to the suspended state given by user.\n\n";
	sys_req(WRITE, COM1, suspendPCB_name, strlen(suspendPCB_name));
	char suspendPCB_args[] = "\033[0;36mArguments\n\t\033[0;37mUser input of the name of the PCB to be moved to suspended\n\n";
	sys_req(WRITE, COM1, suspendPCB_args, strlen(suspendPCB_args));
	char suspendPCB_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will take in user input for a name and will move the PCB with that given name\n\tinto the suspended state (PCB can't be a system process).\n\n\n";
	sys_req(WRITE, COM1, suspendPCB_desc, strlen(suspendPCB_desc));
	
	// Resume PCB
	char resumePCB_name[] = "\033[0;32mName\n\tResumePCB\033[0;37m - Will move a PCB based on name out of the suspended state given by user.\n\n";
	sys_req(WRITE, COM1, resumePCB_name, strlen(resumePCB_name));
	char resumePCB_args[] = "\033[0;36mArguments\n\t\033[0;37mUser input of the name of the PCB to be moved out of suspended\n\n";
	sys_req(WRITE, COM1, resumePCB_args, strlen(resumePCB_args));
	char resumePCB_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will take in user input for a name and will move the PCB with that given name\n\tout of the suspended state.\n\n\n";
	sys_req(WRITE, COM1, resumePCB_desc, strlen(resumePCB_desc));
	
	//  Set Priority
	char priorityPCB_name[] = "\033[0;32mName\n\tSetPriority\033[0;37m - Will change the priority of a PCB given by the user.\n\n";
	sys_req(WRITE, COM1, priorityPCB_name, strlen(priorityPCB_name));
	char priorityPCB_args[] = "\033[0;36mArguments\n\t\033[0;37mUser input of the name of the PCB to change as well as the new priority (0-9)\n\n";
	sys_req(WRITE, COM1, priorityPCB_args, strlen(priorityPCB_args));
	char priorityPCB_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will take in user input for a name as well as the priority value that the PCB\n\tshould be changed to and will make that update (name must be a valid PCB that exists and priority must be between 0-9).\n\n\n";
	sys_req(WRITE, COM1, priorityPCB_desc, strlen(priorityPCB_desc));
	
	// Show PCB
	char showPCB_name[] = "\033[0;32mName\n\tShowPCB\033[0;37m - Will display the PCB with the given name that was provided by the user.\n\n";
	sys_req(WRITE, COM1, showPCB_name, strlen(showPCB_name));
	char showPCB_args[] = "\033[0;36mArguments\n\t\033[0;37mUser input of the name of the PCB to display to the user\n\n";
	sys_req(WRITE, COM1, showPCB_args, strlen(showPCB_args));
	char showPCB_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will take in user input for a name of the PCB that will be displayed to the\n\tuser, including the information associated with the PCB (name must be a valid name of a PCB that exists).\n\n\n";
	sys_req(WRITE, COM1, showPCB_desc, strlen(showPCB_desc));
	
	//  Show Ready
	char ready_name[] = "\033[0;32mName\n\tShowReady\033[0;37m - Will display all the PCBs that are currently in the ready queue.\n\n";
	sys_req(WRITE, COM1, ready_name, strlen(ready_name));
	char ready_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will display all of the PCBs that have been created that are currently within\n\tthe ready queue in the system.\n\n\n";
	sys_req(WRITE, COM1, ready_desc, strlen(ready_desc));
	
	//  Show Blocked
	char blocked_name[] = "\033[0;32mName\n\tShowBlocked\033[0;37m - Will display all the PCBs that are currently in the blocked queue.\n\n";
	sys_req(WRITE, COM1, blocked_name, strlen(blocked_name));
	char blocked_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will display all of the PCBs that have been created that are currently within\n\tthe blocked queue in the system.\n\n\n";
	sys_req(WRITE, COM1, blocked_desc, strlen(blocked_desc));
	
	//  Show All
	char all_name[] = "\033[0;32mName\n\tShowAll\033[0;37m - Will display all the PCBs that are currently in the system.\n\n";
	sys_req(WRITE, COM1, all_name, strlen(all_name));
	char all_desc[] = "\033[0;34mDescription\n\t\033[0;37mThis command will display all of the PCBs that have been created that are currently within\n\teither the ready queue or the blocked queue in the system.\n\n\n";
	sys_req(WRITE, COM1, all_desc, strlen(all_desc));
}
