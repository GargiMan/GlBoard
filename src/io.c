/**
 * io.c
 *
 *  Created on: 14 dec 2023
 *      Author: Marek Gergel
 */

#include "io.h"

void pinSetupGPIO(int pin, int mode) {
    // Configure the GPIO pin
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = (1ULL << pin);
    io_conf.intr_type = GPIO_INTR_DISABLE;

    if (mode == OUTPUT) {
        io_conf.mode = GPIO_MODE_OUTPUT;
    } else if (mode == INPUT || mode == INPUT_PULLUP || mode == INPUT_PULLDOWN) {
        io_conf.mode = GPIO_MODE_INPUT;
    } else {
        io_conf.mode = GPIO_MODE_DISABLE;
    }

    if (mode == INPUT_PULLUP) {
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    } else {
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    }

    if (mode == INPUT_PULLDOWN) {
        io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    } else {
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    }

    gpio_config(&io_conf);
}

bool pinReadGPIO(int pin) {
    return gpio_get_level(pin) == 1;
}

void pinWriteGPIO(int pin, bool value) {
    gpio_set_level(pin, value ? 1 : 0);
}

adc_bits_width_t adc2channelResolution[10];

bool isADC1(int pin) {
    return pin == 36 || pin == 39 || pin == 32 || pin == 33 || pin == 34 || pin == 35;
}

bool isADC2(int pin) {
    return pin == 4 || pin == 0 || pin == 2 || pin == 15 || pin == 13 || pin == 12 || pin == 14 || pin == 27 || pin == 25 || pin == 26;
}

int getADC1Channel(int pin) {
    switch (pin) {
        case 36:
            return ADC1_CHANNEL_0;
        case 39:
            return ADC1_CHANNEL_3;
        case 32:
            return ADC1_CHANNEL_4;
        case 33:
            return ADC1_CHANNEL_5;
        case 34:
            return ADC1_CHANNEL_6;
        case 35:
            return ADC1_CHANNEL_7;
        default:
            return -1;
    }
}

int getADC2Channel(int pin) {
    switch (pin) {
        case 4:
            return ADC2_CHANNEL_0;
        case 0:
            return ADC2_CHANNEL_1;
        case 2:
            return ADC2_CHANNEL_2;
        case 15:
            return ADC2_CHANNEL_3;
        case 13:
            return ADC2_CHANNEL_4;
        case 12:
            return ADC2_CHANNEL_5;
        case 14:
            return ADC2_CHANNEL_6;
        case 27:
            return ADC2_CHANNEL_7;
        case 25:
            return ADC2_CHANNEL_8;
        case 26:
            return ADC2_CHANNEL_9;
        default:
            return -1;
    }
}

void pinSetupADC1(int channel, int resolution, adc_atten_t attenuation) {
    // Configure the ADC1
    adc1_config_width(resolution);
    adc1_config_channel_atten(channel, attenuation);
}

void pinSetupADC2(int channel, int resolution, adc_atten_t attenuation) {
    // Configure the ADC2
    switch (channel) {
        case ADC2_CHANNEL_0:
            adc2channelResolution[0] = resolution;
            break;
        case ADC2_CHANNEL_1:
            adc2channelResolution[1] = resolution;
            break;
        case ADC2_CHANNEL_2:
            adc2channelResolution[2] = resolution;
            break;
        case ADC2_CHANNEL_3:
            adc2channelResolution[3] = resolution;
            break;
        case ADC2_CHANNEL_4:
            adc2channelResolution[4] = resolution;
            break;
        case ADC2_CHANNEL_5:
            adc2channelResolution[5] = resolution;
            break;
        case ADC2_CHANNEL_6:
            adc2channelResolution[6] = resolution;
            break;
        case ADC2_CHANNEL_7:
            adc2channelResolution[7] = resolution;
            break;
        case ADC2_CHANNEL_8:
            adc2channelResolution[8] = resolution;
            break;
        case ADC2_CHANNEL_9:
            adc2channelResolution[9] = resolution;
            break;
        default:
            return;
        adc2_config_channel_atten(channel, attenuation);
    }
}

void pinSetupADC(int pin, int resolution, adc_atten_t attenuation) {
    if (isADC1(pin)) {
        pinSetupADC1(getADC1Channel(pin), resolution, attenuation);
    } else if (isADC2(pin)) {
        pinSetupADC2(getADC2Channel(pin), resolution, attenuation);
    } else  {
        return;
    }
}

uint32_t pinReadADC1(int channel) {
    return adc1_get_raw(channel);
}

uint32_t pinReadADC2(int channel) {
    if (channel > 9 || channel < 0) {
        return 0;
    }
    int raw_value;
    adc2_get_raw(channel, adc2channelResolution[channel], &raw_value);
    return raw_value;
}

uint32_t pinReadADC(int pin) {
    if (isADC1(pin)) {
        return pinReadADC1(getADC1Channel(pin));
    } else if (isADC2(pin)) {
        return pinReadADC2(getADC2Channel(pin));
    } else {
        return 0;
    }
}

bool isDAC1(int pin) {
    return pin == 25;
}

bool isDAC2(int pin) {
    return pin == 26;
}

void pinSetupDAC(int pin) {
    if (isDAC1(pin)) {
        dac_output_enable(DAC_CHANNEL_1);
    } else if (isDAC2(pin)) {
        dac_output_enable(DAC_CHANNEL_2);
    } else {
        return;
    }
}

void pinWriteDAC(int pin, uint32_t value) {
    if (isDAC1(pin)) {
        dac_output_voltage(DAC_CHANNEL_1, value);
    } else if (isDAC2(pin)) {
        dac_output_voltage(DAC_CHANNEL_2, value);
    } else {
        return;
    }
}

int pinPWMChannel[34];
bool pinPWMHighSpeed[34];

bool isPWM(int pin) {
    return pin == 2 || pin == 4 || pin == 5 || pin == 12 || pin == 13 || pin == 14 || pin == 15 || pin == 16 || pin == 17 || pin == 18 || pin == 19 || pin == 21 || pin == 22 || pin == 23 || pin == 25 || pin == 26 || pin == 27 || pin == 32 || pin == 33;
}

void pinSetupPWM(int pin, int channel, int timer, int frequency, int resolution, bool highSpeed) {
    // Check if pin is valid PWM pin
    if (!isPWM(pin)) {
        return;
    }

    // Check if channel is valid (0-15 for ESP32)
    if (channel < 0 || channel > 15) {
        return;
    }

    // Configure the PWM
    ledc_timer_config_t ledc_timer;
    ledc_timer.duty_resolution = resolution;
    ledc_timer.freq_hz = frequency;
    ledc_timer.speed_mode = highSpeed ? LEDC_HIGH_SPEED_MODE : LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num = timer;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel;
    ledc_channel.channel = channel;
    ledc_channel.duty = 0;
    ledc_channel.gpio_num = pin;
    ledc_channel.speed_mode = highSpeed ? LEDC_HIGH_SPEED_MODE : LEDC_LOW_SPEED_MODE;
    ledc_channel.hpoint = 0;
    ledc_channel.timer_sel = timer;
    ledc_channel_config(&ledc_channel);

    pinPWMChannel[pin] = channel;
    pinPWMHighSpeed[pin] = highSpeed;
}

void pinWritePWM(int pin, uint32_t value) {
    // Check if pin is valid PWM pin
    if (!isPWM(pin)) {
        return;
    }

    ledc_set_duty(pinPWMHighSpeed[pin] ? LEDC_HIGH_SPEED_MODE : LEDC_LOW_SPEED_MODE, pinPWMChannel[pin], value);
    ledc_update_duty(pinPWMHighSpeed[pin] ? LEDC_HIGH_SPEED_MODE : LEDC_LOW_SPEED_MODE, pinPWMChannel[pin]);
}