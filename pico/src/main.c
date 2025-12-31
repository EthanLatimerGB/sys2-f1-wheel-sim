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

#include <LCD_1in3.h>

Command unacknowledged_cmds[MAX_COMMANDS];

int main()
{
	if (init_display() != 0) {
		printf("Catastrophic failure to init - report issue");
		return -1;
	}

}
