/**
 * Covers the linking between LVGL and the Graphics driver of the Display used in this project.
 * Also contains all the information about widgets setup.
 */
#include <lvgl/lvgl.h>

#define DISP_HOR_RES 240
#define DISP_VER_RES 240
/**
 * typedef struct - State Object for LVGL in order to draw those objects included.
 * @scr:	Defines all the possible screens.
 * @gear_lab:
 * @rpm_lab:
 * @speed_lab:
 * @thrott_lab:
 * @gear_val_lab:	Label displaying current gear
 * @rpm_val_lab:	Label displaying current RPM of engine
 * @speed_val_lab:	Label displaying current speed of Car in Km/h
 * @thrott_val_lab:	Label displaying current throttle application 
 * @gear_val:		Value of gear
 * @rpm_val:		Value of RPM
 * @speed_val:		Value of Speed
 * @throttle_val:	Value of throttle application between 0-100% (Integers)
 *
 * See brief
*/
typedef struct {
	// Screens
	lv_obj_t *scr[2];
	// Labels
	lv_obj_t *gear_lab;
	lv_obj_t *rpm_lab;
	lv_obj_t *speed_lab;
	lv_obj_t *thrott_lab;
	// Labels displaying data
	lv_obj_t *gear_val_lab;
	lv_obj_t *rpm_val_lab;
	lv_obj_t *speed_val_lab;
	lv_obj_t *thrott_val_lab;
	// Values
	uint16_t gear_val;
	uint16_t rpm_val;
	uint16_t speed_val;
	uint16_t throttle_val;
} lvgl_data_struct;

/**
 * A type defined as a function pointer that accepts 4 integers.
 * Used to point to an internal funciton in the LVGL library to set the current window.
*/
typedef void (*LCD_SetWindowsFunc)(int32_t, int32_t, int32_t, int32_t);

/**
* Callback function to send LVGL display data to display driver.
*/
void display_flush_cb(lv_display_t * display, const lv_area_t * area,
		      lv_color_t * color_p);

/**
* TODO: Once compelte, finish explanation
* Intialises LVGL contexts and builds the widgets.
*/
void LVGL_init();

/**
* Handles button actions in terms of the LVGL context.
*/
void handle_keypress(lvgl_data_struct lv_state);

/**
 * Switches screen.
 * Will only switch if the lv_state struct has changed to a different screen.
*/
void switch_to_screen(lvgl_data_struct lv_state);

/**
* Returns ticks since startup from RP2040
*/
uint32_t get_tick_since_startup();
