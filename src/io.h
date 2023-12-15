/**
 * io.h
 *
 *  Created on: 14 dec 2023
 *      Author: Marek Gergel
 */

#ifndef IO_H_
#define IO_H_

#include <Arduino.h>    // for mode definitions
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <driver/gpio.h>
#include <driver/adc.h>
#include <driver/dac.h>
#include <driver/ledc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Setup GPIO pin
 * @param pin gpio pin number
 * @param mode pin mode (OUTPUT, INPUT, INPUT_PULLUP, INPUT_PULLDOWN)
 */
void pinSetupGPIO(int pin, int mode);

/**
 * Read value from GPIO pin
 * @param pin gpio pin number
 * @return value read (logic 1 - true, logic 0 - false)
 */
bool pinReadGPIO(int pin);

/**
 * Write value to GPIO pin
 * @param pin gpio pin number
 * @param value value to write (logic 1 - true, logic 0 - false)
 */
void pinWriteGPIO(int pin, bool value);

/**
 * Setup GPIO pin for ADC/DAC
 * @param pin gpio pin number
 * @param resolution ADC bit resolution ADC_WIDTH_BIT_X (9-16)
 * @param attenuation ADC attenuation (ADC_ATTEN_DB_0, ADC_ATTEN_DB_2_5, ADC_ATTEN_DB_6, ADC_ATTEN_DB_11)
 */
void pinSetupADC(int pin, int resolution, adc_atten_t attenuation);

/**
 * Read value from ADC pin
 * @param pin gpio pin number
 * @return value read
 */
uint32_t pinReadADC(int pin);

/**
 * Setup GPIO pin for DAC
 * @param pin gpio pin number
 */
void pinSetupDAC(int pin);

/**
 * Write value to DAC pin
 * @param pin gpio pin number
 * @param value value to write
 */
void pinWriteDAC(int pin, uint32_t value);

/**
 * Setup GPIO pin for PWM
 * @param pin gpio pin number
 * @param channel PWM channel (0-7)
 * @param timer PWM timer (0-3)
 * @param frequency frequency in Hz
 * @param resolution PWM bit resolution (1-20)
 * @param highSpeed high speed mode (lowe
 */
void pinSetupPWM(int pin, int channel, int timer, int frequency, int resolution, bool highSpeed);

/**
 * Write value to PWM pin
 * @param pin gpio pin number
 * @param value value to write (0-2^resolution)
 */
void pinWritePWM(int pin, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif // IO_H_
