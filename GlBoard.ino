/**
 * Glboard.ino
 *
 * Created on: 13 jul 2020
 *     Author: Marek Gergel
 *    Version: 2.0
 *
 * File 'config.h' can be modified with custom configuration.
 */

//Arduino.h is included in compilation process
//#include "C:/Users/mage2/Appdata/Local/Arduino15/packages/esp32/hardware/esp32/2.0.14/cores/esp32/Arduino.h"
//#include "C:/Users/mage2/Appdata/Local/Arduino15/packages/esp32/hardware/esp32/2.0.14/libraries/BluetoothSerial/src/BluetoothSerial.h"

#include "config.h"
#include "src/VescUart.h"
#include "src/crc8.h"
#include <BluetoothSerial.h>
#include <Preferences.h>

VescUart VESC;
BluetoothSerial BT_PORT;
Preferences preferences;

bool hasClient = false;
int maxLightPower = pow(2, PWM_RESOLUTION) - 1;

void debug_message(const char *fmt, ...);
void read_rest_of_buffer();
void verify_architecture();
void send_metrics_data();
void parse_received_data();
bool parse_config_data(uint8_t* payload, uint8_t payloadLen);
void update_lights();

void setup() {
    // debug
#if defined(DEBUG) || defined(VESC_DEBUG)
    DEBUG_PORT.begin(PORT_SPEED);
#ifdef VESC_DEBUG
    VESC.setDebugPort(&DEBUG_PORT);
#endif // VESC_DEBUG
#endif // DEBUG || VESC_DEBUG

    verify_architecture();

    debug_message("Project version: %s", PROJECT_VERSION);

    // preferences
    preferences.begin(PROJECT_NAME, RO_MODE);
    if (preferences.isKey(KEY_PROJECT_VERSION) && preferences.getString(KEY_PROJECT_VERSION) == PROJECT_VERSION) {
        debug_message("Preferences loaded");
    } else { // preferences not found or project version changed
        preferences.end();
        preferences.begin(PROJECT_NAME, RW_MODE);
        preferences.clear();
        preferences.putString(KEY_PROJECT_VERSION, PROJECT_VERSION);
        preferences.putInt(KEY_MAX_THROTTLE_POWER, MAX_THROTTLE_POWER);
        preferences.putInt(KEY_MAX_BRAKE_POWER, MAX_BRAKE_POWER);
        preferences.putBool(KEY_FRONT_LIGHT_ENABLED, false);
        preferences.putFloat(KEY_FRONT_LIGHT_POWER, FRONT_LIGHT_POWER);
        preferences.putBool(KEY_REAR_LIGHT_ENABLED, false);
        preferences.putFloat(KEY_REAR_LIGHT_POWER, REAR_LIGHT_POWER);
        preferences.end();
        preferences.begin(PROJECT_NAME, RO_MODE);
        debug_message("Preferences recreated");
    }

    // vesc
    VESC_PORT.begin(PORT_SPEED);
    VESC.setSerialPort(&VESC_PORT);
    debug_message("VESC ready");

    // bluetooth
    BT_PORT.begin(BT_DEVICE); // start Bluetooth with device name
    debug_message("Bluetooth ready (%s)", BT_DEVICE);

    // pin setup
    pinMode(LED_ONBOARD_PIN, OUTPUT);
    ledcSetup(FRONT_LIGHT_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(FRONT_LIGHT_PIN, FRONT_LIGHT_CHANNEL);
    ledcSetup(REAR_LIGHT_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(REAR_LIGHT_PIN, REAR_LIGHT_CHANNEL);
    debug_message("GPIO ready");
}

void loop() {

    // update bluetooth connection status
    if (hasClient != BT_PORT.hasClient()) {
        hasClient = BT_PORT.hasClient();
        debug_message("Client %s", hasClient ? "connected" : "disconnected");
        digitalWrite(LED_ONBOARD_PIN, BT_PORT.hasClient() ? HIGH : LOW);
    }

    // Check if client is connected
    if (BT_PORT.hasClient()){
        // send metrics data
        send_metrics_data();

        // receive
        parse_recieved_data();
    }

    // lights
    update_lights();
}

/**
 * Print debug message to the debug port.
 * @param message
 */
void debug_message(const char *fmt, ...) {
#ifdef DEBUG
    static char buffer[1024];
    va_list args;
    va_start(args, fmt);
    int results = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    DEBUG_PORT.print(buffer);
    DEBUG_PORT.print("\n");
#endif // DEBUG
    return;
}

/**
 * Read the rest of the buffer.
 */
void read_rest_of_buffer() {
    while (BT_PORT.available()) {
        BT_PORT.read();
    }
}

/**
 * Verify that the architecture is valid for this program.
 * If not, the program will be stuck in an infinite loop.
 */
void verify_architecture() {
    // verify size of data types
    while (sizeof(float) != 4 || sizeof(long) != 4) {
        debug_message("Wrong data type size \n");
    }
}

/**
 * Send data from VESC to the client.
 */
void send_metrics_data() {
    static unsigned long lastTime = 0;
    if (millis() - lastTime < METRICS_UPDATE_DELAY) return;

    if (VESC.getVescValues()) {
        // Correct vesc values for current motor poles and phases
        VESC.data.rpm /= MOTOR_PHASES * MOTOR_POLES;
        VESC.data.tachometer /= MOTOR_PHASES * MOTOR_POLES;
        VESC.data.tachometerAbs /= MOTOR_PHASES * MOTOR_POLES;

#ifdef VESC_DEBUG
        VESC.printVescValues();
#endif // VESC_DEBUG

        // Calculate the total size needed for data in the byte array
        size_t vescDataSize = 10 * sizeof(float) + 3 * sizeof(long);

        // Create a byte array to store values (size of payload + 1 byte CRC + 1 byte flags + 1 byte size of payload)
        uint8_t byteArray[vescDataSize + 3];

        // Set flags
        SET_SOURCE_NODE(byteArray[1], MASTER_FLAG);
        SET_MESSAGE_TYPE(byteArray[1], RESPONSE_FLAG);
        SET_COMMAND(byteArray[1], METRICS_FLAG);

        // Set size of payload
        byteArray[2] = vescDataSize;

        // Copy payload to the byte array
        int startIndex = 3, dataIndex = 0;
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.tempMosfet, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.tempMotor, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.avgMotorCurrent, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.avgInputCurrent, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.dutyCycleNow, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.rpm, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.inpVoltage, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.ampHours, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.ampHoursCharged, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.wattHours, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.wattHoursCharged, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.tachometer, 4);
        memcpy(byteArray + startIndex + dataIndex++ * 4, &VESC.data.tachometerAbs, 4);

        // Calculate CRC
        byteArray[0] = crc8(byteArray + 1, vescDataSize + 2);

        // Send the byte array
        BT_PORT.write((const uint8_t *)byteArray, vescDataSize + 3);
    }
}

void parse_received_data() {
    static unsigned long lastTimeValidControlReceived = 0;
    static bool resetCurrent = false;

    if (BT_PORT.available()) {
        uint8_t crc = BT_PORT.read();
        uint8_t flags = BT_PORT.read();
        uint8_t payloadLen = BT_PORT.read();

        // Check CRC
        uint8_t* packet = new uint8_t[payloadLen + 2];
        packet[0] = flags;
        packet[1] = payloadLen;
        if (payloadLen > 0) {
            BT_PORT.readBytes(packet + 2, payloadLen);
        }
        if (crc != crc8(packet, payloadLen + 2)) {
            read_rest_of_buffer();
            debug_message("Wrong message crc value (message 0x%02X, calculated 0x%02X)", crc, crc8(packet, payloadLen + 2));
            delete[] packet;
            return;
        }

        // Check source node
        if (!IS_SLAVE(flags)) {
            read_rest_of_buffer();
            debug_message("Wrong message source node (flags 0x%02X)", flags);
            delete[] packet;
            return;
        }

        uint8_t* payload = packet + 2;

        switch (GET_COMMAND(flags)) {
            case TEST_FLAG:
                debug_message("Test message recieved");
                break;
            case METRICS_FLAG:
                debug_message("Metrics message recieved");
                break;
            case CONTROL_FLAG:
                if (IS_REQUEST(flags)) {
                    if (payloadLen != 2) {
                        read_rest_of_buffer();
                        debug_message("Wrong control payload");
                        delete[] packet;
                        return;
                    }

                    resetCurrent = true;
                    lastTimeValidControlReceived = millis();

                    // converting direction from char to false/true (forward/reverse)
                    bool direction = payload[0] != 0;
                    // converting current value from char to -100/100
                    int8_t current = payload[1];

                    // calculate current and send to vesc
                    if (current > 0) {
                        if (direction) { // reverse
                            VESC.setCurrent(-current * MAX_THROTTLE_POWER / 100);
                        } else { // forward
                            VESC.setCurrent(current * MAX_THROTTLE_POWER / 100);
                        }
                    } else if (current < 0) {
                        VESC.setBrakeCurrent(abs(current) * MAX_BRAKE_POWER / 100);
                    } else {
                        VESC.setCurrent(0);
                    }
                }
                break;
            case CONFIG_FLAG:
                if (IS_REQUEST(flags)) {
                    bool success = parse_config_data(payload, payloadLen);
                    if (!success) {
                        read_rest_of_buffer();
                        debug_message("Wrong config payload");
                        delete[] packet;
                        return;
                    }
                }
                break;
            default:
                debug_message("Wrong message command");
                delete[] packet;
                return;
        }

        delete[] packet;
    } else {
        // reset current if no control message received for a while
        if (resetCurrent) {
            if (millis() - lastTimeValidControlReceived > CONTROL_TIMEOUT) {
                VESC.setCurrent(0);
            }
        }
    }
}

bool parse_config_data(uint8_t* payload, uint8_t payloadLen) {
    int payloadIndex = 0;
    bool error = false;

    // check if payload is not empty and reopens preferences for writing
    if (payloadLen != 0) {
        preferences.end();
        preferences.begin(PROJECT_NAME, RW_MODE);

        while (payloadIndex < payloadLen && !error) {
            uint8_t config = payload[payloadIndex++];
            switch (config) {
                case CFG_FRONT_LIGHT:
                    preferences.putBool(KEY_FRONT_LIGHT_ENABLED, !preferences.getBool(KEY_FRONT_LIGHT_ENABLED));
                    debug_message("Front light %s", preferences.getBool(KEY_FRONT_LIGHT_ENABLED) ? "enabled" : "disabled");
                    continue;
                case CFG_REAR_LIGHT:
                    preferences.putBool(KEY_REAR_LIGHT_ENABLED, !preferences.getBool(KEY_REAR_LIGHT_ENABLED));
                    debug_message("Rear light %s", preferences.getBool(KEY_REAR_LIGHT_ENABLED) ? "enabled" : "disabled");
                    continue;
                case CFG_MAX_THROTTLE_POWER:
                    if (payloadIndex + 1 > payloadLen) {
                        error = true;
                        break;
                    }
                    // check if value is different from current value, reduce number of writes to flash memory
                    if (payload[payloadIndex] != preferences.getInt(KEY_MAX_THROTTLE_POWER)) {
                        preferences.putInt(KEY_MAX_THROTTLE_POWER, payload[payloadIndex]);
                        debug_message("Max throttle power set to %d", preferences.getInt(KEY_MAX_THROTTLE_POWER));
                    }
                    payloadIndex++;
                    continue;
                case CFG_MAX_BRAKE_POWER:
                    if (payloadIndex + 1 > payloadLen) {
                        error = true;
                        break;
                    }
                    // check if value is different from current value, reduce number of writes to flash memory
                    if (payload[payloadIndex] != preferences.getInt(KEY_MAX_BRAKE_POWER)) {
                        preferences.putInt(KEY_MAX_BRAKE_POWER, payload[payloadIndex]);
                        debug_message("Max brake power set to %d", preferences.getInt(KEY_MAX_BRAKE_POWER));
                    }
                    payloadIndex++;
                    continue;
                default:
                    error = true;
                    break;
            }
        }

        // close preferences for writing and reopen for reading
        preferences.end();
        preferences.begin(PROJECT_NAME, RO_MODE);
    }

    return !error;
}

void update_lights() {
    if (preferences.getBool(KEY_FRONT_LIGHT_ENABLED)) {
        ledcWrite(FRONT_LIGHT_CHANNEL, maxLightPower * preferences.getFloat(KEY_FRONT_LIGHT_POWER));
    } else {
        ledcWrite(FRONT_LIGHT_CHANNEL, 0);
    }
    if (preferences.getBool(KEY_REAR_LIGHT_ENABLED)) {
        ledcWrite(REAR_LIGHT_CHANNEL, maxLightPower * preferences.getFloat(KEY_REAR_LIGHT_POWER));
    } else {
        ledcWrite(REAR_LIGHT_CHANNEL, 0);
    }
}