#include "pcbuser.h"
#include "pcb.h"
#include "time.h"
#include <string.h>
#include <sys_req.h>
#include <stdlib.h>
#include <pcb.h>


void createPCB() {
	char name[50] = {0};
	int pcbClass = 0;
	int priority = 0;
	
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
	
	// CLASS LOOP
	while(1) {
		// Prompts user for the class
		char classMsg[] = "Please enter the class of the PCB: User-0, System-1. ";
		sys_req(WRITE, COM1, classMsg, strlen(classMsg));
		
		// Takes in user input
		char classIn [50] = {0};
		char userIn = sys_req(READ, COM1, classIn, strlen(classIn));
		sys_req(WRITE, COM1, classIn, userIn);
		
		// Checks if 1 or 0 was input
		if(strcmp(classIn, "0") == 0 || strcmp(classIn, "1") == 0) {
			// If we get here then user input was fine and we can break out
			pcbClass = atoi(classIn);
			break;
		}
		
		// If we are here then the user input wasn't right and we need to reprompt
		char errorMsg[] = "User input doesn't match 0 (user) or 1 (system), please try again.\n";
		sys_req(WRITE, COM1, errorMsg, strlen(errorMsg));
	}
	
	// PRIORITY LOOP
	while(1) {
		// Prompts user for priority level
		char askForPriority[] = "Please enter the priority level";
		sys_req(WRITE, COM1, askForPriority, strlen(askForPriority));
		
		// Takes in user input
		char priorityIn [50] = {0};
		char userIn = sys_req(READ, COM1, priorityIn, strlen(priorityIn));
		sys_req(WRITE, COM1, priorityIn, userIn);
		
		// Changes user input to an integer
		priority = atoi(priorityIn);
		
		// Checks if input is valid
		if(priority >= 0 && priority <= 9) {
			// If here we are fine and can break out
			break;
		}
		// If we make it here the input is wrong and reprompt
		char errorMsg[] = "User input is not between 0 and 9, please try again.\n";
		sys_req(WRITE, COM1, errorMsg, strlen(errorMsg));
	}
	
	// Calls the kernel methods to make and insert the pcb
	struct pcb* userPCB = pcb_setup(name, pcbClass, priority);
	pcb_insert(userPCB);
}

void deletePCB() {
	char name[50] = {0};
	
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
		return;
	}
		
	// Checks to see if the name is a system process
	if(pcb_find(name)->class == 1) {
		char nameMsg[] = "Name entered is a system process and can't be deleted.\n";
		sys_req(WRITE, COM1, nameMsg, strlen(nameMsg));
		return;
	}
	
	// If we make it here then we have an ok name so we can remove it	
	pcb_remove(pcb_find(name));
	sys_req(WRITE, COM1, "PCB deleted", strlen("PCB deleted"));
}

void blockPCB() {
	char name[50] = {0};
	
	// Prompts user for PCB name
	char askForName[] = "Please enter the PCB name: ";
	sys_req(WRITE, COM1, askForName, strlen(askForName));
	
	// Reads in user response
	int userIn = sys_req(READ, COM1, name, strlen(name));
	sys_req(WRITE, COM1, name, userIn);
	
	// Checks if the PCB exists
	if(pcb_find(name) == NULL) {
		char nameMsg[] = "Name entered does not exist.\n";
		sys_req(WRITE, COM1, nameMsg, strlen(nameMsg));
		return;
	}
	else {
		// Find the PCB and remove it from current queue
		struct pcb* blockPCB = pcb_find(name);
		pcb_remove(blockPCB);
		
		// Changes exec_state to blocked
		blockPCB->exec_state = 1;
		
		// Re-inserts the PCB to appropriate queue
		pcb_insert(blockPCB);
	}
}

void unblockPCB() {
	char name[50] = {0};
	
	// Prompts user for PCB name
	char askForName[] = "Please enter the PCB name: ";
	sys_req(WRITE, COM1, askForName, strlen(askForName));
	
	// Reads in user response
	int userIn = sys_req(READ, COM1, name, strlen(name));
	sys_req(WRITE, COM1, name, userIn);
	
	// Checks if the PCB exists
	if(pcb_find(name) == NULL) {
		char nameMsg[] = "Name entered does not exist.\n";
		sys_req(WRITE, COM1, nameMsg, strlen(nameMsg));
		return;
	}
	else {
		// Find the PCB and remove it from current queue
		struct pcb* unblockPCB = pcb_find(name);
		pcb_remove(unblockPCB);
		
		// Changes exec_state to unblocked/ready
		unblockPCB->exec_state = 0;
		
		// Re-inserts the PCB to appropriate queue
		pcb_insert(unblockPCB);
	}
}

void suspendPCB() {
	char name[50] = {0};
	
	// Prompts user for PCB name
	char askForName[] = "Please enter the PCB name: ";
	sys_req(WRITE, COM1, askForName, strlen(askForName));
	
	// Reads in user response
	int userIn = sys_req(READ, COM1, name, strlen(name));
	sys_req(WRITE, COM1, name, userIn);
	
	// Checks if the PCB exists
	if(pcb_find(name) == NULL) {
		char nameMsg[] = "Name entered does not exist.\n";
		sys_req(WRITE, COM1, nameMsg, strlen(nameMsg));
		return;
	}
	// Check if the process is a system process
	else if(pcb_find(name)->class == 1) {
		char sysMsg[] = "A system process can't be suspended.\n";
		sys_req(WRITE, COM1, sysMsg, strlen(sysMsg));
		return;
	}
	else {
		// Find the PCB and remove it from current queue
		struct pcb* suspendPCB = pcb_find(name);
		pcb_remove(suspendPCB);
		
		// Changes disp_state to suspended
		suspendPCB->disp_state = 1;
		
		// Re-inserts the PCB to appropriate queue
		pcb_insert(suspendPCB);
	}
}

void resume_pcb(){
    char name[50] = {0};

    char askForRName[] = "Please enter the PCB you wish to resume: ";
    sys_req(WRITE, COM1, askForRName, strlen(askForRName));

    // Reads in user response
    int userIn = sys_req(READ, COM1, name, strlen(name));
    sys_req(WRITE, COM1, name, userIn);

    // Checks if the PCB exists
    if(pcb_find(name) == NULL) {
        char nameMsg[] = "Name entered does not exist.\n";
        sys_req(WRITE, COM1, nameMsg, strlen(nameMsg));
        return;
    }

    else {
        // Find the PCB and remove it from current queue
        struct pcb* resumePCB = pcb_find(name);
        pcb_remove(resumePCB);

        // Changes disp_state to not suspended
        resumePCB->disp_state = 0;

        // Re-inserts the PCB to appropriate queue
        pcb_insert(resumePCB);
    }


}

void set_priority(){
    char name[50] = {0};
    int priority = 0;

    // Prompts user for PCB name
    char askForName[] = "Please enter the PCB you which to set priority: ";
    sys_req(WRITE, COM1, askForName, strlen(askForName));


    // Reads in user response
    int userIn = sys_req(READ, COM1, name, strlen(name));
    sys_req(WRITE, COM1, name, userIn);

    // Checks if the PCB exists
    if(pcb_find(name) == NULL) {
        char nameMsg[] = "Name entered does not exist.\n";
        sys_req(WRITE, COM1, nameMsg, strlen(nameMsg));
        return;
    }


    // Prompts user for priority level
    char askForPriority[] = "Please enter the priority level";
    sys_req(WRITE, COM1, askForPriority, strlen(askForPriority));

    // Takes in user input
    char priorityIn [50] = {0};
    char userInP = sys_req(READ, COM1, priorityIn, strlen(priorityIn));
    sys_req(WRITE, COM1, priorityIn, userInP);

    // Changes user input to an integer
    priority = atoi(priorityIn);

    struct pcb* priorityPCB = pcb_find(name);
    priorityPCB->disp_state = priority;

}

void show_pcb(){

    char name[50] = {0};

    // Prompts user for PCB name
    char askForName[] = "Please enter the PCB name: ";
    sys_req(WRITE, COM1, askForName, strlen(askForName));

    // Reads in user response
    int userIn = sys_req(READ, COM1, name, strlen(name));
    sys_req(WRITE, COM1, name, userIn);

    // Checks if the PCB exists
    if(pcb_find(name) == NULL) {
        char nameMsg[] = "Name entered does not exist.\n";
        sys_req(WRITE, COM1, nameMsg, strlen(nameMsg));
        return;
    }

    struct pcb *showtarget = pcb_find(name);

    // holder for class
    char class_string[20] = {0};
    itoa(showtarget->class, class_string, 10);

    //holder for state
    char exestate_string[20] = {0};
    itoa(showtarget->exec_state, exestate_string, 10);

    //holder for priority
    char priorty_string[20] = {0};
    itoa(showtarget->priority, priorty_string, 10);

    char suspension_string[20] = {0};
    itoa(showtarget->disp_state, suspension_string, 10);


    sys_req(WRITE, COM1, name, strlen(name));
    sys_req(WRITE, COM1, class_string, strlen(class_string));
    sys_req(WRITE, COM1, exestate_string, strlen(exestate_string));
    sys_req(WRITE, COM1, priorty_string, strlen(priorty_string));

}


