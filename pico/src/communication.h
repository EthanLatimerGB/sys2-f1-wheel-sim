#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/regs/dreq.h>
#include <hardware/regs/intctrl.h>
#include <hardware/uart.h>

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
