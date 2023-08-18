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

int serial_poll(device dev, char *buffer, size_t len)
{
    if (!buffer || len == 0) {
        return -1; // Error due to null buffer or zero length
    }

    size_t bytesRead = 0;
    while (bytesRead < len - 1) { // Reserve space for null-termination
        // Check if data is available
        if (inb(dev + LSR) & 0x01) {
            char c = inb(dev);

            // Handle alphanumeric characters and space
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                (c >= '0' && c <= '9') || c == ' ') {
                buffer[bytesRead++] = c;
                serial_out(dev, &c, 1); // Echo back the character
            }
                // Handle backspace
            else if (c == 0x08) {
                if (bytesRead > 0) {
                    bytesRead--;
                    serial_out(dev, &c, 1); // Echo back the backspace
                }
            }
                // Handle delete (assuming it deletes the last character)
            else if (c == 0x7F) {
                if (bytesRead > 0) {
                    bytesRead--;
                    char backspace = 0x08;
                    serial_out(dev, &backspace, 1); // Echo back a backspace
                }
            }
                // Handle arrow keys (assuming escape sequences of the form 0x1B 0x5B <Arrow Code>)
            else if (c == 0x1B) {
                char next1 = inb(dev);
                char next2 = inb(dev);
                // For now, we're ignoring arrow keys. If you decide to handle them, this is where you'd do it.
                continue;
            }
                // Handle newline or carriage return
            else if (c == '\n' || c == '\r') {
                buffer[bytesRead++] = c;
                serial_out(dev, &c, 1); // Echo back the newline/carriage return
                break; // Exit loop on newline
            }
        }
    }

    buffer[bytesRead] = '\0'; // Null-terminate the buffer
    return (int)bytesRead;
}


