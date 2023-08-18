//
// Created by Dylan Caldwell on 8/18/23.
//

#ifndef FIJI_CMDHANDLER_H
#define FIJI_CMDHANDLER_H

#include <stddef.h>  // For size_t

// Define constants, macros, or typedefs if needed.

// Function declarations:

/**
 * Command handler function that processes user commands from the serial interface.
 * This function continually reads commands from the serial port, processes them,
 * and then responds back through the same serial port.
 */
void comhand(void);


#endif //FIJI_CMDHANDLER_H
