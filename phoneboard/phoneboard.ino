#include <DNSServer.h>
#include <ESPUI.h>
#include <WiFi.h>
#include "vescfiles/VescUart.h"
VescUart UART1;
DNSServer dnsServer;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
const char *ssid = "El Board";
const char *password = "abcd1234";

#define flight 22
#define rlight 23

long oldTime = 0;
int hh,mm,x = 0;
String timer;
const float fpow = 0.7;
const float rpow = 0.4;
const float dutydead = 0.05;
int mode = 1;
float dmode = mode;
bool valk = 0;
bool vall = 0;
float vescduty,vescvolt,vesctach,vescrpm;
float modelimit;
int a , b , t , throtle, braking;
float u , c;
String message;
String messages = message;
float speed,dist;

void command(Control sender, int type) {
  int inval = sender.value.toInt();
  if (inval >= 10000 && inval < 20000) {
    inval = inval - 10000;
    hh = inval / 100;
    mm = inval - (hh * 100) - 1;
    timer = String(hh) + String(':');
    if (hh < 10) timer = String('0') + String(timer);
    if (mm < 10) timer = String(timer) + String('0');
    timer = timer + String(mm);
    oldTime = millis() - 60000;
    Serial.print("Time set: ");
    Serial.println(String(timer));
  }
  else {
    Serial.println(sender.value);
  }
}

void control(Control sender, int value) {
  switch (value) {
    case P_LEFT_DOWN:
      x = x - 1;
      if (x < 0) {
        x = 5;
      }
      break;
    case P_LEFT_UP:
      break;
    case P_RIGHT_DOWN:
      x = x + 1;
      if (x > 5) {
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
      if (vescduty < dutydead) {
        mode = mode + 1;
        if (mode >= 11) {
          mode = 1;
        }
        dmode = mode;
      }
      Serial.print("Mode: ");
      Serial.println(mode);
      break;
    case P_CENTER_UP:
      break;
  }
}

void drive(Control sender, int value) {
  switch (value) {
    case S_ACTIVE:
      Serial.println("Backward");
      a = 1;
      break;
    case S_INACTIVE:
      Serial.println("Forward");
      a = 0;
      break;
  }
}

void frontlight(Control sender, int value) {
  switch (value) {
    case S_ACTIVE:
      vall = !vall;
      Serial.print("Flight: ");
      Serial.println(vall);
      break;
    case S_INACTIVE:
      vall = !vall;
      Serial.print("Flight: ");
      Serial.println(vall);
      break;
  }
}

void rearlight(Control sender, int value) {
  switch (value) {
    case S_ACTIVE:
      valk = !valk;
      Serial.print("Rlight: ");
      Serial.println(valk);
      break;
    case S_INACTIVE:
      valk = !valk;
      Serial.print("Rlight: ");
      Serial.println(valk);
      break;
  }
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  //WiFi.setHostname(ssid);

  //    WiFi.softAP(ssid);
  WiFi.softAP(ssid, password);
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // change the beginning to this if you want to join an existing network

  //  Serial.begin(115200);
  //  WiFi.begin(ssid, password);
  //  Serial.println("");
  //  // Wait for connection
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(100);
  //    Serial.print(".");
  //  }
  //  Serial.println("");
  //  Serial.print("IP address: ");
  //  Serial.println(WiFi.localIP());

  ESPUI.label("Info", COLOR_CARROT, "Mode: 1");
  ESPUI.pad("Control", true, &control, COLOR_WETASPHALT);
  ESPUI.switcher("Forward / Backward", false, &drive, COLOR_PETERRIVER);
  ESPUI.switcher("Front Light", false, &frontlight, COLOR_SUNFLOWER);
  ESPUI.switcher("Rear Light", false, &rearlight, COLOR_ALIZARIN);
  ESPUI.text("Command", &command, COLOR_EMERALD, "");

  dnsServer.start(DNS_PORT, "*", apIP);

  ESPUI.begin("El Board");
  oldTime = millis() - 60000;
  throtle = millis();
  braking = millis();

  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);
  ledcAttachPin(flight, 1);
  ledcAttachPin(rlight, 2);

  Serial2.begin(115200);
  UART1.setSerialPort(&Serial2);
//  UART1.setDebugPort(&Serial);
}

void loop(void) {
  dnsServer.processNextRequest();

  timer = String(hh) + String(':');
  if (hh < 10) timer = String('0') + String(timer);
  if (mm < 10) timer = String(timer) + String('0');
  timer = timer + String(mm);
  if (millis() - oldTime >= 60000) {
    mm = mm + 1;
    if (mm == 60) {
      hh = hh + 1;
      mm = 0;
      if (hh == 24) {
        hh = 0;
      }
    }
    oldTime = millis();
  }

  if ( UART1.getVescValues() ) {
    vescduty = UART1.data.dutyCycleNow;
    if (vescduty < 0) {
      vescduty = -vescduty;
    }
    vescvolt = UART1.data.inpVoltage;
    vesctach = UART1.data.tachometerAbs;
    vescrpm = UART1.data.rpm;
    if (vescrpm < 0) {
      vescrpm = -vescrpm;
    }
  }

  if (vall == 1) {
    ledcWrite(1, 255 * fpow);
  }
  else {
    ledcWrite(1, 0);
  }

  if (valk == 1) {
    if (c > 0) {
      ledcWrite(2, 255);
    }
    else {
      ledcWrite(2, 255 * rpow);
    }
  }
  else {
    ledcWrite(2, 0);
  }

  if (t == 1 && b == 0) {
    if (millis() - throtle >= 250 && u >= 0 && u < dmode * 5) {
      throtle = millis();
      u = u + (0.05 * dmode * 5);
    }
    else if (u > dmode * 5) {
      u = dmode * 5;
    }
    if (a == 0) {
      UART1.setCurrent(-u); //forward
    }
    else if (a == 1) {
      UART1.setCurrent(u); //backward
    }
  }

  else if (t == 0 && b == 1) {
    if (mode >= 6) c = 10;
    if (mode <= 5) c = 5;
    UART1.setBrakeCurrent(c);
  }

  else {
    UART1.setCurrent(0);
    c = 0;
    if (millis() - throtle >= 250 && u > 0) {
      throtle = millis();
      u = u - (0.05 * dmode * 5);
      if (u < 0) {
        u = 0;
      } 
    }
    else if (vescduty < dutydead) {
      u = 0;
    }
  }

  if (x == 0){
    message = String("Mode: ")+String(mode);
  }
  else if (x == 1){
    message = String("Battery: ")+String(vescvolt)+String(" V");
  }
  else if (x == 2) {
    speed = vescrpm/38*3.14159265359*0.000075*60;
    message = String("Speed: ")+String(speed)+String(" km/h");
  } 
  else if (x == 3) {
    float vescdutyp = vescduty * 100; // 1 -> 100%
    message = String("Duty: ")+String(vescdutyp)+String(" %");
  }
  else if (x == 4) {
    dist = vesctach/38*3.14159265359*0.000075;
    message = String("Distance: ")+String(dist)+String(" km");
  }
  else if (x == 5) {
    message = String("Time: ")+String(timer);
    }

  if (message != messages) {
    ESPUI.print("Info", message);
    messages = message;
  }
}
