#include <drawing.h>
#include <pico/time.h>
#include <src/display/lv_display.h>
#include <src/lv_init.h>
#include <src/misc/lv_area.h>
#include <src/misc/lv_color.h>
#include <src/misc/lv_types.h>
#include <src/tick/lv_tick.h>
#include <stdint.h>

// LCD Display specific description
LCD_SetWindowsFunc LCDSetWindows;

// LVGL specific descriptions
lv_display_t *display_lcd;

// The scratchpad buffers for the display. Represents 'chunks' of the display
// which is guaranteed to be 'dirty' and needs re-rendered.
static lv_color_t *buf0;
static lv_color_t *buf1;

/**
 * display-flush-cb - Callback funct. to send LVGL display data to display driver. 
 * @display: LVGL display data
 * @area: LVGL area
 * @color_p: TODO: Finish doc here
 *
 *
 */
void display_flush_cb(lv_display_t *display, const lv_area_t *area,
		      lv_color_t *color_p)
{
	LCD_SetWindowsFunc(area->x1, area->y1, area->x2, area->y2);

	// NOTE: COntinue for SPI functionality
}

/**
* 
*
*/
void LVGL_init()
{
	lv_init();

	// Initialise tick timer for LVGL synchronisation
	lv_tick_set_cb(get_tick_since_startup);
	display_lcd = lv_display_create(DISP_HOR_RES, DISP_VER_RES);

	// Initialising the draw buffers (Less tearing). It is 1/10th of a screen
	// as a engineer choice for good space usage but maintains decent perf.
	// These buffers will be sent over SPI to the display IC and later gets called
	// to draw by this controller. 
	buf0 = malloc(DISP_HOR_RES * DISP_VER_RES * sizeof(lv_color_t) / 10);
	buf1 = malloc(DISP_HOR_RES * DISP_VER_RES * sizeof(lv_color_t) / 10);

	// Set the 
	lv_display_set_buffers(display_lcd, buf0, buf1, sizeof(buf0),
			       LV_DISPLAY_RENDER_MODE_PARTIAL);

}

void handle_keypress(lvgl_data_struct lv_state);
void switch_to_screen(lvgl_data_struct lv_state);

uint32_t get_tick_since_startup()
{
	return to_ms_since_boot(get_absolute_time());
}
