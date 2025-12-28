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
#include "DEV_Config.h"
#include "communication.h"
#include "display.h"

#include <LCD_1in3.h>

Command unacknowledged_cmds[MAX_COMMANDS];

int main()
{
	if (init_display() != 0) {
		printf("Catastrophic failure to init - report issue");
		return -1;
	}

	/*

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

	 */

}
