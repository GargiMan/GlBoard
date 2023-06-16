#ifndef CONFIG_H
#define CONFIG_H

//---------------------------------//
//------------SETTINGS-------------//
//---------------------------------//

#define DEBUG 1 // Debug info , comment for no debug output

#define PORT_SPEED 115200 // Default esp and vesc speed , need to be defined for any communication

#define CYCLE_MS 2 // Main cycle time in ms

//---------------------------------//
//------------BLUETOOTH------------//
//---------------------------------//

#define BT_PORT SerialBT    // Bluetooth port
#define BT_DEVICE "GlBoard" // Bluetooth device name

// protocol description
#define MASTER 0x01
#define SLAVE 0x02

//---------------------------------//
//---------BOARD-SETTINGS----------//
//---------------------------------//

// port and vesc settings
#define DEBUG_PORT Serial        // Debug port, no debug if comented
#define VESC_CONTROL_PORT Serial2 // Vesc control port
// #define VESC_DEBUG_PORT Serial1          //Vesc debug print, no vesc debug if comented

// lights settings
// #define FLIGHT_PIN 4                        //Front light pin
// #define FLIGHT_POWER 1.0                    //intenzita svietenia flight (stála) , 0 - 1
// #define RLIGHT_PIN 15                       //Rear light pin
// #define RLIGHT_POWER 0.4                    //intenzita svietenia rlight (stála) , 0 - 1
#define LED_PIN_ONBOARD 2

// current limit
#define MAX_THROTTLE_POWER 10
#define MAX_BRAKE_POWER 10

#endif // CONFIG_H
