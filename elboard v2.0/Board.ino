/**
 * Glboard
 * Author: Gargi
 * 
 * Only "config.h" file can be modified with user settings
 * 
 */

#include "config.h"     //library with settings

#if (UPLOAD_DEVICE == 0) //board compilation

    #include "VescUart.h"

    VescUart VESC;

#elif (UPLOAD_DEVICE == 1) //controller compilation

    #include <driver/adc.h>

#endif // UPLOAD_DEVICE (global)


void setup() {

    //Usb comm
    #ifdef DEBUG_PORT
        DEBUG_PORT.begin(DEFAULT_PORT_SPEED);
    #endif // DEBUG_PORT

    #if (UPLOAD_DEVICE == 0) //board compilation

        //Vesc port check and setup
        #ifdef VESC_CONTROL_PORT

            //Vesc port open and set speed
            #ifdef VESC_CONTROL_PORT_SPEED
                VESC_CONTROL_PORT.begin(CONTROL_PORT_SPEED);
            #else // VESC_CONTROL_PORT_SPEED
                VESC_CONTROL_PORT.begin(DEFAULT_PORT_SPEED);
            #endif // VESC_CONTROL_PORT_SPEED
            VESC.setSerialPort(&VESC_CONTROL_PORT);

            #ifdef VESC_DEBUG
                VESC.setDebugPort(&VESC_DEBUG_PORT);
            #endif // VESC_DEBUG

        #endif // VESC_CONTROL_PORT

        //Front light setup
        #ifdef FLIGHT_PIN
            ledcSetup(1, 5000, 8);
            ledcAttachPin(FLIGHT_PIN, 1);
        #endif

        //Rear light setup
        #ifdef RLIGHT_PIN
            ledcSetup(2, 5000, 8);
            ledcAttachPin(RLIGHT_PIN, 2);
        #endif

    #elif (UPLOAD_DEVICE == 1) //controller compilation



    #endif // UPLOAD_DEVICE (setup)

}


void loop() {

    #if (UPLOAD_DEVICE == 0) //board compilation

    #elif (UPLOAD_DEVICE == 1) //controller compilation

    #endif // UPLOAD_DEVICE (loop)

}