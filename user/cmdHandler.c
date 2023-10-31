// Created by Dylan Caldwell on 8/18/23.
// ./mpx.sh
#include "cmdHandler.h"
#include "version.h"
#include "help.h"
#include "time.h"
#include "pcb.h"
#include "pcbuser.h"
#include <alarm.h>
#include <yield.h>
#include "load_r3.h"
#include <string.h> // for string manipulation functions
#include <sys_req.h>
#include <mpx/serial.h>
#include <stdlib.h>

#define COM1 0x3F8
#define MAX_WELCOME_SIZE 1024

typedef struct {
    const char *command_str;
    void (*command_func)(void);
    const char *output_message;
    int subMenuIndex;  // -1 if no submenu
} command_map_t;

typedef struct {
    const char* menu_name;
    command_map_t* commands;
} menu_t;

static command_map_t main_commands[] = {
        {"Help", help, "Displaying available commands...\n", -1},
        {"Time/Date Functions", NULL, "Navigating to Time/Date Functions...\n", 1},
        {"PCB Functions", NULL, "Navigating to PCB Functions...\n", 2},
        {"Version", version, "Displaying Version...\n", -1},
//        {"Yield", yield, "Yielding current process...\n", -1}, REMOVED FOR M4
        {"Alarm", get_alarm, "Setting Alarm...\n", -1},
        {NULL, NULL, NULL, -1}
};

static command_map_t time_date_commands[] = {
        {"Set Time", setTime, "Setting Time...\n", -1},
        {"Get Time", get_time, "Current Time is...\n", -1},
        {"Set Date", setDate, "Setting Date...\n", -1},
        {"Get Date", getDate, "Getting Date...\n", -1},
        {"Return to Main Menu", NULL, "Returning...\n", 0},
        {NULL, NULL, NULL, -1}
};

static command_map_t pcb_commands[] = {
        {"Load R3", load_r3, "Loading R3 test processes...\n", -1},
//        {"Create PCB", create_pcb, "Creating PCB...\n", -1}, REMOVED FOR M3
        {"Delete PCB", delete_pcb, "Deleting PCB...\n", -1},
        {"Block PCB", block_pcb, "Blocking PCB...\n", -1},
        {"Unblock PCB", unblock_pcb, "Unblocking PCB...\n", -1},
        {"Suspend PCB", suspend_pcb, "Suspending PCB...\n", -1},
        {"Resume PCB", resume_pcb, "Resuming PCB...\n", -1},
        {"Set Priority", set_priority, "Setting Priority...\n", -1},
        {"Show PCB", show_pcb, "Showing PCB...\n", -1},
        {"Show Ready", show_ready, "Displaying processes in READY state...\n", -1},
        {"Show Blocked", show_blocked, "Displaying processes in BLOCKED state...\n", -1},
        {"Show All", show_all, "Displaying all processes...\n", -1},
        {"Return to Main Menu", NULL, "Returning...\n", 0},
        {NULL, NULL, NULL, -1}
};

static menu_t menus[] = {
        {"Main Menu", main_commands},
        {"Time/Date Menu", time_date_commands},
        {"PCB Menu", pcb_commands}
};

static int current_menu = 0;

void generate_welcome_message(char *buf) {
    menu_t currentMenu = menus[current_menu];

    strcpy(buf, "Welcome to MPX. ");
    strcat(buf, currentMenu.menu_name);
    strcat(buf, "\n");

    int index = 1;
    for (int i = 0; currentMenu.commands[i].command_str; i++) {
        char option[100];
        sprintf(option, "%d) %s\n", index, currentMenu.commands[i].command_str);
        strcat(buf, option);
        index++;
    }

    strcat(buf, "Enter choice: ");
}

static int shutdown_requested = 0;  // 0 for false, 1 for true

static void process_command(const char *command) {
    // Shutdown command
    if (strcmp(command, "shutdown") == 0) {
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
            shutdown_requested = 1;  // set the flag true
            return;
        }
    } else {
        // Parse the entered command into an integer
        int choice = atoi(command);
        if (choice <= 0) {
            char error_msg[] = "Invalid choice. Please enter a valid number.\n";
            sys_req(WRITE, COM1, error_msg, strlen(error_msg));
            return;
        }

        menu_t currentMenu = menus[current_menu];
        if (currentMenu.commands[choice - 1].command_str == NULL) {
            char error_msg[] = "Invalid choice. Please enter a valid number.\n";
            sys_req(WRITE, COM1, error_msg, strlen(error_msg));
            return;
        }

        // Check if the command has a function to execute
        if (currentMenu.commands[choice - 1].command_func) {
            currentMenu.commands[choice - 1].command_func();
            return;
        }
        // Check if the command is meant to navigate to another menu
        if (currentMenu.commands[choice - 1].subMenuIndex >= 0) {
            current_menu = currentMenu.commands[choice - 1].subMenuIndex;
            return;
        }
    }
}


void comhand(void) {
    char buf[101] = {0};

    char welcome_msg[MAX_WELCOME_SIZE];
    generate_welcome_message(welcome_msg);
    sys_req(WRITE, COM1, welcome_msg, strlen(welcome_msg));

    for (;;) {
        if (shutdown_requested) {
            break;  // break out of the loop and end the comhand function
        }

        memset(buf, 0, sizeof(buf));
        int nread = serial_poll(COM1, buf, sizeof(buf) - 1);

        if (nread >= 0) {
            buf[nread] = '\0';
        } else {
            continue;
        }

        if (buf[nread - 1] == '\n' || buf[nread - 1] == '\r') {
            buf[nread - 1] = '\0';
        }

        process_command(buf);
        generate_welcome_message(welcome_msg);
        sys_req(WRITE, COM1, welcome_msg, strlen(welcome_msg));
    }
}

