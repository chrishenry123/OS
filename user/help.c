#include "help.h"
#include <sys_req.h>
#include <string.h>


// This file will be for the help command, listing out each of the commands Name, Synopsis and Description.
// Created by David Clark on 8/25/23

void help() {

	char first_line[] = "MPX Project Help Page\n\n";

	// Outputting first line
	sys_req(WRITE, COM1, first_line, strlen(first_line));

	// String for the name of Version method
	char version_name[] = "Name\n\tVersion - Displays the current version of MPX as well as the date it was most recently updated\n";

	// Outputting the Name for the version command
	sys_req(WRITE, COM1, version_name, strlen(version_name));

	// String for the description of version command
	char version_description[] = "Description\n\tThis command holds the main purpose of displaying the current working version of the MPX compiler. The current version is expressed in the form of R#.? where # stands for the current module being worked on and ? stands for the numbered update to the development branch. For example version R3.2 would represent module 3 being currently worked on, and the second major commit to the Development branch.\n\n";

	// Outputting the Description for the version command
	sys_req(WRITE, COM1, version_description, strlen(version_description));

	// String for name of shutdown method
	char shutdown_name[] = "Name\n\tShutdown - Will shut down the operating system\n";

	// Outputs the name for the shutdown command
	sys_req(WRITE, COM1, shutdown_name, strlen(shutdown_name));

	// String for description of shutdown command
	char shutdown_description[] = "Description\n\tThis command will halt all processes currently running within the operating system followed by the operating system itself shutting down\n\n";

	// Outputs the description for shutdown command
	sys_req(WRITE, COM1, shutdown_description, strlen(shutdown_description));

	// String for name of the settime command
	char settime_name[] = "Name\n\tSetTime - Will set the time on the operating system\n";

	// Outputs the name for settime command
	sys_req(WRITE, COM1, settime_name, strlen(settime_name));

	// String for arguments of settime command
	char settime_args[] = "Arguments\n\tArguments - MM-DD-YR HR:MM:SS\n";

	// Outputs the args for settime command
	sys_req(WRITE, COM1, settime_args, strlen(settime_args));

	// String for the description of settime command
	char settime_description[] = "Description\n\tThis command will allow the user to input a time (date?) that will change the current clock within the MPX operating system.\n\n";

	// Outputs the description of settime command
	sys_req(WRITE, COM1, settime_description, strlen(settime_description));
}
