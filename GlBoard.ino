/**
 * Glboard
 * Author: Gargi
 *
 * Change UPLOAD_DEVICE value for another device
 * File 'config.h' can be modified with other settings
 */

#include "config.h"
#include "libs/VescUart.h"
#include <BluetoothSerial.h>

VescUart VESC;
BluetoothSerial BT_PORT;

void setup()
{

// debug
#ifdef DEBUG
    DEBUG_PORT.begin(PORT_SPEED);
#endif // DEBUG

// vesc
#ifdef VESC_CONTROL_PORT
    VESC_CONTROL_PORT.begin(PORT_SPEED);
    VESC.setSerialPort(&VESC_CONTROL_PORT);
    // VESC.setDebugPort(&VESC_DEBUG_PORT);
#endif // VESC_CONTROL_PORT

// bluetooth
#ifdef BT_PORT
    BT_PORT.begin(BT_DEVICE); // start Bluetooth with device name
#ifdef DEBUG
    DEBUG_PORT.print("Bluetooth ready: ");
    DEBUG_PORT.println(BT_DEVICE);
#endif // DEBUG
#endif // BT_PORT

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
        // float vamp = VESC.data.avgInputCurrent;
        // float vduty = abs(VESC.data.dutyCycleNow);
        float vvolt = VESC.data.inpVoltage;
        // long vtach = VESC.data.tachometerAbs;
        // long vrpm = abs(VESC.data.rpm);

        char buffer[10];
        dtostrf(vvolt, 2, 2, buffer);
        String str = String(buffer);

        String payload = "";
        payload += MASTER;
        payload += str.length();
        payload += str;
        BT_PORT.write((const uint8_t *)payload.c_str(), payload.length());
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
            DEBUG_PORT.write("\nWrong message format\n");
        }
        else
        {
            payloadLen = BT_PORT.read();
        }

        if (BT_PORT.available() && payloadLen == 1)
        {
            payloadLen--;
            char data = BT_PORT.read();
            // converting value from char to -100/100
            int value = data > 127 ? data - 256 : data;
            DEBUG_PORT.print(value);

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
                DEBUG_PORT.write(BT_PORT.read());
            }
        }

        if (payloadLen > 0)
        {
            while (BT_PORT.available())
            {
                BT_PORT.read();
            }
            DEBUG_PORT.write("\nWrong payload length\n");
        }
        else
        {
            if (msgFrom == SLAVE)
            {
                DEBUG_PORT.write("\n");
            }
        }
    }
    else
    {

        // send data to vesc
        VESC.setCurrent(0);
    }

    delay(CYCLE_MS);
}