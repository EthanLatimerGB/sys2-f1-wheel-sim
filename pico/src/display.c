#include "display.h"
#include "DEV_Config.h"
#include "LCD_1in3.h"
#include <hardware/dma.h>
#include <pico/time.h>
#include <stdint.h>

//flush Custom Colours
#define BACKGROUND 0x39a7
#define PRIMARY 0xf965
#define REV_COUNTER_YELLOW 0xffe0
#define REV_COUNTER_RED 0xf800
#define REV_COUNTER_BLUE 0x047f

UWORD *image_buf_ptr;
uint dma_tx;			// Transmit pin for DMA to Display
dma_channel_config dma_cfg;

int init_display()
{
	dma_tx = dma_claim_unused_channel(true);
	if (dma_tx == -1) {
		printf
		    ("WARNING: Failed to grab a DMA channel - report this issue if persistent.");
		return -1;
	}
	// Intialises SPI interface with display
	printf("Initializing LVGL display...\n");
	if (DEV_Module_Init() != 0) {
		printf
		    ("Failed to initlaise display, check your connections to display and "
		     "reboot");
		return -1;
	}
	// Set display specific params
	DEV_SET_PWM(50);	// Set backlight
	LCD_1IN3_Init(HORIZONTAL);
	LCD_1IN3_Clear(0xFFFF);	// init as white
	return 0;
}
