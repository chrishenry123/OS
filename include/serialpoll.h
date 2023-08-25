//
// Created by Dylan Caldwell on 8/23/23.
//

#ifndef FIJI_SERIALPOLL_H
#define FIJI_SERIALPOLL_H

#include <stddef.h>  // For size_t

/**
 * Polls the serial device for available data and reads it into a buffer.
 * This function is non-blocking and will read as much data as available
 * into the user-provided buffer, up to len - 1 bytes, reserving space for null-termination.
 *
 * @param dev    The device to read from.
 * @param buffer The user-provided buffer to store read data.
 * @param len    The size of the user-provided buffer.
 *
 * @return The number of bytes read, or a negative number on error.
 */
int serial_poll(device dev, char *buffer, size_t len);

#endif //FIJI_SERIALPOLL_H
