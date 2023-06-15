/**
 * Glboard
 * Author: Gargi
 *
 * Change UPLOAD_DEVICE value for another device
 * File 'config.h' can be modified with other settings
 */

#include "config.h"
#include "BluetoothSerial.h"
#include "VescUart.h"

VescUart VESC;
BluetoothSerial SerialBT;

void setup()
{

    // debug
    #ifdef DEBUG
        DEBUG_PORT.begin(PORT_SPEED);
    #endif // DEBUG

    //vesc
    #ifdef VESC_CONTROL_PORT
        VESC_CONTROL_PORT.begin(PORT_SPEED);
        VESC.setSerialPort(&VESC_CONTROL_PORT);
        //VESC.setDebugPort(&VESC_DEBUG_PORT);
    #endif // VESC_CONTROL_PORT

    //bluetooth
    #ifdef BT_PORT
        BT_PORT.begin(BT_DEVICE); // Bluetooth device name
        #ifdef DEBUG
            DEBUG_PORT.print("Bluetooth ready: ");
            DEBUG_PORT.println(BT_DEVICE);
        #endif // DEBUG
    #endif // BT_PORT

    //pin setup
    pinMode(LED_PIN_ONBOARD, OUTPUT);
}

void loop()
{
    // check if client is connected
    digitalWrite(LED_PIN_ONBOARD, BT_PORT.hasClient() ? HIGH : LOW);

    // send
    if (DEBUG_PORT.available())
    {
        String str = "";

        while (DEBUG_PORT.available())
        {
            str += (char)DEBUG_PORT.read();
        }

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
            DEBUG_PORT.write("\nWrong message format");
        }
        else
        {
            payloadLen = BT_PORT.read();
        }

        while (BT_PORT.available() && payloadLen--)
        {
            // temp for converting value from char to -100/100
            if (payloadLen == 0)
            {
                char data = BT_PORT.read();
                int value = data > 127 ? data - 256 : data;
                DEBUG_PORT.print(value);
            }
            else
            {
                DEBUG_PORT.write(BT_PORT.read());
            }
        }

        if (payloadLen > 0)
        {
            while (BT_PORT.available())
            {
                BT_PORT.read();
                DEBUG_PORT.write("\nWrong payload length\n");
            }
        }
        else
        {
            if (msgFrom == SLAVE)
            {
                DEBUG_PORT.write("\n");
            }
        }
    }

    delay(2);
}
