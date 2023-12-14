#ifndef CONFIG_H
#define CONFIG_H

//---------------------------------//
//------------SETTINGS-------------//
//---------------------------------//

#define PROJECT_NAME "GlBoard" // Project name
#define PROJECT_VERSION "2.0"  // Project version

// debug options , comment for no debug output
#define DEBUG 1       // Default debug info
//#define VESC_DEBUG 1  // Vesc debug info

#define PORT_SPEED 115200 // Default esp and vesc baud rate

#define METRICS_UPDATE_DELAY 500 // Minimum delay between metrics update in ms

#define CONTROL_TIMEOUT 2000 // Timeout for control message in ms

//---------------------------------//
//------------BLUETOOTH------------//
//---------------------------------//

#define BT_PORT SerialBT       // Bluetooth port
#define BT_DEVICE PROJECT_NAME // Bluetooth device name

//---------------------------------//
//-----COMMUNICATION-PROTOCOL------//
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

// macro functions with flags
#define IS_REQUEST(x) ((x & MESSAGE_TYPE_MASK) == REQUEST_FLAG)
#define IS_RESPONSE(x) ((x & MESSAGE_TYPE_MASK) == RESPONSE_FLAG)
#define IS_MASTER(x) ((x & SOURCE_NODE_MASK) == MASTER_FLAG)
#define IS_SLAVE(x) ((x & SOURCE_NODE_MASK) == SLAVE_FLAG)
#define GET_COMMAND(x) (x & COMMAND_MASK)
#define SET_COMMAND(x, y) (x = (x & ~COMMAND_MASK) | y)
#define SET_SOURCE_NODE(x, y) (x = (x & ~SOURCE_NODE_MASK) | y)
#define SET_MESSAGE_TYPE(x, y) (x = (x & ~MESSAGE_TYPE_MASK) | y)

// config values
#define CFG_FRONT_LIGHT 0x01
#define CFG_REAR_LIGHT 0x02
#define CFG_MAX_THROTTLE_POWER 0x10
#define CFG_MAX_BRAKE_POWER 0x20

//---------------------------------//
//---------BOARD-SETTINGS----------//
//---------------------------------//

// port and vesc settings
#define DEBUG_PORT Serial           // Debug port
#define VESC_PORT Serial2           // Vesc port

// lights settings
#define PWM_FREQUENCY 5000          //PWM frequency
#define PWM_RESOLUTION 8            //PWM resolution
#define FRONT_LIGHT_CHANNEL 1       //Front light pwm channel
#define FRONT_LIGHT_PIN 4           //Front light pin
#define FRONT_LIGHT_POWER 1.0       //Front light default power (0.0 - 1.0)
#define REAR_LIGHT_CHANNEL 2        //Rear light pwm channel
#define REAR_LIGHT_PIN 15           //Rear light pin
#define REAR_LIGHT_POWER 0.4        //Rear light default power (0.0 - 1.0)
#define LED_ONBOARD_PIN 2

// current limit
#define MAX_THROTTLE_POWER 10       //Max throttle power in A
#define MAX_BRAKE_POWER 10          //Max brake power in A

// motor settings
#define MOTOR_POLES 12 
#define MOTOR_PHASES 3

//---------------------------------//
//-----------PREFERENCES-----------//
//---------------------------------//

// preference macros
#define RW_MODE false
#define RO_MODE true

// preference keys (max 15 char length)
#define KEY_PROJECT_VERSION "project_version"
#define KEY_MAX_THROTTLE_POWER "tpower_max"
#define KEY_MAX_BRAKE_POWER "bpower_max"
#define KEY_FRONT_LIGHT_ENABLED "lfront_enabled"
#define KEY_FRONT_LIGHT_POWER "lfront_power"
#define KEY_REAR_LIGHT_ENABLED "lrear_enabled"
#define KEY_REAR_LIGHT_POWER "lrear_power"

#endif // CONFIG_H
