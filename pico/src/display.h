#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

void init_display();
void update_RPM(uint16_t * rpm);
void update_gear(uint16_t * gearNumber);
void update_speed(uint16_t * speed);
void draw_display();
int findRevMeterPixels(uint16_t * engineRPM);
