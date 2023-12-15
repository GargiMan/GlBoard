# Electric longboard

### GlBoard

Program `GlBoard.ino` is used for ESP32 on board acting as a Bluetooth server.
Communication data are adjusted and then exchanged between VESC (motor controller) and Bluetooth client.
Program also controls front and rear lights on board.

### GlControl

Program `GlControl.ino` is used for ESP32 on remote control acting as a Bluetooth client.
Communication data are read from GPIO pins and then sent to Bluetooth server.

# 1. Theory

## 1.1. ESP32 development board
Development board is used for prototyping and testing ESP32 applications.
ESP32 is a system on a chip that integrates the following features:
- Wi-Fi (2.4 GHz band) and Bluetooth
- Dual high performance Xtensa® 32-bit LX6 CPU cores
- Ultra Low Power co-processor
- Multiple peripherals

## 1.2. Bluetooth
Bluetooth is a wireless technology standard for exchanging data between fixed and mobile devices over short distances using short-wavelength UHF radio waves in the industrial, scientific and medical radio bands, from 2.402 GHz to 2.480 GHz, and building personal area networks (PANs).

## 1.3. GPIO
General purpose input output (GPIO) pins are used for digital communication. GPIO pins can be configured as input or output pins. Input pins can be configured with pull-up or pull-down resistors. Output pins can be configured with high or low state.

## 1.4. PWM
Pulse width modulation (PWM) is a technique for getting analog results with digital means. PWM pins can be configured with frequency and resolution.

## 1.5. ADC
Analog to digital converter (ADC) is used for converting analog signal to digital signal.

## 1.6. UART
UART is a serial communication protocol that is used for communication between two devices. UART protocol is used for communication between ESP32 and VESC. UART protocol is used for communication between ESP32 and Bluetooth module.

## 1.7. VESC
VESC is an open source ESC (electronic speed controller) that can be used for controlling BLDC (brushless DC) motors.

## 1.8. CRC
Cyclic redundancy check (CRC) is an error-detecting code commonly used in digital networks and storage devices to detect accidental changes to raw data. Blocks of data entering these systems get a short check value attached, based on the remainder of a polynomial division of their contents. On retrieval, the calculation is repeated and, in the event the check values do not match, corrective action can be taken against data corruption. CRCs can be used for error correction.

# 2. Usage

## 2.1. Prerequisites

- Installed Arduino IDE with ESP32 board library
- Updated `config.h` file according to your hardware setup
- Installed VESC Tool
  - Configured VESC HW for UART communication with specified baudrate
- Wiring according to your hardware setup

## 2.2. Hardware

- ESP32 development board
- VESC (Electronic Speed Controller)
- Battery pack with BMS (Battery Management System) that can be used for charging and discharging and suitable for VESC HW
- Front and rear lights (optional)
- Blueooth remote controller or mobile phone that uses application GlApp for controlling board

### Board setup

Configured VESC HW for UART communication needs to be connected to ESP32 UART1 pins.
For powering ESP32 development board is used VESC 5V output.
Boards lights should be connected to GPIO pins that support PWM. Pins are specified in `config.h` file.

### Controller setup

Controller requires ESP32 development board with wired joystick and buttons to GPIO pins specified in `config.h` file.
Buttons should be connected to vcc and GPIO pins with pull-down resistors.

## 2.3. Software

Program can be compiled and uploaded to ESP32 development board with Arduino IDE.
Because of problematic IDE, each program `.ino` has macro in header that must be defined before compiling and uploading program.
Commented macro in header disables other program `.ino` files from compiling and uploading and causing errors.

## 2.4. User interface

For controlling board, user can use remote control. 
After turning on controller with on/off switch, it automatically connects to board.
Bluetooth connection is indicated with blinking front light.
LED on both server and client ESP32 development board is enabled when Bluetooth connection is established.

### Accelerate / Brake joystick
- joystick up send accelerate values to board
- joystick down send brake values to board
- joystick center button can be used for switching direction of motor rotation

### Hold power button
- hold power button can lock current joystick values will keep sending them to board until hold power button is released

### Front light
- button press on controller toggles front light on/off
- front light blinks when Bluetooth client connects
- default light intensity is defined in `config.h` file

### Rear light
- button press on controller toggles rear light on/off
- rear light increases light intensity when brake is applied
- default light intensity is defined in `config.h` file

### Mode switch
- switch between modes of max current applied
- switch options are 50%, 100% and 200% of max current, which is defined in `config.h` file
- button press on controller increases current applied, in order 50% -> 100% -> 200% -> 50% -> ...

# 3. Implementation

Source codes are written in C and C++ language.
Program can be compiled and uploaded to ESP32 development board with Arduino IDE.

## 3.1. Debug

For debugging purposes, debug messages can be enabled in `config.h` file. Debug messages are printed to usb serial port with specified baud rate from `config.h` file.
Debug messages can be enabled with defined `DEBUG` macro in `config.h` file. For debug messages from VESC library `VESC_DEBUG` macro must be defined in `config.h` file.

## 3.2. UART

UART communication is implemented with Arduino library `HardwareSerial.h`. UART pins are configured with specified baud rate from `config.h` file.
ESP32 has 3 UART interfaces that can be used for serial communication. UART0 is used for USB serial communication. UART1 is used for VESC communication.

## 3.3. VESC

VESC communication is implemented with VESC firmware library `bldc.h` and VESC UART wrapper library `VescUart.h`. VESC UART wrapper library is used for simplifying VESC communication.
UART communication baud rate is defined in `config.h` file.

## 3.4. GPIO / ADC / PWM

Library `io.h` combines all GPIO, ADC and PWM control functions in one library to simplify usage and merge multiple libraries into one with similar approach for each purpose.
Specific GPIO, ADC and PWM pins are defined in `config.h` file.

## 3.5. Preferences

User preferences are stored in ESP32 flash memory with Arduino library `Preferences.h`. Preferences are stored in key-value pairs.
Keys are defined in `config.h` file. After changing project version, preferences are automatically cleared.

## 3.6. Bluetooth

Bluetooth communication is implemented with ESP32 built-in Bluetooth module and Arduino library `BluetoothSerial.h`. Bluetooth module is configured with specified baud rate and with Single Secure Pairing (SSP) mode.
Data transferred between Bluetooth server and Bluetooth client must match communication protocol described in chapter Communication protocol.

## 3.7. Communication protocol

Communication protocol packet structure should contain 1st byte for CRC, 2nd byte for flags, 3rd byte for payload length and rest of bytes for payload data.

| CRC    | Flags  | Payload Length | Payload |
|--------|--------|----------------|---------|
| 1 byte | 1 byte | 1 byte         | n bytes |

### CRC

CRC byte of message is CRC8 checksum for data transfer error detection.

### Flags

Flags byte of message is used for message type (2 bits), source node (2 bits) and command identification (3 bits).
Bit values in tables are ordered from MSB to LSB. Bit 7 is reserved and without usage.

| bit index | 7        | 6 - 4   | 3 - 2       | 1 - 0        |
|-----------|----------|---------|-------------|--------------|
| data      | reserved | command | source node | message type |

#### Message type

| Value (bit) | Value (byte) | Type     |
|-------------|--------------|----------|
| 00          | 0x00         | Request  |
| 01          | 0x01         | Response |

#### Source nodes

| Value (bit) | Value (byte) | Node   | Description |
|-------------|--------------|--------|-------------|
| 00          | 0x00         | Master | Board       |
| 01          | 0x04         | Slave  | Remote      |

#### Commands

| Value (bit) | Value (byte) | Command | Description  |
|-------------|--------------|---------|--------------|
| 000         | 0x00         | Test    | Test data    |
| 001         | 0x10         | Metrics | Metrics data |
| 010         | 0x20         | Control | Control data |
| 011         | 0x30         | Config  | Config data  |

### Payload length

Payload length byte of message is number of bytes in payload.

### Payload

Payload bytes of message are data bytes for specified command.

#### Test

Test command payload is not implemented yet.  
Command will be used for connection test.

#### Metrics

Metrics command payload contains data with metric values. Data must be in order as defined in table.

| Bytes index | Data type | Value type            | Value description                   |
|-------------|-----------|-----------------------|-------------------------------------|
| 0 - 3       | float     | Temperature Mosfet    | temperature from VESC mosfet sensor |
| 4 - 7       | float     | Temperature Motor     | temperature from motor sensor       |
| 8 - 11      | float     | Average Motor Current | average motor current               |
| 12 - 15     | float     | Average Input Current | average input current               |
| 16 - 19     | float     | Duty Cycle Now        | duty cycle now (percentage power)   |
| 20 - 23     | long      | RPM                   | motor RPM                           |
| 24 - 27     | float     | Input Voltage         | input voltage to VESC               |
| 28 - 31     | float     | Amp Hours             | amp hours used                      |
| 32 - 35     | float     | Amp Hours Charged     | amp hours charged                   |
| 36 - 39     | float     | Watt Hours            | watt hours used                     |
| 40 - 43     | float     | Watt Hours Charged    | watt hours charged                  |
| 44 - 47     | long      | Tachometer            | tachometer count                    |
| 48 - 51     | long      | Tachometer Abs        | tachometer count absolute           |

#### Control

Control command payload contains data with drive direction (1 byte) and drive power (1 byte).

| Byte index | Value type | Values     | Value description                                   |
|------------|------------|------------|-----------------------------------------------------|
| 0          | Reverse    | 0 or 255   | 0 - false, 255 - true                               |
| 1          | Power %    | -100 - 100 | positive - accelerate, negative - brake             |

#### Config

Config command payload contains configuration data. Data can contain multiple configuration parameters in any order. 
Each parameter is defined with 1 byte for parameter id and n bytes for parameter value. 
Parameter ids and values are defined in parameter table.

| Byte index | Value type | Values             | Value description |
|------------|------------|--------------------|-------------------|
| 0          | Parameter  | 0x00 - 0xFF        | Parameter id      |
| n          | Value      | parameter specific | Parameter value   |

Parameter table:

| Parameter `config.h` name | Parameter id | Value n bytes | Parameter description      |
|---------------------------|--------------|---------------|----------------------------|
| CFG_FRONT_LIGHT           | 0x01         | 0             | toggle front light on/off  |
| CFG_REAR_LIGHT            | 0x02         | 0             | toggle rear light on/off   |
| CFG_MAX_THROTTLE_POWER    | 0x10         | 1             | set max throttle current   |
| CFG_MAX_BRAKE_POWER       | 0x20         | 1             | set max brake current      |

Macros are defined in `config.h` file.  
Range for CFG_MAX_THROTTLE_POWER is 0 - MAX_THROTTLE_CURRENT.  
Range for CFG_MAX_BRAKE_POWER is 0 - MAX_BRAKE_CURRENT.  

# 4. References

Android app - GlApp: https://github.com/GargiMan/GlApp  
ESP32 datasheet: https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf  
ESP32 Arduino API: https://docs.espressif.com/projects/arduino-esp32/en/latest/libraries.html  
ESP32 ESP-IDF API: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/index.html  
ESP32 Arduino library: https://github.com/espressif/arduino-esp32  
VESC Tool: https://vesc-project.com/vesc_tool  
VESC firmware library: https://github.com/vedderb/bldc  
VESC UART wrapper library: https://github.com/SolidGeek/VescUart

# 5. Project auto evaluation and summary

**(E) Approach:** 0.8 / 1  
**(F) Functionality:** 4.5 / 5  
**(Q) Quality:** 2.5 / 3  
**(P) Presentation:** 0.8 / 1  
**(D) Documentation:** 2 / 4  

**Score summary:** (0.25 + 0.75 * F / 5) * (E + F + Q + P + D) = 9.8 / 14

# Summary

Project was successfully implemented and tested with custom controller and mobile phone.  
Due to lack of time, gyro sensor was not implemented and Bluetooth library was not implemented with ESP-IDF.  

**Known issues:**  
- Bluetooth connection SSP mode is not working properly  
- Usage of Arduino libraries reduce functionality adjustments of Bluetooth and UART communication  
