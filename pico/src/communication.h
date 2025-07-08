#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/regs/dreq.h>
#include <hardware/regs/intctrl.h>
#include <hardware/uart.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_COMMANDS 5

typedef enum { EXPR_ES, EXPR_CS, EXPR_CG, EXPR_T, EXPR_B, EXPR_INVALID } Expr;

typedef struct {
	Expr cmd;
	uint16_t value;
} Command;

/*
 * A interrupt handler that processes input messages that comes through 
 */
void command_over_uart_handler();

/*
 * Sets up the UART pins on GPIO for message communication between PC and Pico
*/
void setupUART();

/*
 * Sets up the DMA controller to directly manipulate the memory buffer uart_rx_buffer, with data coming from UART RX
*/
void setupDMAforUART();

/*
* Sets up all the interrupts involved in the event a DMA controller completes an operation
*/
void setupDMAInterrupts();

/*
 * IRQ handler in the event that a new buffer for a message has came through.
 * IRQ triggered by DMA controller
*/
void dmaIRQhandler();

/*
 * Given two strings, each containing the information extracted from a string, this function returns it's formatted Command. Commands with EXPR_INVALID should be ignored and not acted on further. 
*/
Command parseCommand(const char *command_str, const char *value_str);
/*
 * Handles interpreting an input string and breaks it up into a list of Commands, which is saved . 
 * Will transfer parsed contents into parsed_buffer variable.
 *
 * Grammar is defined as:
 * CMDS := CMD | CMD ';' CMDS
 * CMD  := EXPR ':' VAL
 * EXPR := 'ES' | 'CS' | 'CG' | 'T' | 'B'
 * VAL  := 16-bit unsigned integer (0 to 65535)
 *
 * \param Single line which contains the commands.
 * \param The location of the Command array
 * \param The size of the command array
*/
int parseInput(const char *input, Command * commands, int max_commands);
