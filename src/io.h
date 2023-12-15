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

/**
 * Setup GPIO pin
 * @param pin gpio pin number
 * @param mode pin mode (OUTPUT, INPUT, INPUT_PULLUP, INPUT_PULLDOWN)
 */
void pinSetupGPIO(int pin, int mode);

/**
 * Read value from GPIO pin
 * @param pin gpio pin number
 * @return value read
 */
bool pinReadGPIO(int pin);

/**
 * Write value to GPIO pin
 * @param pin gpio pin number
 * @param value value to write
 */
void pinWriteGPIO(int pin, bool value);

/**
 * Setup GPIO pin for ADC/DAC
 * @param pin gpio pin number
 * @param resolution ADC resolution (9, 10, 11, 12, 13, 14, 15, 16 bits)
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

#endif // IO_H_
