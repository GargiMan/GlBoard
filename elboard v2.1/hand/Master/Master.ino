//libraries
#include <WiFi.h>             //hand control
#include <driver/adc.h>       //analog to digital library , joy needed
#include "esp_now.h"          //upravené hodnoty prenosu

//pin layout (input)
#define throtle 32            //analog, throtle / brake value
#define hold 2                //digital, hold speed control
#define modes 13              //digital, mode switch
#define flight 16             //digital, front light control
#define rlight 17             //digital, rear light control
#define rotation 4            //digital, drive rotation control

//value settings
#define sleepin 0x000000004   //hex - 2 wake up pin
#define afktime 30000         //millis without any change , deepsleep
#define h 540                 //deadzone horná hranica
#define mid 500               //deadzone stred, výpočet prúdu začína od hk , aplikovanie prúdu na motor od h
#define d 420                 //deadzone dolná hranica

//variable definition
bool reset;
unsigned long oldtime;

//connection settings
uint16_t data[6];             //array, data for transfer, same type in espnow.h
String boardname = "ElBoard"; //rovnake ako ssid v slave
esp_now_peer_info_t slave;
#define CHANNEL 3
#define DELETEBEFOREPAIR 1    //bolo off , testujem

//hand (controller) functions - ScanForSlave, manageSlave, OnDataSent, TransferProcess

void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));

  if (scanResults != 0) {
    for (int i = 0; i < scanResults; ++i) {
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);
      // delay(10);
      if (SSID.indexOf(boardname) == 0) {
        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x%c",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }
        slave.channel = CHANNEL; // pick a channel
        slave.encrypt = 0; // no encryption
        slaveFound = 1;
        break;
      }
    }
  }

  WiFi.scanDelete();  // clean up ram
}

bool manageSlave() {
  if (slave.channel == CHANNEL) {
    if (DELETEBEFOREPAIR) { //vymazat mozno - zistit vyznam
      const esp_now_peer_info_t *peer = &slave;
      const uint8_t *peer_addr = slave.peer_addr;
      esp_err_t delStatus = esp_now_del_peer(peer_addr);
    }
    const esp_now_peer_info_t *peer = &slave;
    const uint8_t *peer_addr = slave.peer_addr;
    bool exists = esp_now_is_peer_exist(peer_addr);
    if ( exists) {
      return true;
    } else {
      esp_err_t addStatus = esp_now_add_peer(peer);
    }
  } else {
    return false;
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { //funkcia pri prijatí/neprijatí dát
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // if (status == ESP_NOW_SEND_SUCCESS) {}
}

void TransferProcess() {
  // ScanForSlave(); //spomalené esp ak sa opakuje vždy pred prenosom
  if (slave.channel == CHANNEL) {
    bool isPaired = manageSlave();
    if (isPaired) {
      const uint8_t *peer_addr = slave.peer_addr;
      esp_now_send(peer_addr, data, sizeof(data)); //esp_err_t result = 
     }
   }
}

void getData(String howtoget) {
  if (howtoget == "sensor") {
    data[0]=adc1_get_raw(ADC1_CHANNEL_4); // data[0]=analogRead(throtle);
    data[1]=digitalRead(hold);
    data[2]=digitalRead(modes);
    data[3]=digitalRead(flight);
    data[4]=digitalRead(rlight);
    data[5]=digitalRead(rotation);
  }
  else if (howtoget == "zeroval") {
    data[0]=mid;
    data[1]=0;
    data[2]=0;
    data[3]=0;
    data[4]=0;
    data[5]=0;
  }
}

void powersleep () {
  if (data[0] <= h && data[0] >= d && data[1] == 0 && data[2] == 0 && data[3] == 0 && data[4] == 0 && data[5] == 0) {
    
    Serial.print("   ");
    Serial.print((afktime-(millis()-oldtime))/1000);
    
    if (reset == 0) {

      oldtime = millis();
      reset = 1;

    } else if (millis() - oldtime >= afktime) {

      getData("zeroval");
      TransferProcess();

      Serial.print("deepsleep");
      esp_deep_sleep_start(); 
    }

  } else {

    reset = 0;
  
  } 
}

void setup() {

  Serial.begin(115200); //len v prípade debugu

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK) { ESP.restart(); }
  esp_now_register_send_cb(OnDataSent);
  ScanForSlave(); //spomalený prenos ... ak sa opakuje vždy pred odoslaním

  pinMode(throtle,INPUT);
  pinMode(hold,INPUT_PULLDOWN);
  pinMode(modes,INPUT_PULLDOWN);
  pinMode(flight,INPUT_PULLDOWN);
  pinMode(rlight,INPUT_PULLDOWN);
  pinMode(rotation,INPUT_PULLDOWN);

  esp_sleep_enable_ext1_wakeup(sleepin,ESP_EXT1_WAKEUP_ANY_HIGH);
  
  adc1_config_width(ADC_WIDTH_BIT_10);
  adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_11);

}

void loop() {
  
  getData("sensor");
  TransferProcess(); //spomalené esp

  Serial.print(data[0]);
  Serial.print("   ");
  Serial.print(data[1]);
  Serial.print(data[2]);
  Serial.print(data[3]);
  Serial.print(data[4]);
  Serial.print(data[5]);

  powersleep();

  Serial.println(" ");

}
