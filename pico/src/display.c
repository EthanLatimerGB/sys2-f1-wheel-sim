#include "display.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "LCD_1in3.h"
#include "config.h"
#include "lib/Fonts/fonts.h"
#include <pico/time.h>
#include <stdint.h>

// Custom Colours
#define BACKGROUND 0x39a7
#define PRIMARY 0xf965
#define REV_COUNTER_YELLOW 0xffe0
#define REV_COUNTER_RED 0xf800
#define REV_COUNTER_BLUE 0x047f

UWORD *image_buf_ptr;

void init_display()
{
	printf("Initializing LVGL display...\n");
	// Intialises SPI interface with display
	if (DEV_Module_Init() != 0) {
		printf
		    ("Failed to initlaise display, check your connections to display and "
		     "reboot");
		return;
	}

	DEV_SET_PWM(50);	// Set backlight
	LCD_1IN3_Init(HORIZONTAL);

	UDOUBLE image_buf_size = LCD_1IN3_HEIGHT * LCD_1IN3_WIDTH * 2;

	image_buf_ptr = (UWORD *) malloc(image_buf_size);
	if (image_buf_ptr == NULL) {
		printf("Failed to initalise display buffer");
		return;
	}

	Paint_NewImage((UBYTE *) image_buf_ptr, LCD_1IN3.WIDTH, LCD_1IN3.HEIGHT,
		       0, WHITE);
	Paint_SetScale(65);	// This scales something, lines needed for display to work

	Paint_Clear(BACKGROUND);
	Paint_DrawRectangle(18, 38, 222, 18 + 152, PRIMARY, DOT_PIXEL_3X3,
			    DRAW_FILL_EMPTY);
	Paint_DrawRectangle(18, 38, 18 + 70, 18 + 152, PRIMARY, DOT_PIXEL_1X1,
			    DRAW_FILL_FULL);

	Paint_DrawString_EN(100, 218, "km/h", &Font12, WHITE, BACKGROUND);

	// Refreshes image buffer
	LCD_1IN3_Display(image_buf_ptr);
}

void update_RPM(uint16_t *rpm)
{
	// Calculating string buffer for RPM to display
	char num_buffer[5];
	sprintf(num_buffer, "%*d", 5, *rpm);

	// Drawing rev counter distance
	int rev_counter_pixels = findRevMeterPixels(rpm);

	printf("UPDATED ENGINE SPEED: %d \n", rpm);
	Paint_ClearWindows(0, 0, LCD_1IN3_WIDTH, 20, BACKGROUND);
	Paint_DrawRectangle(0, 0, rev_counter_pixels, 20, REV_COUNTER_BLUE,
			    DOT_PIXEL_DFT, DRAW_FILL_FULL);

	// Drwawing rev counter text
	Paint_DrawString_EN(120, 90, num_buffer, &Font24, WHITE, BACKGROUND);
	// LCD_1IN3_DisplayWindows(120, 90, 210, 114, image_buf_ptr);
	// LCD_1IN3_DisplayWindows(0, 0, LCD_1IN3_WIDTH, 20, image_buf_ptr);
}

void update_gear(uint16_t *gearNumber)
{
	// Calculate string buffer
	char gear_string[2];
	sprintf(gear_string, "%*d", 2, *gearNumber);

	printf("UPDATED GEAR: %d \n", gearNumber);
	Paint_SelectImage((UBYTE *) image_buf_ptr);
	Paint_DrawString_EN(40, 90, gear_string, &Font24, WHITE, PRIMARY);
	// LCD_1IN3_DisplayWindows(40, 90, 88, 114, image_buf_ptr);
}

void update_speed(uint16_t *speed)
{
	char num_buffer[4];
	sprintf(num_buffer, "%*d", 4, *speed);

	printf("UPDATED CAR SPEED: %d \n", speed);
	Paint_SelectImage((UBYTE *) image_buf_ptr);

	Paint_DrawString_EN(100, 189, num_buffer, &Font24, WHITE, BACKGROUND);
	// LCD_1IN3_DisplayWindows(100, 189, 140, 216, image_buf_ptr);
}

void draw_display()
{
	LCD_1IN3_Display(image_buf_ptr);
	return;
}

int findRevMeterPixels(uint16_t *engineRPM)
{
	if (*engineRPM < 10000)
		return 0;

	if (*engineRPM > MAX_ENGINE_RPM)
		return LCD_1IN3_WIDTH;

	// Maps engine RPM from [10000, 15000] onto pixel values [0,240]
	return ((*engineRPM - 10000) * LCD_1IN3_WIDTH) / (MAX_ENGINE_RPM -
							  10000);
}
