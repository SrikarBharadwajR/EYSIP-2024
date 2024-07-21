#include "WS2812B.h"

void nrf_delay_us_ticks(uint32_t ticks) {
  while (ticks--) {
    __NOP(); // This will burn some cycles
  }
}
void gpio_init(void) {
  NRF_GPIO->DIRSET = (1 << WS2812B_PIN);  // Configure the pin as output
  NRF_GPIO->OUTCLR = (1 << WS2812B_PIN); // Set the pin low initially
}

void send_high(void) {
  NRF_GPIO->OUTSET = (1 << WS2812B_PIN); // Set the pin high
  nrf_delay_us_ticks(2);                 // Wait ~800 ns
  NRF_GPIO->OUTCLR = (1 << WS2812B_PIN); // Set the pin low
  nrf_delay_us_ticks(0);                 // Wait ~450 ns
}

void send_low(void) {
  NRF_GPIO->OUTSET = (1 << WS2812B_PIN); // Set the pin high
  nrf_delay_us_ticks(0);                 // Wait ~400 ns
  NRF_GPIO->OUTCLR = (1 << WS2812B_PIN); // Set the pin low
  nrf_delay_us_ticks(1);                 // Wait ~850 ns
}

void switch_off_LEDs(uint8_t number_of_leds) {
  for (uint8_t j = 0; j < number_of_leds; j++) {
    for (uint8_t i = 0; i < PACKET_SIZE; i++) {
      send_low();
    }
    nrf_delay_us(PACKET_DELAY);
  }
}

void WS2812B_Init(void) {
  gpio_init();
  switch_off_LEDs(MAX_LEDs_in_STRIP);
  nrf_delay_ms(50);
}

void set_LED(uint8_t rgb[3]) {
  uint8_t curr_val;
  for (uint8_t j = 0; j < 3; j++) {
    curr_val = (j == 0) ? rgb[1] : ((j == 1) ? rgb[0] : rgb[2]);
    for (uint8_t i = 0; i < 8; i++) {
      if (curr_val & (1 << (7 - i)))
        send_high();
      else
        send_low();
    }
  }
  nrf_delay_us(PACKET_DELAY);
}

void set_one_LED(uint8_t rgb[3], uint8_t led_number) {
  switch_off_LEDs(led_number - 1);
  set_LED(rgb);
  switch_off_LEDs(MAX_LEDs_in_STRIP - led_number);
}
