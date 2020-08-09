/**
 * Glboard
 * Author: Gargi
 * 
 * Change UPLOAD_DEVICE value for another device 
 * File 'config.h' can be modified with other settings 
 */

//upload device select
#define UPLOAD_DEVICE 1                     //Device where code will be uploaded, 0 = board / 1 = controller


// GLBOARD CODE


#include "config.h"     //library with settings

#if (UPLOAD_DEVICE == 0) //board compilation

    #include "VescUart.h"

    VescUart VESC;

#elif (UPLOAD_DEVICE == 1) //controller compilation

    #include <driver/adc.h>

#endif // UPLOAD_DEVICE (global)


void setup() {

    //Usb comm
    #ifdef ESP_DEBUG_PORT
            ESP_DEBUG_PORT.begin(PORT_SPEED);
    #endif // ESP_DEBUG_PORT

    #if (UPLOAD_DEVICE == 0) //board compilation

        //Vesc port check and setup
        #ifdef VESC_CONTROL_PORT
            #if (ESP_DEBUG_PORT != VESC_CONTROL_PORT)
                VESC_CONTROL_PORT.begin(PORT_SPEED);
            #endif // (ESP_DEBUG_PORT != VESC_CONTROL_PORT)
            VESC.setSerialPort(&VESC_CONTROL_PORT);
            #ifdef VESC_DEBUG_PORT
                #if (ESP_DEBUG_PORT != VESC_DEBUG_PORT)
                    VESC_DEBUG_PORT.begin(PORT_SPEED);
                #endif // (ESP_DEBUG_PORT != VESC_DEBUG_PORT)
                VESC.setDebugPort(&VESC_DEBUG_PORT);
            #endif // VESC_DEBUG_PORT
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
