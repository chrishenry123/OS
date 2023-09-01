//
// Created by Dylan Caldwell on 8/18/23.
//
//./mpx.sh
#include "cmdHandler.h"
#include "version.h"
#include "help.h"
#include <string.h> // for string manipulation functions
#include <sys_req.h>
#include <mpx/serial.h>


#define COM1 0x3F8



// A helper function to process the user's command choice.
static void process_command(const char *command) {
    if (strcmp(command, "1") == 0) {
        // Call the Help function
        sys_req(WRITE, COM1, "Help: Displaying available commands...\n", strlen("Help: Displaying available commands...\n"));
	help();
    } else if (strcmp(command, "2") == 0) {
        // Set the Time
        sys_req(WRITE, COM1, "Setting Time...\n", strlen("Setting Time...\n"));
        // Add your time-setting logic here.
    } else if (strcmp(command, "3") == 0) {
        // Get the Time
        sys_req(WRITE, COM1, "Current Time: XX:XX:XX\n", strlen("Current Time: XX:XX:XX\n"));
    } else if (strcmp(command, "4") == 0) {
	// Show the version
	sys_req(WRITE, COM1, "Displaying Version...\n", strlen("Displaying Version...\n"));
	version();
    } else {
        sys_req(WRITE, COM1, "Invalid command. Please try again.\n", strlen("Invalid command. Please try again.\n"));
    }
}

// Function to remove trailing whitespace from a string
void rtrim(char *str) {
    int n = strlen(str) - 1;
    while (n >= 0 && (str[n] == ' ' || str[n] == '\t' || str[n] == '\n' || str[n] == '\r')) {
        str[n--] = '\0';
    }
}

void comhand(void) {
    char buf[101] = {0};

    // Initial welcome message
    char welcome_msg[] = "Welcome to MPX. Please select an option.\n1) Help\n2) Set Time\n3) Get Time\n4) Version\nEnter choice: ";
    sys_req(WRITE, COM1, welcome_msg, strlen(welcome_msg));

    for (;;) {
        memset(buf, 0, sizeof(buf));
        int nread = serial_poll(COM1, buf, sizeof(buf) - 1);

        if (nread >= 0) {
            buf[nread] = '\0';
        } else {
            continue;
        }

        // Remove newline or carriage return
        if (buf[nread - 1] == '\n' || buf[nread - 1] == '\r') {
            buf[nread - 1] = '\0';
        }

        // Remove trailing spaces
        rtrim(buf);

        // Now process the command
        if (strcmp(buf, "shutdown") == 0) {
            char confirm_msg[] = "Are you sure you want to shut down? (y/n): ";
            sys_req(WRITE, COM1, confirm_msg, strlen(confirm_msg));

            char confirm[5] = {0};
            sys_req(READ, COM1, confirm, sizeof(confirm) - 1);
            confirm[4] = '\0';

            int confirmRead = strlen(confirm);
            if (confirm[confirmRead - 1] == '\n' || confirm[confirmRead - 1] == '\r') {
                confirm[confirmRead - 1] = '\0';
            }
            if (strcmp(confirm, "y") == 0) {
                return;
            }
        } else {
            process_command(buf);
        }

        sys_req(WRITE, COM1, welcome_msg, strlen(welcome_msg));
    }
}



