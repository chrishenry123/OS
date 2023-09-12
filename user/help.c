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
	char version_name[] = "Name\n\tVersion - Displays the current version of MPX as well as the date it was most recently updated\n";

	// Outputting the Name for the version command
	sys_req(WRITE, COM1, version_name, strlen(version_name));

	// String for the description of version command
	char version_description[] = "Description\n\tThis command holds the main purpose of displaying the current working version of the MPX compiler.\n\tThe current version is expressed in the form of R#.? where # stands for the current module being worked on\n\tand ? stands for the numbered update to the development branch. For example version R3.2 would represent\n\tmodule 3 being currently worked on, and the second major commit to the Development branch.\n\n";

	// Outputting the Description for the version command
	sys_req(WRITE, COM1, version_description, strlen(version_description));

	// String for name of shutdown method
	char shutdown_name[] = "Name\n\tShutdown - Will shut down the operating system\n";

	// Outputs the name for the shutdown command
	sys_req(WRITE, COM1, shutdown_name, strlen(shutdown_name));

	// String for description of shutdown command
	char shutdown_description[] = "Description\n\tThis command will halt all processes currently running within the operating system followed by the\n\toperating system itself shutting down\n\n";

	// Outputs the description for shutdown command
	sys_req(WRITE, COM1, shutdown_description, strlen(shutdown_description));

	// String for name of the settime command
	char settime_name[] = "Name\n\tSetTime - Will set the time on the operating system.\n";

	// Outputs the name for settime command
	sys_req(WRITE, COM1, settime_name, strlen(settime_name));

	// String for arguments of settime command
	char settime_args[] = "Arguments\n\tArguments - Three user inputs of 'hh', 'mm', 'ss'\n";

	// Outputs the args for settime command
	sys_req(WRITE, COM1, settime_args, strlen(settime_args));

	// String for the description of settime command
	char settime_description[] = "Description\n\tFirst user prompt will ask for the hours in the form of 'hh' (Ex. 13).\n\tThe second prompt will ask the user for the minutes in the form of 'mm' (Ex. 45). The final\n\tprompt will ask the user for the seconds in the form 'ss' (Ex. 29).\n\n";
	
	// Outputs the settime description
	sys_req(WRITE, COM1, settime_description, strlen(settime_description));
	
	// String for name of getTime command
	char gettime_name[] = "Name\n\tGetTime - Will get the current time on the operating system.\n";
	
	// Outputs the name for gettime command
	sys_req(WRITE, COM1, gettime_name, strlen(gettime_name));
	
	// String for description of the gettime command
	char gettime_desc[] = "Description\n\tThis command will get the current time saved on the operating system, whether it's the default\n\ttime when launched or the changed time if the user has previously changed it and display it to the user.\n\n";
	
	// Outputs the description for gettime command
	sys_req(WRITE, COM1, gettime_desc, strlen(gettime_desc));
	
	// String for name of the setdate command
	char setdate_name[] = "Name\n\tSetDate - Will set the date on the operating system.\n";

	// Outputs the name for setdate command
	sys_req(WRITE, COM1, setdate_name, strlen(setdate_name));

	// String for arguments of setdate command
	char setdate_args[] = "Arguments\n\tArguments - Three user inputs of 'mm', 'dd', 'yy'\n";

	// Outputs the args for setdate command
	sys_req(WRITE, COM1, setdate_args, strlen(setdate_args));

	// String for the description of setdate command
	char setdate_description[] = "Description\n\tFirst user prompt will ask for the month in the form of 'mm' (Ex. 05).\n\tThe second prompt will ask the user for the day in the form of 'dd' (Ex. 28). The final\n\tprompt will ask the user for the year in the form 'yy' (Ex. 05).\n\n";

	// Outputs the description of setdate command
	sys_req(WRITE, COM1, setdate_description, strlen(setdate_description));
	
	// String for name of getdate command
	char getdate_name[] = "Name\n\tGetDate - Will get the current date on the operating system.\n";
	
	// Outputs the name for gettime command
	sys_req(WRITE, COM1, getdate_name, strlen(getdate_name));
	
	// String for description of the gettime command
	char getdate_desc[] = "Description\n\tThis command will get the current date saved on the operating system, whether it's the default\n\tdate when launched or the changed date if the user has previously changed it and display it to the user.\n\n";
	
	// Outputs the description for gettime command
	sys_req(WRITE, COM1, getdate_desc, strlen(getdate_desc));
}
