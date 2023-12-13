#ifndef CONFIG_H
#define CONFIG_H

//---------------------------------//
//------------SETTINGS-------------//
//---------------------------------//

// debug options , comment for no debug output
#define DEBUG 1       // Default debug info
//#define VESC_DEBUG 1  // Vesc debug info

#define PORT_SPEED 115200 // Default esp and vesc baud rate

#define METRICS_UPDATE_DELAY 500 // Minimum delay between metrics update in ms

#define CONTROL_TIMEOUT 2000 // Timeout for control message in ms

//---------------------------------//
//------------BLUETOOTH------------//
//---------------------------------//

#define BT_PORT SerialBT    // Bluetooth port
#define BT_DEVICE "GlBoard" // Bluetooth device name

//---------------------------------//
//-----PROTOCOL-COMMUNICATION------//
//---------------------------------//

// flags
#define REQUEST_FLAG 0x00
#define RESPONSE_FLAG 0x01
#define MASTER_FLAG 0x00
#define SLAVE_FLAG 0x04
#define TEST_FLAG 0x00
#define METRICS_FLAG 0x10
#define CONTROL_FLAG 0x20
#define CONFIG_FLAG 0x30

// flag masks
#define MESSAGE_TYPE_MASK 0x03
#define SOURCE_NODE_MASK 0x0c
#define COMMAND_MASK 0x70

// functions with flags
#define IS_REQUEST(x) ((x & MESSAGE_TYPE_MASK) == REQUEST_FLAG)
#define IS_RESPONSE(x) ((x & MESSAGE_TYPE_MASK) == RESPONSE_FLAG)
#define IS_MASTER(x) ((x & SOURCE_NODE_MASK) == MASTER_FLAG)
#define IS_SLAVE(x) ((x & SOURCE_NODE_MASK) == SLAVE_FLAG)
#define GET_COMMAND(x) (x & COMMAND_MASK)
#define SET_COMMAND(x, y) (x = (x & ~COMMAND_MASK) | y)
#define SET_SOURCE_NODE(x, y) (x = (x & ~SOURCE_NODE_MASK) | y)
#define SET_MESSAGE_TYPE(x, y) (x = (x & ~MESSAGE_TYPE_MASK) | y)

//---------------------------------//
//---------BOARD-SETTINGS----------//
//---------------------------------//

// port and vesc settings
#define DEBUG_PORT Serial // Debug port
#define VESC_PORT Serial2 // Vesc port

// lights settings
// #define FLIGHT_PIN 4                        //Front light pin
// #define FLIGHT_POWER 1.0                    //intenzita svietenia flight (stála) , 0 - 1
// #define RLIGHT_PIN 15                       //Rear light pin
// #define RLIGHT_POWER 0.4                    //intenzita svietenia rlight (stála) , 0 - 1
#define LED_PIN_ONBOARD 2

// current limit
#define MAX_THROTTLE_POWER 10
#define MAX_BRAKE_POWER 10

// motor settings
#define MOTOR_POLES 12 
#define MOTOR_PHASES 3

#endif // CONFIG_H
