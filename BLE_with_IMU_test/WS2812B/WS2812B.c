#include "WS2812B.h"

/**
 * @brief Delays execution for a specified number of ticks.
 *
 * This function uses a loop of no-operation instructions to create a delay for the given number of ticks.
 *
 * @param ticks The number of ticks to delay.
 */
void nrf_delay_us_ticks(uint32_t ticks) {
  while (ticks--) {
    __NOP(); // This will burn some cycles
  }
}

/**
 * @brief Initializes the GPIO pin for the WS2812B LED strip.
 *
 * This function configures the specified pin as an output and sets it low initially.
 */
void gpio_init(void) {
  NRF_GPIO->DIRSET = (1 << WS2812B_PIN); // Configure the pin as output
  NRF_GPIO->OUTCLR = (1 << WS2812B_PIN); // Set the pin low initially
}

/**
 * @brief Sends a high signal to the WS2812B LED strip.
 *
 * This function sets the specified pin high, waits for a long period, and then sets it low, waits for a short period.
 */
void send_high(void) {
  NRF_GPIO->OUTSET = (1 << WS2812B_PIN); // Set the pin high
  nrf_delay_us_ticks(2);                 // Wait ~800 ns
  NRF_GPIO->OUTCLR = (1 << WS2812B_PIN); // Set the pin low
  nrf_delay_us_ticks(0);                 // Wait ~450 ns
}

/**
 * @brief Sends a low signal to the WS2812B LED strip.
 *
 * This function sets the specified pin high, waits for a short period, and then sets it low, waits for a long period.
 */
void send_low(void) {
  NRF_GPIO->OUTSET = (1 << WS2812B_PIN); // Set the pin high
  nrf_delay_us_ticks(0);                 // Wait ~400 ns
  NRF_GPIO->OUTCLR = (1 << WS2812B_PIN); // Set the pin low
  nrf_delay_us_ticks(1);                 // Wait ~850 ns
}
/**
 * @brief Turns off a specified number of LEDs on the WS2812B LED strip.
 *
 * This function sends low signals to turn off the specified number of LEDs.
 *
 * @param number_of_leds The number of LEDs to turn off.
 */
void switch_off_LEDs(uint8_t number_of_leds) {
  for (uint8_t j = 0; j < number_of_leds; j++) { // Loop through each LED to turn off
    for (uint8_t i = 0; i < PACKET_SIZE; i++) {  // Send the low signal for each packet size
      send_low();                                // Send low signal to the LED
    }                                            //
    nrf_delay_us(PACKET_DELAY);                  // Delay between packets
  }
}

/**
 * @brief Initializes the WS2812B LED strip.
 *
 * This function initializes the GPIO pin and turns off all LEDs on the strip.
 */
void WS2812B_Init(void) {
  gpio_init();                        // Initialize the GPIO pin
  switch_off_LEDs(MAX_LEDs_in_STRIP); // Turn off all LEDs on the strip
  nrf_delay_ms(50);                   // Delay for 50 milliseconds to ensure all LEDs are off
}

/**
 * @brief Sets the color of a single LED on the WS2812B LED strip.
 *
 * This function sends the RGB color data to a single LED.
 *
 * @param rgb An array containing the RGB values.
 */
void set_LED(uint8_t rgb[3]) {
  uint8_t curr_val;
  for (uint8_t j = 0; j < 3; j++) {                              // Loop through RGB values
    curr_val = (j == 0) ? rgb[1] : ((j == 1) ? rgb[0] : rgb[2]); // Assign the current color value (GRB format)
    for (uint8_t i = 0; i < 8; i++) {                            // Loop through each bit of the current color value
      if (curr_val & (1 << (7 - i)))                             // Check if the bit is 1
        send_high();                                             // Send high signal for bit 1
      else                                                       //
        send_low();                                              // Send low signal for bit 0
    }
  }
  nrf_delay_us(PACKET_DELAY); // Delay between packets
}

/**
 * @brief Sets the color of a specific LED on the WS2812B LED strip.
 *
 * This function sets the color of the specified LED and turns off the rest.
 *
 * @param rgb An array containing the RGB values.
 * @param led_number The index of the LED to set (1-based index).
 */
void set_one_LED(uint8_t rgb[3], uint8_t led_number) {
  switch_off_LEDs(led_number - 1);                 // Turn off LEDs before the specified LED
  set_LED(rgb);                                    // Set the color of the specified LED
  switch_off_LEDs(MAX_LEDs_in_STRIP - led_number); // Turn off LEDs after the specified LED
}
