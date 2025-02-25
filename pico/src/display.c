#include "display.h"
#include "DEV_Config.h"
#include "LCD_1in3.h"
#include "GUI_Paint.h"

// Custom Colours
#define BACKGROUND		0x39a7
#define PRIMARY			0xf965
#define REV_COUNTER_YELLOW	0xffe0
#define REV_COUNTER_RED		0xf800
#define REV_COUNTER_BLUE	0x047f

void init_display()
{
	printf("Initialising defualt display configuration\n");
	// Intialises SPI interface with display
	if(DEV_Module_Init() != 0) {
		printf("Failed to initlaise display, check your connections to display and reboot");
		return;
	}

	// No idea what this does, but part of init steps
	DEV_SET_PWM(50);

	UWORD* image_buf_ptr;
	UDOUBLE image_buf_size = LCD_1IN3.HEIGHT * LCD_1IN3.WIDTH * 2;

	image_buf_ptr = malloc(image_buf_size);
	if(image_buf_ptr == NULL) {
		printf("Failed to initalise display buffer");
		exit(0);
	}

	LCD_1IN3_Init(HORIZONTAL);
	LCD_1IN3_Clear(BACKGROUND);

	Paint_NewImage((UBYTE *)image_buf_ptr, LCD_1IN3.WIDTH, LCD_1IN3.HEIGHT, 0, BACKGROUND);
	
	Paint_DrawRectangle(18, 18, 18 + 204, 18 + 132, PRIMARY, DOT_PIXEL_3X3, DRAW_FILL_EMPTY);
	Paint_DrawRectangle(18, 18, 18 + 70, 18 + 132, PRIMARY, DOT_PIXEL_3X3, DRAW_FILL_FULL);

	LCD_1IN3_Display(image_buf_ptr);
}
