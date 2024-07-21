#ifndef _WS2812B_H_
#define _WS2812B_H_

#include "nrf.h"
#include "nrf_delay.h"

#define WS2812B_PIN 28
#define MAX_LEDs_in_STRIP 8
#define PACKET_SIZE 24
#define PACKET_DELAY 10 // in us

void nrf_delay_us_ticks(uint32_t ticks);
void gpio_init(void);
void send_high(void);
void send_low(void);
void switch_off_LEDs(uint8_t number_of_leds);
void WS2812B_Init(void);
void set_one_LED(uint8_t rgb[3], uint8_t led_number);
void set_LED(uint8_t rgb[3]);

#endif