//
// Created by Dylan Caldwell on 8/18/23.
//
//./mpx.sh
#include "cmdHandler.h"
#include "version.h"
#include <string.h> // for string manipulation functions
#include <sys_req.h>
#include <mpx/serial.h>


#define COM1 0x3F8



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
    char buf[101] = {0};

    // Initial welcome message
    char welcome_msg[] = "Welcome to MPX. Please select an option.\n1) Help\n2) Set Time\n3) Get Time\nEnter choice: ";
    sys_req(WRITE, COM1, welcome_msg, strlen(welcome_msg));

    // Main loop
    for (;;) {
        memset(buf, 0, sizeof(buf));  // Zero out the buffer
        // Read from the serial port using serial_poll
        int nread = serial_poll(COM1, buf, sizeof(buf) - 1);
        // Null-terminate the string to be safe
        if (nread >= 0) {
            buf[nread] = '\0';
        } else {
            // Handle error condition if needed
            continue;
        }
        // Strip newline or carriage return if necessary
        if (buf[nread - 1] == '\n' || buf[nread - 1] == '\r') {
            buf[nread - 1] = '\0';
        }
        // Check if the user wants to shut down
        if (strcmp(buf, "shutdown") == 0) {
            char confirm_msg[] = "Are you sure you want to shut down? (y/n): ";
            sys_req(WRITE, COM1, confirm_msg, strlen(confirm_msg));
            char confirm[5] = {0};
            sys_req(READ, COM1, confirm, sizeof(confirm));
            if (strcmp(confirm, "y") == 0) {
                // Shut down and break out of the loop
                return;
            }
        } else {
            process_command(buf);
        }
        // Show the menu again using sys_req
        sys_req(WRITE, COM1, welcome_msg, strlen(welcome_msg));
    }
}



