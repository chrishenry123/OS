//
// Created by Dylan Caldwell on 8/18/23.
//
//./mpx.sh
#include "cmdHandler.h"
#include <string.h> // for string manipulation functions
#include <sys_req.h>


#define COM1 0x3F8


// Define other necessary constants, macros, and internal helper functions.

// A helper function to process the user's command choice.
static void process_command(const char *command) {
    if (strcmp(command, "1") == 0) {
        // Call the Help function
        sys_req(WRITE, COM1, "Help: Displaying available commands...\n", strlen("Help: Displaying available commands...\n"));
    } else if (strcmp(command, "2") == 0) {
        // Set the Time
        sys_req(WRITE, COM1, "Setting Time...\n", strlen("Setting Time...\n"));
        // Add your time-setting logic here.
    } else if (strcmp(command, "3") == 0) {
        // Get the Time
        sys_req(WRITE, COM1, "Current Time: XX:XX:XX\n", strlen("Current Time: XX:XX:XX\n"));
    } else {
        sys_req(WRITE, COM1, "Invalid command. Please try again.\n", strlen("Invalid command. Please try again.\n"));
    }
}

void comhand(void) {
    sys_req(WRITE, COM1, "Welcome to MPX. Please select an option.\n1) Help\n2) Set Time\n3) Get Time\nEnter choice: ", strlen("Welcome to MPX. Please select an option.\n1) Help\n2) Set Time\n3) Get Time\nEnter choice: "));

    for (;;) {
        char buf[100] = {0};
        int nread = sys_req(READ, COM1, buf, sizeof(buf));

        // Echo the input back to the user for clarity
        sys_req(WRITE, COM1, buf, nread);

        // Check if the user wants to shut down.
        if (strcmp(buf, "shutdown") == 0) {
            sys_req(WRITE, COM1, "Are you sure you want to shut down? (y/n): ", strlen("Are you sure you want to shut down? (y/n): "));
            char confirm[5] = {0};
            sys_req(READ, COM1, confirm, sizeof(confirm));
            if (strcmp(confirm, "y") == 0) {
                // Implement the shutdown process.
                return;
            }
        } else {
            process_command(buf);
        }

        // Display the menu again for the next command
        sys_req(WRITE, COM1, "\nWelcome to MPX. Please select an option.\n1) Help\n2) Set Time\n3) Get Time\nEnter choice: ", strlen("\nWelcome to MPX. Please select an option.\n1) Help\n2) Set Time\n3) Get Time\nEnter choice: "));
    }
}
