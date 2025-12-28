#include "DEV_Config.h"
#include <drawing.h>
#include <hardware/dma.h>
#include <hardware/spi.h>
#include <pico/time.h>
#include <src/display/lv_display.h>
#include <src/lv_init.h>
#include <src/misc/lv_area.h>
#include <src/misc/lv_color.h>
#include <src/misc/lv_types.h>
#include <src/tick/lv_tick.h>
#include <stdint.h>
#include <display.h>
#include <DEV_Config.h>

// LCD Display specific description
LCD_SetWindowsFunc LCDSetWindows;

// LVGL specific descriptions
lv_display_t *display_lcd;

// The scratchpad buffers for the display. Represents 'chunks' of the display
// which is guaranteed to be 'dirty' and needs re-rendered.
static lv_color_t *buf0;
static lv_color_t *buf1;

static inline uint32_t area_pixel_cnt(const lv_area_t *a)
{
	// Get the pixel count of the width and heights
	uint32_t w = (a->x2 - a->x1 + 1);
	uint32_t h = (a->y2 - a->y1 + 1);

	return w * h;
}

/**
 * display-flush-cb - Callback funct. to send LVGL display data to display driver. 
 * @display: LVGL display data
 * @area: LVGL area
 * @color_p: Pointer to start address of pointer
 * 
 * Callback function designed to flush LVGL data to the display driver over SPI. 
 * This callback utilises Direct Memory Access (DMA) to reduce CPU cycles involved
 * with this flush. 
 * An IRQ separate to this function will handle when the process has completed. 
 */
void display_flush_cb(lv_display_t *display, const lv_area_t *area,
		      lv_color_t *color_p)
{
	LCDSetWindows(area->x1, area->y1, area->x2, area->y2);
	DEV_Digital_Write(LCD_DC_PIN, 1);	// Data is written
	DEV_Digital_Write(LCD_CS_PIN, 0);	// Select display on SPI

	// Indicates the total number of bytes sent per frame. We multiply by 2
	// to indicate 2 bytes per pixel. RGB565
	uint32_t transfer_count = area_pixel_cnt(area) * 2;

	// NOTE: Dodgy (no idea how it works) for spi_get_hw.
	// Trigger (last var) indicates we start transfer immediately
	dma_channel_configure(dma_tx,
			      &dma_cfg,
			      spi_get_hw(spi1), color_p, transfer_count, true);
}

/**
* LVGL_init - Initialises the LVGL content.
* Function sets up all which is LVGL; the internal workings, 
* hooking to the display driver, creating display buffers, etc. 
*/ void LVGL_init()
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

	// Set the display buffers up
	lv_display_set_buffers(display_lcd, buf0, buf1, sizeof(buf0),
			       LV_DISPLAY_RENDER_MODE_PARTIAL);

	// Set up a flush callback that the display driver specifically recognises
	lv_display_set_flush_cb(display_lcd, display_flush_cb);

}

void handle_keypress(lvgl_data_struct lv_state);
void switch_to_screen(lvgl_data_struct lv_state);

uint32_t get_tick_since_startup()
{
	return to_ms_since_boot(get_absolute_time());
}
