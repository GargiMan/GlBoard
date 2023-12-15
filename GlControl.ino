/**
 * GlControl.ino
 *
 * Created on: 13 dec 2023
 *     Author: Marek Gergel
 *    Version: 2.0
 *
 * File 'config.h' can be modified with custom configuration.
 */

//#define UPLOAD_CONTROL
#ifdef UPLOAD_CONTROL

#include "config.h"
#include "src/io.h"
#include "src/crc8.h"
#include <BluetoothSerial.h>

BluetoothSerial BT_PORT;

uint8_t power = 0;
bool reverse = false;
bool hold = false;
bool frontLight = false;
bool rearLight = false;
bool mode = false;
int modeThrottle = MAX_THROTTLE_POWER;
int modeBrake = MAX_BRAKE_POWER;

void debug_message(const char *fmt, ...);
void throw_bt_buffer();
void verify_architecture();
void send_control_data();
void send_config_data();
void parse_received_data();
void read_inputs();

void setup() {
    // debug
#if defined(DEBUG)
    DEBUG_PORT.begin(PORT_SPEED);
#endif // DEBUG

    verify_architecture();

    debug_message("Project version: %s", PROJECT_VERSION);

    // bluetooth
    BT_PORT.begin(BT_CLIENT_DEVICE, true); // start Bluetooth with device name
    BT_PORT.setPin(BT_PIN); // set pin
    debug_message("Bluetooth ready (%s)", BT_CLIENT_DEVICE);

    // pin setup
    pinSetupGPIO(LED_ONBOARD_PIN, OUTPUT);
    pinSetupGPIO(HOLD_POWER_BUTTON_PIN, INPUT_PULLDOWN);
    pinSetupGPIO(FRONT_LIGHT_BUTTON_PIN, INPUT_PULLDOWN);
    pinSetupGPIO(REAR_LIGHT_BUTTON_PIN, INPUT_PULLDOWN);
    pinSetupGPIO(DIRECTION_SWITCH_BUTTON_PIN, INPUT_PULLDOWN);
    pinSetupADC(POWER_JOYSTICK_PIN, ADC_WIDTH_BIT_10, ADC_ATTEN_DB_11);
    debug_message("GPIO ready");

    // bluetooth connection
    BT_PORT.connect(BT_SERVER_DEVICE); // auto connect to server
    debug_message("Connecting to server (%s)", BT_SERVER_DEVICE);
}

void loop() {

    // update bluetooth connection status
    static bool isConnected = false;
    if (isConnected != BT_PORT.connected()) {
        isConnected = BT_PORT.connected();
        debug_message("Server %s", isConnected ? "connected" : "disconnected");
        pinWriteGPIO(LED_ONBOARD_PIN, BT_PORT.connected() ? true : false);
    }

    // Check if server is connected
    if (BT_PORT.connected()){
        // read buttons and joystick values
        read_inputs();

        // send control data
        static uint8_t lastPower = 0;
        if (power != 0 || lastPower != power) {
            lastPower = power;
            send_control_data();
        }

        // send config data
        if (frontLight || rearLight || mode) {
            send_config_data();
        }

        // receive
        parse_received_data();
    } else {
        delay(10000);

        // Attempt to reconnect to the server
        BT_PORT.connect(BT_SERVER_DEVICE);
        debug_message("Reconnecting to server (%s)", BT_SERVER_DEVICE);
    }
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

void parse_received_data() {
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
        if (!IS_MASTER(flags)) {
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
                //debug_message("Message received: Metrics message received");
                break;
            case CONTROL_FLAG:
                debug_message("Message received: Control message received");
                break;
            case CONFIG_FLAG:
                debug_message("Message received: Config message received");
                break;
            default:
                throw_bt_buffer();
                debug_message("Message received: Invalid message command");
                delete[] packet;
                return;
        }

        delete[] packet;
    }
}

void send_control_data() {
    // Create a byte array to store values (size of payload + 1 byte CRC + 1 byte flags + 1 byte size of payload)
    uint8_t byteArray[5];

    // Set flags
    SET_SOURCE_NODE(byteArray[1], SLAVE_FLAG);
    SET_MESSAGE_TYPE(byteArray[1], REQUEST_FLAG);
    SET_COMMAND(byteArray[1], CONTROL_FLAG);

    // Set size of payload
    byteArray[2] = 2;

    // Copy payload to the byte array
    byteArray[3] = reverse ? 0xFF : 0x00;
    byteArray[4] = power;

    // Calculate CRC
    byteArray[0] = crc8(byteArray + 1, 4);

    // Send the byte array
    BT_PORT.write((const uint8_t *)byteArray, 5);
    BT_PORT.flush();
    //debug_message("Control message sent: %s, %d", reverse ? "reverse" : "forward", power);
}

void send_config_data() {
    uint8_t payloadLength = (frontLight ? 1 : 0) + (rearLight ? 1 : 0) + (mode ? 4 : 0);

    // Create a byte array to store values (size of payload + 1 byte CRC + 1 byte flags + 1 byte size of payload)
    uint8_t byteArray[payloadLength + 3];

    // Set flags
    SET_SOURCE_NODE(byteArray[1], SLAVE_FLAG);
    SET_MESSAGE_TYPE(byteArray[1], REQUEST_FLAG);
    SET_COMMAND(byteArray[1], CONFIG_FLAG);

    // Set size of payload
    byteArray[2] = payloadLength;


    // Copy payload to the byte array
    int lastPayloadIndex = 3;

    if (frontLight) {
        frontLight = false;
        byteArray[lastPayloadIndex++] = CFG_FRONT_LIGHT;
    }

    if (rearLight) {
        rearLight = false;
        byteArray[lastPayloadIndex++] = CFG_REAR_LIGHT;
    }

    if (mode) {
        mode = false;
        byteArray[lastPayloadIndex++] = CFG_MAX_THROTTLE_POWER;
        byteArray[lastPayloadIndex++] = modeThrottle;
        byteArray[lastPayloadIndex++] = CFG_MAX_BRAKE_POWER;
        byteArray[lastPayloadIndex++] = modeBrake;
    }

    // Calculate CRC
    byteArray[0] = crc8(byteArray + 1, payloadLength + 2);

    // Send the byte array
    BT_PORT.write((const uint8_t *)byteArray, payloadLength + 3);
    BT_PORT.flush();
}

void read_inputs() {
    // read hold power button
    static unsigned long lastTimeHoldButtonPressed = 0;
    if (pinReadGPIO(HOLD_POWER_BUTTON_PIN)) {
        lastTimeHoldButtonPressed = millis();
        hold = true;
        debug_message("Hold button pressed");
    } else if (hold && !pinReadGPIO(HOLD_POWER_BUTTON_PIN) && millis() - lastTimeHoldButtonPressed > BUTTON_DEBOUNCE_TIME) {
        hold = false;
    }

    // read front light button
    static unsigned long lastTimeFrontLightButtonPressed = 0;
    if (pinReadGPIO(FRONT_LIGHT_BUTTON_PIN) && millis() - lastTimeFrontLightButtonPressed > BUTTON_HOLD_TIME) {
        lastTimeFrontLightButtonPressed = millis();
        frontLight = true;
        debug_message("Front light button pressed");
    }

    // read rear light button
    static unsigned long lastTimeRearLightButtonPressed = 0;
    if (pinReadGPIO(REAR_LIGHT_BUTTON_PIN) && millis() - lastTimeRearLightButtonPressed > BUTTON_HOLD_TIME) {
        lastTimeRearLightButtonPressed = millis();
        rearLight = true;
        debug_message("Rear light button pressed");
    }

    // read mode button
    static unsigned long lastTimeModeSwitchButtonPressed = 0;
    if (pinReadGPIO(MODE_SWITCH_BUTTON_PIN) && millis() - lastTimeModeSwitchButtonPressed > BUTTON_HOLD_TIME) {
        lastTimeModeSwitchButtonPressed = millis();
        mode = true;
        switch (modeThrottle) {
            case MAX_THROTTLE_POWER / 2:
                debug_message("Max throttle power set to %d", MAX_THROTTLE_POWER);
                modeThrottle = MAX_THROTTLE_POWER;
                break;
            case MAX_THROTTLE_POWER:
                debug_message("Max throttle power set to %d", MAX_THROTTLE_POWER * 2);
                modeThrottle = MAX_THROTTLE_POWER * 2;
                break;
            case MAX_THROTTLE_POWER * 2:
                debug_message("Max throttle power set to %d", MAX_THROTTLE_POWER / 2);
                modeThrottle = MAX_THROTTLE_POWER / 2;
                break;
        }
        switch (modeBrake) {
            case MAX_BRAKE_POWER / 2:
                debug_message("Max brake power set to %d", MAX_BRAKE_POWER);
                modeBrake = MAX_BRAKE_POWER;
                break;
            case MAX_BRAKE_POWER:
                debug_message("Max brake power set to %d", MAX_BRAKE_POWER * 2);
                modeBrake = MAX_BRAKE_POWER * 2;
                break;
            case MAX_BRAKE_POWER * 2:
                debug_message("Max brake power set to %d", MAX_BRAKE_POWER / 2);
                modeBrake = MAX_BRAKE_POWER / 2;
                break;
        }
        debug_message("Mode switch button pressed");
    }

    // read direction button
    static unsigned long lastTimeDirectionSwitchButtonPressed = 0;
    if (pinReadGPIO(DIRECTION_SWITCH_BUTTON_PIN) && millis() - lastTimeDirectionSwitchButtonPressed > BUTTON_HOLD_TIME && !hold) {
        lastTimeDirectionSwitchButtonPressed = millis();
        reverse = !reverse;
        debug_message("Direction switch button pressed");
    }

    // read power joystick
    if (!hold) {
        int joystick = pinReadADC(POWER_JOYSTICK_PIN) / 4;    // analog read with 10 bit resolution, divide by 4 to get 8 bit resolution
        if (joystick < (uint8_t)(127.0 * (1.0 - JOYSTICK_DEADZONE))) {
            power = 255 - abs((float)(joystick - 127.0) / 127.0 * 100.0);
        } else if (joystick > (uint8_t)(127.0 * (1.0 + JOYSTICK_DEADZONE))) {
            power = (float)(joystick - 127.0) / 128.0 * 100.0;
        } else {
            power = 0;
        }
    }
}

#endif // UPLOAD_CONTROL