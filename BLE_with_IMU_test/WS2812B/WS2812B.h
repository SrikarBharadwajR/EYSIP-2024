#ifndef _WS2812B_H_
#define _WS2812B_H_

#include "nrf.h"       // Include Nordic Semiconductor's hardware definitions
#include "nrf_delay.h" // Include Nordic Semiconductor's delay functions

// Define constants for WS2812B LED strip
#define WS2812B_PIN 17       ///< GPIO pin connected to the WS2812B data line
#define MAX_LEDs_in_STRIP 12 ///< Maximum number of LEDs in the strip
#define PACKET_SIZE 24       ///< Number of bits per LED (8 bits for each color: R, G, B)
#define PACKET_DELAY 10      ///< Delay between packets in microseconds

/**
 * @brief Delays execution for a specified number of ticks.
 *
 * This function uses a loop of no-operation instructions to create a delay for the given number of ticks.
 *
 * @param ticks The number of ticks to delay.
 */
void nrf_delay_us_ticks(uint32_t ticks);

/**
 * @brief Initializes the GPIO pin for the WS2812B LED strip.
 *
 * This function configures the specified pin as an output and sets it low initially.
 */
void gpio_init(void);

/**
 * @brief Sends a high signal to the WS2812B LED strip.
 *
 * This function sets the specified pin high, waits for a short period, and then sets it low.
 */
void send_high(void);

/**
 * @brief Sends a low signal to the WS2812B LED strip.
 *
 * This function sets the specified pin high, waits for a short period, and then sets it low.
 */
void send_low(void);

/**
 * @brief Turns off a specified number of LEDs on the WS2812B LED strip.
 *
 * This function sends low signals to turn off the specified number of LEDs.
 *
 * @param number_of_leds The number of LEDs to turn off.
 */
void switch_off_LEDs(uint8_t number_of_leds);

/**
 * @brief Initializes the WS2812B LED strip.
 *
 * This function initializes the GPIO pin and turns off all LEDs on the strip.
 */
void WS2812B_Init(void);

/**
 * @brief Sets the color of a specific LED on the WS2812B LED strip.
 *
 * This function sets the color of the specified LED and turns off the rest.
 *
 * @param rgb An array containing the RGB values.
 * @param led_number The index of the LED to set (1-based index).
 */
void set_one_LED(uint8_t rgb[3], uint8_t led_number);

/**
 * @brief Sets the color of a single LED on the WS2812B LED strip.
 *
 * This function sends the RGB color data to a single LED.
 *
 * @param rgb An array containing the RGB values.
 */
void set_LED(uint8_t rgb[3]);

#endif // _WS2812B_H_
