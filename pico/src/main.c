#include "pico/cyw43_arch.h"
#include "hardware/irq.h"
#include "hardware/timer.h"
#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/regs/dreq.h>
#include <hardware/regs/intctrl.h>
#include <hardware/uart.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <pico/util/queue.h>
#include <stdint.h>

#include "communication.h"
#include "display.h"

// Task: Sample every 100ms a serial message and display it to a screen
// - Create a periodic timer that runs a function to read the input and push message to a buffer structure.

int main()
{
	stdio_init_all();
	// setupUART();
	// setupDMAforUART();
	// setupDMAInterrupts();

	init_display();	


	// Set up repeating timer, this is an API call that sets up an IRQ
	while (1) {
	}
}
