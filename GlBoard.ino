/**
 * Glboard
 * Author: Gargi
 *
 * Change UPLOAD_DEVICE value for another device
 * File 'config.h' can be modified with other settings
 */

#include "config.h"
#include "src/VescUart.h"
#include <BluetoothSerial.h>

VescUart VESC;
BluetoothSerial BT_PORT;

void setup()
{

// debug
#if defined(DEBUG) || defined(VESC_DEBUG)
    DEBUG_PORT.begin(PORT_SPEED);
#endif // DEBUG || VESC_DEBUG

// vesc
VESC_PORT.begin(PORT_SPEED);
VESC.setSerialPort(&VESC_PORT);
#ifdef DEBUG
    DEBUG_PORT.println("VESC ready");
#endif // DEBUG
#ifdef VESC_DEBUG
    VESC.setDebugPort(&DEBUG_PORT);
#endif // VESC_DEBUG

// bluetooth
BT_PORT.begin(BT_DEVICE); // start Bluetooth with device name
#ifdef DEBUG
    DEBUG_PORT.print("Bluetooth ready: ");
    DEBUG_PORT.println(BT_DEVICE);
#endif // DEBUG

    // pin setup
    pinMode(LED_PIN_ONBOARD, OUTPUT);
}

void loop()
{
    // check if client is connected
    digitalWrite(LED_PIN_ONBOARD, BT_PORT.hasClient() ? HIGH : LOW);

    // send
    if (VESC.getVescValues())
    {

#ifdef VESC_DEBUG
        VESC.printVescValues();
#endif // VESC_DEBUG

        if (sizeof(float) != 4 || sizeof(long) != 4)
        {
#ifdef DEBUG
            DEBUG_PORT.write("Wrong data type size\n");
#endif // DEBUG
            return;
        }

        // Calculate the total size needed for data in the byte array
        size_t dataSize = 6 * sizeof(float) + 3 * sizeof(long);

        // Create a byte array to store values
        uint8_t byteArray[dataSize + 2];

        // Copy values to the byte array
        byteArray[0] = MASTER;
        byteArray[1] = dataSize;
        memcpy(byteArray + 2, &VESC.data.avgMotorCurrent, 4);
        memcpy(byteArray + 6, &VESC.data.avgInputCurrent, 4);
        memcpy(byteArray + 10, &VESC.data.dutyCycleNow, 4);
        memcpy(byteArray + 14, &VESC.data.rpm, 4);
        memcpy(byteArray + 18, &VESC.data.inpVoltage, 4);
        memcpy(byteArray + 22, &VESC.data.ampHours, 4);
        memcpy(byteArray + 26, &VESC.data.ampHoursCharged, 4);
        memcpy(byteArray + 30, &VESC.data.tachometer, 4);
        memcpy(byteArray + 34, &VESC.data.tachometerAbs, 4);

        // Send the byte array
        BT_PORT.write((const uint8_t *)byteArray, dataSize + 2);
    }

    // recieve
    if (BT_PORT.available())
    {
        int msgFrom = BT_PORT.read();
        int payloadLen = 0;

        if (msgFrom != SLAVE)
        {
            while (BT_PORT.available())
            {
                BT_PORT.read();
            }
#ifdef DEBUG
            DEBUG_PORT.write("Wrong message format\n");
#endif // DEBUG
        }
        else
        {
            payloadLen = BT_PORT.read();
        }

        if (BT_PORT.available() && payloadLen == 1)
        {
            payloadLen--;
            uint8_t data = BT_PORT.read();
            // converting value from char to -100/100
            int value = data > 127 ? data - 256 : data;

            // calculate current and send to vesc
            if (value > 0)
            {
                // negative value for reverse
                VESC.setCurrent(value * MAX_THROTTLE_POWER / 100);
            }
            else if (value < 0)
            {
                VESC.setBrakeCurrent(abs(value) * MAX_BRAKE_POWER / 100);
            }
            else
            {
                VESC.setCurrent(0);
            }
        }
        else
        {
            while (BT_PORT.available() && payloadLen > 0)
            {
                payloadLen--;
                BT_PORT.read();
            }
        }

        if (payloadLen > 0)
        {
            while (BT_PORT.available())
            {
                BT_PORT.read();
            }
#ifdef DEBUG
            DEBUG_PORT.write("Wrong payload length\n");
#endif // DEBUG
        }
    }
    else
    {

        // send data to vesc
        VESC.setCurrent(0);
    }

    delay(CYCLE_MS);
}