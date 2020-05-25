#include <WiFi.h>
#include <DNSServer.h>
#include <ESPUI.h>
#include "esp_now.h" //upravené hodnoty prenosu
#include "VescUart.h" //umiestnené priamo pri tomto súbore
VescUart UART;
DNSServer dnsServer;

#define flight 22
#define rlight 23

long holdclock, throtle, braking, oldTime = 0;
int hh, mm, x = 0;
String timer, message;
String messages = message;
const float fpow = 0.7; //intenzita svietenia flight (stála) , 0 - 1
const float rpow = 0.4; //intenzita svietenia rlight (stála) , 0 - 1
const float dutydead = 0.05; //deadzone rýchlosti
const float h = 590; //deadzone horná hranica
const float d = 490; //deadzone dolná hranica
const float hk = 550; //konštanta joy počíta od hk po 1023
const float hlimit = 0.95; //limit duty pre vyššie obtiažnosti
const float dlimit = 0.50; //liit duty pre nižšie obtiažnosti
float bpower = 10; //maximálna hodnota brzdenia A
int modes = 1; //hodnota mode, ktorá sa mení
float dmodes = modes; //modes pre počítanie s float hodnotami
float hholdchange = 0.05; //maximálna zmena pri držaní hold , mod 3/4
float dholdchange = 0.03; //maximálna zmena pri držaní hold , mod 1/2
float holdchange;
bool vali = 0;
bool valk = 0;
bool vall = 0;
bool hold = 0;
bool controller = 0;
long vesctach, vescrpm;
float vescduty, vescvolt, vescamp, modelimit;
int a, b, t, i, k, l, m, s, v, joy;
float u, c, w, speed, dist, dutysaved, q;


#define CHANNEL 1
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
const char* SSID = "ElBoard";
const char* PASS = "1234abcd";

void command(Control sender, int type) {
  char intext[10];
  sender.value.toCharArray(intext,10);
  ESPUI.updateText("Command","");

  if (intext[0] == 't') {
    timer = String(intext[1]) + String(intext[2]) + String(':') + String(intext[3]) + String(intext[4]);
    String timerhh = String(intext[1]) + String(intext[2]);
    String timermm = String(intext[3]) + String(intext[4]);
    hh = timerhh.toInt();
    mm = timermm.toInt();
    oldTime = millis();
    Serial.print("Time set: ");
    Serial.println(String(timer));
  }

  else if (intext[0] == 'c') {
    if (intext[1] == '1') {
      controller = 1;
      Serial.println("phone control");
    } else if (intext[1] == '0') {
      controller = 0;
      Serial.println("hand control");
    }
  }

  else {
    Serial.println(sender.value);
  }
}

void control(Control sender, int value) {
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
      if (vescduty < dutydead && controller == 1) {
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

void drive(Control sender, int value) {
  if (vescduty < dutydead && controller == 1) {
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
}

void frontlight(Control sender, int value) {
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

void rearlight(Control sender, int value) {
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

void OnDataRecv(const uint8_t *mac_addr, const uint16_t data[6], int data_len) {  //funkcia ktora sa vykona len v prípade ,že sa obdržali dáta
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  
  if (controller == 0) {
   joy=data[0];

  if (data[1] == 1) {
    hold = 1;
  } else {
    hold = 0;
  }

  if (data[2] == 1 && m == 0 && (d < joy && joy < h) && vescduty < dutydead) {
    m = 1;
    modes = modes + 1;
    if (modes >= 5) {
      modes = 1;
    }
    dmodes = modes;
    if (modes <= 2) {
      ledcWrite(1, 0);
      delay(50);
      ledcWrite(1,255);
      delay(50);
      ledcWrite(1, 0);
    }
    if (modes >= 3) {
      ledcWrite(1, 0);
      delay(50);
      ledcWrite(1,255);
      delay(50);
      ledcWrite(1, 0);
      delay(50);
      ledcWrite(1,255);
      delay(50);
      ledcWrite(1, 0);
    }
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

  if (data[5] == 1 && i == 0 && (d < joy && joy < h) && vescduty < dutydead) {
    i = 1;
    vali = !vali;
    ESPUI.updateSwitcher("Forward / Backward", vali); 
  }
  if (data[5] == 0) {
    i = 0;
  }
  }
}

void clocktimer() {
  if (millis() - oldTime >= 60000) {
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
    oldTime = millis();
  }
}

void lights() {
  if (vall == 1) {
    ledcWrite(1, 255 * fpow);
  }
  else {
    ledcWrite(1, 0);
  }

  if (valk == 1) {
    if (c > 0) {
      ledcWrite(2, 255);
    } else if (w > 0) {
      ledcWrite(2, 255 * (w / d * (1 - rpow) + rpow));
    } else {
      ledcWrite(2, 255 * rpow);
    }
  }
  else {
    ledcWrite(2, 0);
  }
}

void vescvalues() {
  if ( UART.getVescValues() ) {
    vescduty = abs(UART.data.dutyCycleNow);
    // if (vescduty < 0) {
    //   vescduty = -vescduty;
    // }
    vescvolt = UART.data.inpVoltage;
    vesctach = UART.data.tachometerAbs;
    vescrpm = abs(UART.data.rpm);
    // if (vescrpm < 0) {
    //   vescrpm = -vescrpm;
    // }
    vescamp = UART.data.avgInputCurrent;
  }
}

void statusprinter() {
  if (x == 0){
    message = String("Mode: ")+String(modes);
  }
  else if (x == 1){
    message = String("Battery: ")+String(vescvolt)+String(" V");
  }
  else if (x == 2) {
    message = String("Current: ")+String(vescamp)+String(" A");
  }
  else if (x == 3) {
    float vescrpmf = vescrpm;
    speed = vescrpmf/38*3.14159265359*0.000075*60;
    message = String("Speed: ")+String(speed)+String(" km/h");
  } 
  else if (x == 4) {
    float vescdutyf = vescduty * 100; // 1 -> 100%
    message = String("Power: ")+String(vescdutyf)+String(" %");
  }
  else if (x == 5) {
    float vesctachf = vesctach; 
    dist = vesctachf/38*3.14159265359*0.000075;
    message = String("Distance: ")+String(dist)+String(" km");
  }
  else if (x == 6) {
    message = String("Time: ")+String(timer);
    }

  if (message != messages) {
    ESPUI.print("Info", message);
    messages = message;
  }
}

void phonecontrol() {
  if (controller == 1) {
    if (t == 1 && b == 0) {
      if (millis() - throtle >= 250 && u >= 0 && u < dmodes * 5) {
        throtle = millis();
        u = u + (0.05 * dmodes * 5);
      } else if (u > dmodes * 5) {
        u = dmodes * 5;
      }
      if (vali == 0) {
        UART.setCurrent(-u); //forward
      } else if (vali == 1) {
        UART.setCurrent(u); //backward
      }
    } else if (t == 0 && b == 1) {
        if (modes >= 3) c = 10;
        if (modes <= 2) c = 5;
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
      } else if (vescduty < dutydead) {
        u = 0;
      }
    }
  }
}

void handcontrol() {

  if (hold == 1) {                                 //hold - drží aktuálnu rýchlosť
    if (modes >= 3) holdchange = hholdchange;
    if (modes <= 2) holdchange = dholdchange;

    if (joy >= h) {
      q = map(s , 0, v, 0, holdchange*1000);
      q = q / 1000;
      if (millis() - holdclock >= 250) {
        dutysaved += q;
        holdclock = millis();
      }
    }

    if (joy <= d) {
      q = map(w , 0, d, 0, holdchange*1000);
      q = q / 1000;
      if (millis() - holdclock >= 250) {
        dutysaved -= q;
        holdclock = millis();
      }
    }

    UART.setDuty(dutysaved);
  }

  if (joy >= h && hold == 0) {                     //plyn

    v = 1023 - hk;
    s = joy - hk;
    dutysaved = vescduty;
    if (modes >= 3) modelimit = hlimit;
    if (modes <= 2) modelimit = dlimit;

    if (vescduty < modelimit) {
      u = map(s, 0, v, 0, dmodes*5*100);
      u = u / 100;
    } else {
      u = 2;
    }

    if (vali == 0) {
      UART.setCurrent(-u); //forward
    } else if (vali == 1) {
      UART.setCurrent(u);  //backward
    }
  }

  if (joy <= d && hold == 0) {             //brzda

    if (modes >= 3) bpower = 20;
    if (modes <= 2) bpower = 10;
    w =  d - joy;
    c = map(w, 0, d, 0, bpower * 100);
    c = c / 100;

    UART.setBrakeCurrent(c);
  } 
  
  if (joy > d && joy < h && hold == 0) {      //neutral

    dutysaved = vescduty;
    c = 0;
    w = 0;
    u = 0;
    s = 0;

    UART.setCurrent(0);
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

  dnsServer.start(DNS_PORT, "*", apIP);

  ESPUI.label("Info", COLOR_CARROT, "Mode: 1");
  ESPUI.pad("Control", true, &control, COLOR_WETASPHALT);
  ESPUI.switcher("Forward / Backward", false, &drive, COLOR_PETERRIVER);
  ESPUI.switcher("Front Light", false, &frontlight, COLOR_SUNFLOWER);
  ESPUI.switcher("Rear Light", false, &rearlight, COLOR_ALIZARIN);
  ESPUI.text("Command", &command, COLOR_EMERALD, "");
  ESPUI.begin("El Board");

  oldTime = millis() - 60000;
  throtle = millis();
  braking = millis();
  holdclock = millis();

  Serial2.begin(115200);
  UART.setSerialPort(&Serial2);
  // UART.setDebugPort(&Serial);
}

void loop() {
  dnsServer.processNextRequest();
  clocktimer();
  vescvalues();
  phonecontrol();
  handcontrol();
  lights();
  statusprinter();
}
