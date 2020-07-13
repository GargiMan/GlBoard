#ifndef CONFIG_H
#define CONFIG_H

//compilation settings
#define UPLOAD_DEVICE 1                     //Device where code will be uploaded, 0 = board / 1 = controller


//---------------------------------//
//----------BOARD-SETTINGS---------//
//---------------------------------//

//port and vesc settings
#define DEFAULT_PORT_SPEED 115200           //Default comm and vesc speed , need to be defined for any communication
//#define DEBUG_PORT Serial1                  //Debug port , no debug if comented
#define VESC_CONTROL_PORT Serial2           //Vesc control , default port speed or custom
//#define VESC_CONTROL_PORT_SPEED 115200      //Vesc control speed
//#define VESC_DEBUG_PORT Serial1             //Vesc debug print , default port speed , no vesc debug if comented


//lights settings
//#define FLIGHT_PIN 4                        //Front light pin
//#define FLIGHT_POWER 1.0                    //intenzita svietenia flight (stála) , 0 - 1
//#define RLIGHT_PIN 15                       //Rear light pin
//#define RLIGHT_POWER 0.4                    //intenzita svietenia rlight (stála) , 0 - 1


//controller settings
#define JOY_DEAD_APPROX 0.05        //deadzone rýchlosti
#define JOY_DEAD_H 540              //deadzone horná hranica
#define JOY_DEAD_L 420              //deadzone dolná hranica
#define JOY_RES 1023                //rozlíšenie analogread , 10bit - 1023 , 12bit - 4095

#define JOY_LIMIT_H 0.95            //limit duty pre vyššie obtiažnosti
#define JOY_LIMIT_L 0.50            //liit duty pre nižšie obtiažnosti
#define JOY_HOLD_CHANGE_H 0.05      //maximálna zmena pri držaní hold , mod 3/4 , 1 - 100
#define JOY_HOLD_CHANGE_L 0.03      //maximálna zmena pri držaní hold , mod 1/2 , 1 - 100
#define JOY_BRAKE_POWER_H 20        //maximálna hodnota brzdiaceho prúdu, mod 3/4
#define JOY_BRAKE_POWER_L 10        //maximálna hodnota brzdiaceho prúdu, mod 1/2

//---------------------------------//
//-------CONTROLLER-SETTINGS-------//
//---------------------------------//

//sleep settings
#define SLEEP_PIN 0x000000004       //hex - 2 wake up pin
#define SLEEP_AFK_TIME 30000        //millis without any change , deepsleep
#define SLEEP_DEAD 50               //odchýlka od deadzone + aj - , zvykne občas skočiť hodnota 

//controls settings
#define POWER_PIN 32                //analog, throtle / brake value
#define HOLD_PIN 2                  //digital, hold speed control
#define MODE_PIN 13                 //digital, mode switch
#define LIGHT_PIN 16                //digital, light control
//#define LIGHT_PIN 17                //digital, rear light control
#define ROTATE_PIN 4                //digital, drive rotation control

#endif //CONFIG_H
