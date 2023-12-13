/**
 * Glboard
 * Author: Marek Gergel
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

VescUart VESC;
BluetoothSerial BT_PORT;

bool hasClient = false;

void debug_message(const char *fmt, ...);
void read_rest_of_buffer();
void verify_architecture();
void send_metrics_data();
void parse_recieved_data();

void setup() {

    // debug
#if defined(DEBUG) || defined(VESC_DEBUG)
    DEBUG_PORT.begin(PORT_SPEED);
#ifdef VESC_DEBUG
    VESC.setDebugPort(&DEBUG_PORT);
#endif // VESC_DEBUG
#endif // DEBUG || VESC_DEBUG

    // vesc
    VESC_PORT.begin(PORT_SPEED);
    VESC.setSerialPort(&VESC_PORT);
    debug_message("VESC ready");

    // bluetooth
    BT_PORT.begin(BT_DEVICE); // start Bluetooth with device name
    debug_message("Bluetooth ready: %s", BT_DEVICE);

    // pin setup
    pinMode(LED_PIN_ONBOARD, OUTPUT);
}

void loop() {
    verify_architecture();

    // check if client is connected
    if (hasClient != BT_PORT.hasClient()) {
        hasClient = BT_PORT.hasClient();
        debug_message("Client %s", hasClient ? "connected" : "disconnected");
        digitalWrite(LED_PIN_ONBOARD, BT_PORT.hasClient() ? HIGH : LOW);
    }

    // send metrics data
    send_metrics_data();

    // recieve
    parse_recieved_data();
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
    // Check if client is connected
    if (!BT_PORT.hasClient()) return;

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

void parse_recieved_data() {
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
                        debug_message("Wrong message payload length");
                        delete[] packet;
                        return;
                    }

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
                debug_message("Config message recieved");
                break;
            default:
                debug_message("Wrong message command");
                delete[] packet;
                return;
        }

        delete[] packet;
    } else {

        // reset current
        VESC.setCurrent(0);
    }
}