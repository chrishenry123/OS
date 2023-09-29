#include <pcbuser.h>
#include <pcb.h>
#include <string.h>
#include <sys_req.h>
#include <stdlib.h>


void createPCB() {
	char name[50] = {0};
//	int class = 0;
//	int priority = 0;
	
	// NAME LOOP
	while(1) {
		// Prompts user for PCB name
		char askForName[] = "Please enter the PCB name: ";
		sys_req(WRITE, COM1, askForName, strlen(askForName));
		
		// Reads in user response
		int userIn = sys_req(READ, COM1, name, strlen(name));
		sys_req(WRITE, COM1, name, userIn);
		
		// Checks to see if the name is too long
		if(strlen(name) > 10) {
			char nameMsg[] = "Name entered is greater than 10 characters.\n";
			sys_req(WRITE, COM1, nameMsg, strlen(nameMsg));
			continue;
		}
		
		// Checks to see if the name already exists
		if(pcb_find(name) != NULL) {
			char nameMsg[] = "Name entered is already in use.\n";
			sys_req(WRITE, COM1, nameMsg, strlen(nameMsg));
			continue;
		}

		// If we make it here name is fine and exit the loop
		break;
	}
	
	// Class loop
	while(1) {
		// Prompts user for priority level
		char askForPriority[] = "Please enter the priority level";
		sys_req(WRITE, COM1, askForPriority, strlen(askForPriority));
		
	}
	
}

void deletePCB() {
	char name[50] = {0};
	
	// NAME LOOP
	while(1) {
		// Prompts user for PCB name
		char askForName[] = "Please enter the PCB name: ";
		sys_req(WRITE, COM1, askForName, strlen(askForName));
		
		// Reads in user response
		int userIn = sys_req(READ, COM1, name, strlen(name));
		sys_req(WRITE, COM1, name, userIn);
		
		// Checks to see if the name exists
		if(pcb_find(name) == NULL) {
			char nameMsg[] = "Name entered does not exist.\n";
			sys_req(WRITE, COM1, nameMsg, strlen(nameMsg));
			continue;
		}
	}
	
	pcb_remove(pcb_find(name));
	sys_req(WRITE, COM1, "PCB deleted", strlen("PCB deleted"));
}

void blockPCB() {

}

void unblockPCB() {

}

void suspendPCB() {

}


