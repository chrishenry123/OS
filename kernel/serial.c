#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>

enum uart_registers {
	RBR = 0,	// Receive Buffer
	THR = 0,	// Transmitter Holding
	DLL = 0,	// Divisor Latch LSB
	IER = 1,	// Interrupt Enable
	DLM = 1,	// Divisor Latch MSB
	IIR = 2,	// Interrupt Identification
	FCR = 2,	// FIFO Control
	LCR = 3,	// Line Control
	MCR = 4,	// Modem Control
	LSR = 5,	// Line Status
	MSR = 6,	// Modem Status
	SCR = 7,	// Scratch
};

static int initialized[4] = { 0 };

static int serial_devno(device dev)
{
	switch (dev) {
	case COM1: return 0;
	case COM2: return 1;
	case COM3: return 2;
	case COM4: return 3;
	}
	return -1;
}

int serial_init(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1;
	}
	outb(dev + IER, 0x00);	//disable interrupts
	outb(dev + LCR, 0x80);	//set line control register
	outb(dev + DLL, 115200 / 9600);	//set bsd least sig bit
	outb(dev + DLM, 0x00);	//brd most significant bit
	outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
	outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte threshold
	outb(dev + MCR, 0x0B);	//enable interrupts, rts/dsr set
	(void)inb(dev);		//read bit to reset port
	initialized[dno] = 1;
	return 0;
}

int serial_out(device dev, const char *buffer, size_t len)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return -1;
	}
	for (size_t i = 0; i < len; i++) {
		outb(dev, buffer[i]);
	}
	return (int)len;
}

int serial_poll(device dev, char *buffer, size_t len) {
    if (!buffer || len == 0) {
        return -1; // Invalid input
    }

    size_t bytesRead = 0;
    size_t cursorPos = 0;
    int is_escape_seq = 0;

    while (bytesRead < len - 1) {
        if (inb(dev + LSR) & 0x01) {
            char c = inb(dev);
            if (is_escape_seq) {
                if (c == '[') {
                    is_escape_seq++;
                } else if (is_escape_seq == 2) {
                    if (c == 'D' && cursorPos > 0) { // Left Arrow
                        cursorPos--;
                        char moveLeft[] = {0x1B, '[', 'D'};
                        serial_out(dev, moveLeft, 3);
                    } else if (c == 'C' && cursorPos < bytesRead) { // Right Arrow
                        cursorPos++;
                        char moveRight[] = {0x1B, '[', 'C'};
                        serial_out(dev, moveRight, 3);
                    } else if (c == '3' && inb(dev) == '~') { // Delete Key
                        if (cursorPos < bytesRead) {
                            for (size_t i = cursorPos + 1; i < bytesRead; i++) {
                                buffer[i - 1] = buffer[i];
                            }
                            bytesRead--;
                            buffer[bytesRead] = '\0';
                            char clearLine[] = {0x1B, '[', 'K'};
                            serial_out(dev, clearLine, 3);
                            for (size_t i = cursorPos; i < bytesRead; i++) {
                                serial_out(dev, &buffer[i], 1);
                            }
                        }
                    }
                    is_escape_seq = 0;
                }
                continue;
            }
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                (c >= '0' && c <= '9') || c == ' ' || c == ':' ||
                c == ';' || c == '.' || c == ',') {
                // Insert the new character
                for (size_t i = bytesRead; i > cursorPos; i--) {
                    buffer[i] = buffer[i - 1];
                }
                buffer[cursorPos] = c;
                cursorPos++;
                bytesRead++;
                serial_out(dev, &c, 1);
            } else if (c == 0x08 || c == 0x7F) { // Backspace or Delete (on some terminals)
                if (cursorPos > 0) {
                    // Remove the character from the buffer
                    for (size_t i = cursorPos; i < bytesRead; i++) {
                        buffer[i - 1] = buffer[i];
                    }
                    cursorPos--;
                    bytesRead--;
                    buffer[bytesRead] = '\0';
                    // Clear from cursor to end of line
                    char moveLeft[] = {0x1B, '[', 'D'};
                    char clearLine[] = {0x1B, '[', 'K'};
                    serial_out(dev, moveLeft, 3);
                    serial_out(dev, clearLine, 3);

                    // Redraw the line after cursor
                    for (size_t i = cursorPos; i < bytesRead; i++) {
                        serial_out(dev, &buffer[i], 1);
                    }
                    // Move cursor back to original position
                    for (size_t i = bytesRead; i > cursorPos; i--) {
                        serial_out(dev, moveLeft, 3);
                    }
                }
            } else if (c == 0x1B) { // Start of an Escape Sequence
                is_escape_seq = 1;
            } else if (c == '\n' || c == '\r') { // New Line or Carriage Return
                buffer[bytesRead] = c;
                bytesRead++;
                serial_out(dev, &c, 1);
                break;
            }
        }
    }
    buffer[bytesRead] = '\0';
    return (int) bytesRead;
}




















