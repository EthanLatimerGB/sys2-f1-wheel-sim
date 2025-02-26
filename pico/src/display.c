#include "display.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "LCD_1in3.h"
#include "lib/Fonts/fonts.h"
#include <pico/time.h>

// Custom Colours
#define BACKGROUND 0x39a7
#define PRIMARY 0xf965
#define REV_COUNTER_YELLOW 0xffe0
#define REV_COUNTER_RED 0xf800
#define REV_COUNTER_BLUE 0x047f

UWORD *image_buf_ptr;

void init_display() {
  sleep_ms(2000);
  printf("Initialising defualt display configuration\n");
  // Intialises SPI interface with display
  if (DEV_Module_Init() != 0) {
    printf("Failed to initlaise display, check your connections to display and "
           "reboot");
    return;
  }

  // No idea what this does, but part of init steps
  DEV_SET_PWM(50);

  LCD_1IN3_Init(HORIZONTAL);

  UDOUBLE image_buf_size = LCD_1IN3_HEIGHT * LCD_1IN3_WIDTH * 2;

  image_buf_ptr = (UWORD *)malloc(image_buf_size);
  if (image_buf_ptr == NULL) {
    printf("Failed to initalise display buffer");
    return;
  }

  Paint_NewImage((UBYTE *)image_buf_ptr, LCD_1IN3.WIDTH, LCD_1IN3.HEIGHT, 0,
                 WHITE);
  Paint_SetScale(65); // This scales something, lines needed for display to work

  Paint_Clear(BACKGROUND);
  Paint_DrawRectangle(18, 38, 222, 18 + 152, PRIMARY, DOT_PIXEL_3X3,
                      DRAW_FILL_EMPTY);
  Paint_DrawRectangle(18, 38, 18 + 70, 18 + 152, PRIMARY, DOT_PIXEL_3X3,
                      DRAW_FILL_FULL);

  // Refreshes image buffer
  LCD_1IN3_Display(image_buf_ptr);
}

void update_RPM(const char *rpmString) {
  Paint_SelectImage((UBYTE *)image_buf_ptr);

  Paint_ClearWindows(120, 80, 220, 104, BACKGROUND);
  Paint_DrawString_EN(120, 80, rpmString, &Font24, WHITE, BACKGROUND);
  LCD_1IN3_DisplayWindows(120, 80, 220, 104, image_buf_ptr);
}

void update_gear(const char *gearNumber) {
  Paint_SelectImage((UBYTE *)image_buf_ptr);

  Paint_ClearWindows(120, 80, 220, 104, BACKGROUND);
  Paint_DrawString_EN(120, 100, gearNumber, &Font24, WHITE, BACKGROUND);
  LCD_1IN3_DisplayWindows(120, 100, 220, 130, image_buf_ptr);
}
