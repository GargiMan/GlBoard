//libraries
#include <WiFi.h>       //phone / hand control
#include <DNSServer.h>  //phone control
#include <ESPUI.h>      //phone control
#include "esp_now.h"    //upravené hodnoty prenosu
#include "VescUart.h"   //umiestnené priamo pri tomto súbore !!!
VescUart UART;
DNSServer DNS;

//pin layout (output)
#define flight 4
#define rlight 15

//value settings
#define fpow 1.0          //intenzita svietenia flight (stála) , 0 - 1
#define rpow 0.4          //intenzita svietenia rlight (stála) , 0 - 1
#define dutydead 0.05     //deadzone rýchlosti
#define h 540             //deadzone horná hranica
#define mid 500           //deadzone stred, výpočet prúdu začína od hk , aplikovanie prúdu na motor od h
#define d 420             //deadzone dolná hranica
#define joyres 1023       //rozlíšenie analogread , 10bit - 1023 , 12bit - 4095
#define hlimit 0.95       //limit duty pre vyššie obtiažnosti
#define dlimit 0.50       //liit duty pre nižšie obtiažnosti
#define hholdchange 0.05  //maximálna zmena pri držaní hold , mod 3/4 , 1 - 100
#define dholdchange 0.03  //maximálna zmena pri držaní hold , mod 1/2 , 1 - 100
#define hbpower 20        //maximálna hodnota brzdiaceho prúdu, mod 3/4
#define bbpower 10        //maximálna hodnota brzdiaceho prúdu, mod 1/2

//variable settings
int x = 0;               //počiatočný stav info (0-6)
int modes = 1;           //počiatočná hodnota mode (1-4)
           
//variable definition
bool vali, valk, vall, hold, t, b, i, k, l, m, controller, usedcheck;                               //controller do byte , ak chcem full acces mode
int s, v, bpower, mm, hh;
int joy = mid;                                                                                      //počiatočné nastavenie stredu
float vduty, vvolt, vamp, vtach, vrpm, modelimit, holdchange, speed, dist, dutysaved, w, c, u, q;
float dmodes = modes;                                                                               //modes pre počítanie s float hodnotami
unsigned long holdclock, throtle, oldtime, delaytime;                                               //millis() timers
String timer = "00:00";                                                                             //prvotné zobrazenie
String message;
String messages = message;

//connection settings 
#define CHANNEL 1
const char* SSID = "ElBoard";
const char* PASS = "1234abcd"; //heslo iba pripojenie telefonom , na ovládač nefunguje
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);

//phone functions - command, control, direction, frontlight, rearlight, clocktimer, statusprinter, phonecontrol
//hand (controller) functions - OnDataRecv, handcontrol
//board functions - lights, vescvalues

void command(Control sender, int type) { //phone - command control 
  char intext[10];
  sender.value.toCharArray(intext,10);
  ESPUI.updateText("Command","");

  //set time command

  if (intext[0] == 't') {
    timer = String(intext[1]) + String(intext[2]) + String(':') + String(intext[3]) + String(intext[4]);
    String timerhh = String(intext[1]) + String(intext[2]);
    String timermm = String(intext[3]) + String(intext[4]);
    hh = timerhh.toInt();
    mm = timermm.toInt();
    oldtime = millis();
    Serial.print("Time set: ");
    Serial.println(String(timer));
  }

  //change control command

  else if (intext[0] == 'c') {
    if (intext[1] == '1') {
      controller = 1;
      joy = mid;
      Serial.println("phone control");
    } else if (intext[1] == '0') {
      controller = 0;
      Serial.println("hand control");
    } 
  }

  //not command - print input

  else {
    Serial.println(sender.value);
  }
}

void control(Control sender, int value) { //phone - drive , info , mode
  switch (value) {
    case P_LEFT_DOWN:
      x--;
      if (x < 0) {
        x = 6;
      }
      break;
    case P_LEFT_UP:
      break;
    case P_RIGHT_DOWN:
      x++;
      if (x > 6) {
        x = 0;
      }
      break;
    case P_RIGHT_UP:
      break;
    case P_FOR_DOWN:
      t = 1;
      throtle = millis();
      Serial.println("Throtle");
      break;
    case P_FOR_UP:
      t = 0;
      throtle = millis();
      break;
    case P_BACK_DOWN:
      b = 1;
      Serial.println("Brake");
      break;
    case P_BACK_UP:
      b = 0;
      break;
    case P_CENTER_DOWN:
      if (vduty < dutydead && controller == 1) {
        modes = modes + 1;
        if (modes >= 5) {
          modes = 1;
        }
        dmodes = modes;
        Serial.print("Mode: ");
        Serial.println(modes);
      }
      break;
    case P_CENTER_UP:
      break;
  }
}

void direction(Control sender, int value) { //phone (control) - drive direction
  if (vduty < dutydead && controller == 1) {
    switch (value) {
      case S_ACTIVE:
        Serial.println("Backward");
        vali = 1;
        break;
      case S_INACTIVE:
        Serial.println("Forward");
        vali = 0;
        break;
    }
  }
  delay(50);
  ESPUI.updateSwitcher("Forward / Backward", vali); 
}

void frontlight(Control sender, int value) { //phone - front light 
  switch (value) {
    case S_ACTIVE:
      vall = 1;
      Serial.print("Flight: ");
      Serial.println(vall);
      break;
    case S_INACTIVE:
      vall = 0;
      Serial.print("Flight: ");
      Serial.println(vall);
      break;
  }
}

void rearlight(Control sender, int value) { //phone - rear light
  switch (value) {
    case S_ACTIVE:
      valk = 1;
      Serial.print("Rlight: ");
      Serial.println(valk);
      break;
    case S_INACTIVE:
      valk = 0;
      Serial.print("Rlight: ");
      Serial.println(valk);
      break;
  }
}

void clocktimer() { //phone - timer basic settings
  if (millis() - oldtime >= 60000) {
    mm = mm + 1;
    if (mm >= 60) {
      hh = hh + 1;
      mm = 0;
      if (hh >= 24) {
        hh = 0;
      }
    }
    timer = String(hh) + String(':');
    if (hh < 10) timer = String('0') + String(timer);
    if (mm < 10) timer = String(timer) + String('0');
    timer = timer + String(mm);
    oldtime = millis();
  }
}

void statusprinter() { //phone - info bar
  if (x == 0){
    message = "Mode: "+String(modes);
  }
  else if (x == 1){
    message = "Battery: "+String(vvolt)+" V";
  }
  else if (x == 2) {
    message = "Current: "+String(vamp)+" A";
  }
  else if (x == 3) {
    speed = vrpm/7*0.375*3.14159265359*0.000075*60 //erpm / pólové dvojice motora (7)* prevodový pomer (18/48 - 0.375)* pi (3.14...) * velkosť kolesa v km (0.000075) * konštanta 60 min
    message = "Speed: "+String(speed)+" km/h";
  } 
  else if (x == 4) {
    vduty = vduty * 100; // 1 -> 100%
    message = "Power: "+String(vduty)+" %";
  }
  else if (x == 5) {
    dist = vtach/7*0.375*3.14159265359*0.000075;  //erpm tachometer / pólové dvojice motora (7)* prevodový pomer (18/48 - 0.375)* pi (3.14...) * velkosť kolesa v km (0.000075)
    message = "Distance: "+String(dist)+" km";
  }
  else if (x == 6) {
    message = "Time: "+String(timer);
    }

  if (message != messages) {
    ESPUI.print("Info", message);
    messages = message;
  }
}

void phonecontrol() { //phone - control motor
  if (controller == 1) {
    if (t == 1 && b == 0) {
      if (millis() - throtle >= 250 && u >= 0 && u < dmodes * 5) {
        throtle = millis();
        u = u + (0.05 * dmodes * 5);
      } else if (u > dmodes * 5) {
        u = dmodes * 5;
      }
      if (vali == 0) {
        UART.setCurrent(u); //forward
      } else if (vali == 1) {
        UART.setCurrent(-u); //backward
      }
    } else if (t == 0 && b == 1) {
        if (modes >= 3) c = hbpower;
        if (modes <= 2) c = bbpower;
        UART.setBrakeCurrent(c);
    } else {
      UART.setCurrent(0);
      c = 0;
      if (millis() - throtle >= 125 && u > 0) {
        throtle = millis();
        u = u - (0.05 * dmodes * 5);
        if (u < 0) {
          u = 0;
        } 
      } else if (vduty < dutydead) {
        u = 0;
      }
    }
  }
}

void OnDataRecv(const uint8_t *mac_addr, const uint16_t data[6], int data_len) {  //hand - funkcia ktora sa vykona len v prípade ,že sa obdržali dáta
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

  if (controller == 0) {
   joy=data[0];

    if (data[1] == 1) {
      hold = 1;
    } else {
      hold = 0;
    }

    if (data[2] == 1 && m == 0 && (d < joy && joy < h) && vduty < dutydead) {
      m = 1;
      modes++;
      if (modes >= 5) {
        modes = 1;
      }
      dmodes = modes;
    }
    if (data[2] == 0) {
      m = 0;
    }

    if (data[3] == 1 && l == 0) {
      l = 1;
      vall = !vall;
      ESPUI.updateSwitcher("Front Light", vall);
    }
    if (data[3] == 0) {
      l = 0;
    }

    if (data[4] == 1 && k == 0) {
      k = 1;
      valk = !valk;
      ESPUI.updateSwitcher("Rear Light", valk);  
    }
    if (data[4] == 0) {
      k = 0;
    }

    if (data[5] == 1 && i == 0 && (d < joy && joy < h) && vduty < dutydead) {
      i = 1;
      vali = !vali;
      ESPUI.updateSwitcher("Forward / Backward", vali); 
    }
    if (data[5] == 0) {
      i = 0;
    }
  }

  if (usedcheck == 0) {
    usedcheck = 1;
    Serial.println("connected"); 
  }
  delaytime = millis();
}

void handcontrol() { //hand - control motor

  if (hold == 1) {                                 //hold - drží aktuálnu rýchlosť
    if (modes >= 3) holdchange = hholdchange;
    if (modes <= 2) holdchange = dholdchange;

    if (joy >= h) {                               //hold +
      v = joyres - mid;
      s = joy - mid;
      q = map(s , 0, v, 0, holdchange*1000);
      q = q / 1000;
      if (millis() - holdclock >= 250) {
        if (vali == 1) dutysaved -= q;
        if (vali == 0) dutysaved += q;
        holdclock = millis();
      }
    }

    if (joy <= d) {                                //hold -
      w =  d - joy;
      q = map(w , 0, d, 0, holdchange*1000);
      q = q / 1000;
      if (millis() - holdclock >= 250) {
        if (vali == 1) dutysaved += q;
        if (vali == 0) dutysaved -= q;
        holdclock = millis();
      }
    }

    if (vali == 1 && dutysaved >= 0) dutysaved = 0;
    if (vali == 0 && dutysaved <= 0) dutysaved = 0;
    
    UART.setDuty(dutysaved);
    
  }

  if (joy >= h && hold == 0) {                     //plyn

    v = joyres - mid;
    s = joy - mid;
    dutysaved = vduty;
    if (modes >= 3) modelimit = hlimit;
    if (modes <= 2) modelimit = dlimit;

    if (vduty < modelimit) {
      u = map(s, 0, v, 0, dmodes*5*100);
      u = u / 100;
    } else {
      u = 2;
    }

    if (vali == 0) {
      UART.setCurrent(u); //forward
    } else if (vali == 1) {
      UART.setCurrent(-u);  //backward
    }
  }

  if (joy <= d && hold == 0) {             //brzda

    if (modes >= 3) bpower = hbpower;
    if (modes <= 2) bpower = bbpower;

    w =  d - joy;
    c = map(w, 0, d, 0, bpower * 100);
    c = c / 100;

    UART.setBrakeCurrent(c);
  } 
  
  if (joy > d && joy < h && hold == 0) {      //neutral

    c = 0;
    w = 0;
    u = 0;
    s = 0;

    UART.setCurrent(0);
  }

  if (hold == 0) {

    dutysaved = vduty;
 
  }
}

void lights() { //board - lights
  if (vall == 1) {
    ledcWrite(1, 255 * fpow);
  }
  else {
    ledcWrite(1, 0);
  }

  if (valk == 1) {
    if (b == 1 && controller == 1) {
      ledcWrite(2, 255);
    } else if (w > 0 && controller == 0) {
      ledcWrite(2, 255 * (w / d * (1 - rpow) + rpow));
    } else {
      ledcWrite(2, 255 * rpow);
    }
  }
  else {
    ledcWrite(2, 0);
  }
}

void vescvalues() { //board - vescvalues
  if ( UART.getVescValues() ) {
    vduty = UART.data.dutyCycleNow;
    if (vduty < 0) {
      vduty = -vduty;
    }
    vvolt = UART.data.inpVoltage;
    vtach = UART.data.tachometerAbs;
    vrpm = UART.data.rpm;
    if (vrpm < 0) {
      vrpm = -vrpm;
    }
    vamp = UART.data.avgInputCurrent;
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID, PASS, CHANNEL, 0);
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
  }
  else {
    ESP.restart();
  }
  esp_now_register_recv_cb(OnDataRecv);
  
  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);
  ledcAttachPin(flight, 1);
  ledcAttachPin(rlight, 2);

  DNS.start(DNS_PORT, "*", apIP);

  ESPUI.label("Info", COLOR_CARROT, message);
  ESPUI.pad("Control", true, &control, COLOR_WETASPHALT);
  ESPUI.switcher("Forward / Backward", false, &direction, COLOR_PETERRIVER);
  ESPUI.switcher("Front Light", false, &frontlight, COLOR_SUNFLOWER);
  ESPUI.switcher("Rear Light", false, &rearlight, COLOR_ALIZARIN);
  ESPUI.text("Command", &command, COLOR_EMERALD, "");
  ESPUI.begin("El Board");
  
  oldtime = millis();
  throtle = millis();
  holdclock = millis();
  delaytime = millis();

  Serial2.begin(115200);
  UART.setSerialPort(&Serial2);
  // UART.setDebugPort(&Serial);
}

void loop() {

  if (usedcheck == 1 && millis() - delaytime > 20) {
    joy = mid;
    usedcheck = 0;
    Serial.println("connection lost");
    Serial.print("delay: ");
    Serial.println(millis() - delaytime);
    Serial.println("reloading . . .");
  }

  DNS.processNextRequest(); //phone - web control
  
  clocktimer();
  vescvalues();
  phonecontrol();
  handcontrol();
  lights();
  statusprinter();

}
