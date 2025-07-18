#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/regs/dreq.h>
#include <hardware/regs/intctrl.h>
#include <hardware/timer.h>
#include <hardware/uart.h>
#include <pico/time.h>
#include <pico/types.h>
#include <pico/util/queue.h>
#include <pico/stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <pico/time.h>

#include "communication.h"
#include "display.h"

Command unacknowledged_cmds[MAX_COMMANDS];

// Task: Sample every 100ms a serial message and display it to a screen
// - Create a periodic timer that runs a function to read the input and push
// message to a buffer structure.
//

int main()
{
	stdio_init_all();
	char CommBuffer[1024];
	uint16_t rpm = 0;

	absolute_time_t past_time = 0;
	absolute_time_t poll_time = 0;

	// setupUART();
	init_display();

	update_RPM(&rpm);
	while (1) {

		scanf("%1024s", CommBuffer);
		printf("Acknowledged bum: %s\n", CommBuffer);

		parseInput(CommBuffer, unacknowledged_cmds, MAX_COMMANDS);

		past_time = get_absolute_time();
		for (int i = 0;
		     i < (sizeof(unacknowledged_cmds) / sizeof(Command)); i++) {
			Command *command = &unacknowledged_cmds[i];

			switch (command->cmd) {
			case EXPR_ES:
				update_RPM(&command->value);
				break;
			case EXPR_CS:
				update_speed(&command->value);
				break;
			case EXPR_CG:
				update_gear(&command->value);
				break;
			default:
				continue;
			}

		}

		draw_display();
		poll_time = get_absolute_time();

		absolute_time_t difference =
		    absolute_time_diff_us(past_time, poll_time);
		printf("%llu is the difference.\n", difference);
	}

	// setupDMAforUART();
	// setupDMAInterrupts();

	/*
	   int j = 1;

	   // Set up repeating timer, this is an API call that sets up an IRQ
	   while (1) {
	   j += 1;
	   update_gear(&j);

	   for (int i = 0; i < 15000; i += 200) {
	   sleep_ms(8);
	   update_RPM(&i);
	   update_speed(&i);
	   draw_display();
	   }
	   }
	 */
}
