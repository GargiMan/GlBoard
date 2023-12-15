/**
 * GlBoard.ino
 *
 * Created on: 13 jul 2020
 *     Author: Marek Gergel
 *    Version: 2.0
 *
 * File 'config.h' can be modified with custom configuration.
 */

#define UPLOAD_BOARD
#ifdef UPLOAD_BOARD

#include "config.h"
#include "src/io.h"
#include "src/crc8.h"
#include "src/VescUart.h"
#include <BluetoothSerial.h>
#include <Preferences.h>

VescUart VESC;
BluetoothSerial BT_PORT;
Preferences preferences;

bool braking = false;
int maxLightPower = pow(2, PWM_RESOLUTION) - 1;

void debug_message(const char *fmt, ...);
void throw_bt_buffer();
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
    preferences.begin(SERVER_PROJECT_NAME, RO_MODE);
    if (preferences.isKey(KEY_PROJECT_VERSION) && preferences.getString(KEY_PROJECT_VERSION) == PROJECT_VERSION) {
        debug_message("Preferences loaded");
    } else { // preferences not found or project version changed
        preferences.end();
        preferences.begin(SERVER_PROJECT_NAME, RW_MODE);
        preferences.clear();
        preferences.putString(KEY_PROJECT_VERSION, PROJECT_VERSION);
        preferences.putInt(KEY_MAX_THROTTLE_POWER, MAX_THROTTLE_POWER);
        preferences.putInt(KEY_MAX_BRAKE_POWER, MAX_BRAKE_POWER);
        preferences.putBool(KEY_FRONT_LIGHT_ENABLED, false);
        preferences.putFloat(KEY_FRONT_LIGHT_POWER, FRONT_LIGHT_POWER);
        preferences.putBool(KEY_REAR_LIGHT_ENABLED, false);
        preferences.putFloat(KEY_REAR_LIGHT_POWER, REAR_LIGHT_POWER);
        preferences.end();
        preferences.begin(SERVER_PROJECT_NAME, RO_MODE);
        debug_message("Preferences recreated");
    }

    // vesc
    VESC_PORT.begin(PORT_SPEED);
    VESC.setSerialPort(&VESC_PORT);
    debug_message("VESC ready");

    // bluetooth
    BT_PORT.begin(BT_SERVER_DEVICE); // start Bluetooth with device name
    BT_PORT.setPin(BT_PIN); // set pin
    BT_PORT.enableSSP(); // enable single secure pairing mode
    debug_message("Bluetooth ready (%s)", BT_SERVER_DEVICE);

    // pin setup
    pinSetupGPIO(LED_ONBOARD_PIN, OUTPUT);
    pinSetupPWM(FRONT_LIGHT_PIN, 0, 0, PWM_FREQUENCY, PWM_RESOLUTION, false);
    pinSetupPWM(REAR_LIGHT_PIN, 1, 1, PWM_FREQUENCY, PWM_RESOLUTION, false);
    debug_message("GPIO ready");
}

void loop() {

    // update bluetooth connection status
    static bool hasClient = false;
    if (hasClient != BT_PORT.hasClient()) {
        hasClient = BT_PORT.hasClient();
        debug_message("Client %s", hasClient ? "connected" : "disconnected");
        digitalWrite(LED_ONBOARD_PIN, BT_PORT.hasClient() ? true : false);

        // blink front light when client connects
        if (hasClient) {
            pinWritePWM(FRONT_LIGHT_PIN, 0);
            delay(50);
            pinWritePWM(FRONT_LIGHT_PIN, maxLightPower * preferences.getFloat(KEY_FRONT_LIGHT_POWER));
            delay(50);
            pinWritePWM(FRONT_LIGHT_PIN, 0);
            delay(50);
            pinWritePWM(FRONT_LIGHT_PIN, maxLightPower * preferences.getFloat(KEY_FRONT_LIGHT_POWER));
            delay(50);
            pinWritePWM(FRONT_LIGHT_PIN, 0);
        }
    }

    // Check if client is connected
    if (BT_PORT.hasClient()){
        // send metrics data
        send_metrics_data();

        // receive
        parse_received_data();
    }

    // lights
    update_lights();
}

/**
 * Print debug message to the debug port.
 * @param fmt Format string.
 * @param ... Arguments.
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
 * Read the rest of the buffer from bluetooth and throw it away.
 */
void throw_bt_buffer() {
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
    static unsigned long lastTimeMetricsUpdated = 0;
    if (millis() - lastTimeMetricsUpdated < METRICS_UPDATE_DELAY) return;

    lastTimeMetricsUpdated = millis();

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
        BT_PORT.flush();
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
            throw_bt_buffer();
            debug_message("Message received: CRC mismatch (message 0x%02X, calculated 0x%02X)", crc, crc8(packet, payloadLen + 2));
            delete[] packet;
            return;
        }

        // Check source node
        if (!IS_SLAVE(flags)) {
            throw_bt_buffer();
            debug_message("Message received: Invalid source node (flags 0x%02X)", flags);
            delete[] packet;
            return;
        }

        uint8_t* payload = packet + 2;

        switch (GET_COMMAND(flags)) {
            case TEST_FLAG:
                debug_message("Message received: Test message received");
                break;
            case METRICS_FLAG:
                debug_message("Message received: Metrics message received");
                break;
            case CONTROL_FLAG:
                if (IS_REQUEST(flags)) {
                    if (payloadLen != 2) {
                        throw_bt_buffer();
                        debug_message("Message received: Invalid control payload");
                        delete[] packet;
                        return;
                    }

                    resetCurrent = true;
                    lastTimeValidControlReceived = millis();

                    // converting direction from char to false/true (forward/reverse)
                    bool reverse = payload[0] != 0;
                    // converting current value from char to -100/100
                    int8_t current = payload[1];

                    // calculate current and send to vesc
                    if (current > 0) {
                        braking = false;
                        if (reverse) { // reverse
                            VESC.setCurrent(-current * MAX_THROTTLE_POWER / 100);
                        } else { // forward
                            VESC.setCurrent(current * MAX_THROTTLE_POWER / 100);
                        }
                    } else if (current < 0) {
                        braking = true;
                        VESC.setBrakeCurrent(abs(current) * MAX_BRAKE_POWER / 100);
                    } else {
                        braking = false;
                        VESC.setCurrent(0);
                    }
                }
                break;
            case CONFIG_FLAG:
                if (IS_REQUEST(flags)) {
                    bool success = parse_config_data(payload, payloadLen);
                    if (!success) {
                        throw_bt_buffer();
                        debug_message("Message received: Invalid config payload");
                        delete[] packet;
                        return;
                    }
                }
                break;
            default:
                throw_bt_buffer();
                debug_message("Message received: Invalid message command");
                delete[] packet;
                return;
        }

        //debug_message("Message received: OK, millis %lu", millis());

        delete[] packet;
    } else {
        // reset current if no control message received for a while
        if (resetCurrent) {
            if (millis() - lastTimeValidControlReceived > CONTROL_TIMEOUT) {
                braking = false;
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
        preferences.begin(SERVER_PROJECT_NAME, RW_MODE);

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
        preferences.begin(SERVER_PROJECT_NAME, RO_MODE);
    }

    return !error;
}

void update_lights() {
    if (preferences.getBool(KEY_FRONT_LIGHT_ENABLED)) {
        pinWritePWM(FRONT_LIGHT_PIN, maxLightPower * preferences.getFloat(KEY_FRONT_LIGHT_POWER));
    } else {
        pinWritePWM(FRONT_LIGHT_PIN, 0);
    }
    if (preferences.getBool(KEY_REAR_LIGHT_ENABLED)) {
        if (braking) {
            pinWritePWM(REAR_LIGHT_PIN, maxLightPower);
        } else {
            pinWritePWM(REAR_LIGHT_PIN, maxLightPower * preferences.getFloat(KEY_REAR_LIGHT_POWER));
        }
    } else {
        pinWritePWM(REAR_LIGHT_PIN, 0);
    }
}

#endif // UPLOAD_BOARD