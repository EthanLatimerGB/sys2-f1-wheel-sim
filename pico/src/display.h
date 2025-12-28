#include <hardware/dma.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

int dma_tx;			// Transmit pin for DMA to Display
dma_channel_config dma_cfg;

int init_display();

static void disp_dma_handler(void);

// void update_RPM(uint16_t * rpm);
// void update_gear(uint16_t * gearNumber);
// void update_speed(uint16_t * speed);
// void draw_display();
// int findRevMeterPixels(uint16_t * engineRPM);
