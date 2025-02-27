#include <stdio.h>
#include "pico/stdlib.h"


void init_display();
void update_RPM(int *rpm);
void update_gear(int *gearNumber);
void update_speed(int *speed);
void draw_display();
int findRevMeterPixels(int *engineRPM);

