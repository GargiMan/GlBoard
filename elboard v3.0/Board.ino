

//port and vesc settings
#define DEFAULT_PORT_SPEED 115200        //Default comm and vesc speed , need to be defined for any communication
#define COMM_PORT Serial1                //Usb comm port , no comm if comented
#define VESC Vesc                        //Vesc prefix
#define VESC_CONTROL_PORT Serial2        //Vesc control , default port speed or custom
//#define VESC_DEBUG_PORT Serial1          //Vesc debug , default port speed
//#define VESC_CONTROL_PORT_SPEED 115200   //Vesc control speed

//lights settings
//#define FLIGHT_PIN 4                    //Front light pin
//#define FLIGHT_POWER 1.0            //intenzita svietenia flight (stála) , 0 - 1
//#define RLIGHT_PIN 15                   //Rear light pin
//#define RLIGHT_POWER 0.4            //intenzita svietenia rlight (stála) , 0 - 1

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


void setup() {

    //Usb comm
    #ifdef COMM_PORT
        COMM_PORT.begin(DEFAULT_PORT_SPEED);
    #endif

    //Vesc port check and setup
    #ifdef VESC_CONTROL_PORT
        #ifdef VESC_CONTROL_PORT_SPEED
            VESC_CONTROL_PORT.begin(CONTROL_PORT_SPEED);
        #else
            VESC_CONTROL_PORT.begin(DEFAULT_PORT_SPEED);
        #endif
        VESC.setSerialPort(&VESC_CONTROL_PORT);
        #ifdef VESC_DEBUG_PORT
            VESC_DEBUG_PORT.begin(DEFAULT_PORT_SPEED);
            VESC.setDebugPort(&DEBUG_PORT);
        #endif 
    #endif

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

}

void loop() {

}