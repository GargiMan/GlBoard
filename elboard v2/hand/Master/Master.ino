#include "esp_now.h"
#include <WiFi.h>

#define hold 16
#define modes 17
#define flight 5
#define rlight 18
#define rotation 19
#define throttle 4

uint16_t data[6];
uint16_t datas[6];

String boardname = "ElBoard"; //rovnake ako ssid v slave
esp_now_peer_info_t slave;
#define CHANNEL 3
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0
 
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
    if (DELETEBEFOREPAIR) {
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

void sendData() {
  const uint8_t *peer_addr = slave.peer_addr;
  esp_err_t result = esp_now_send(peer_addr, data, sizeof(data));
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { //funkcia pri prijatí/neprijatí dát
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

void TransferProcess() {
  ScanForSlave();
  if (slave.channel == CHANNEL) {
    bool isPaired = manageSlave();
    if (isPaired) {
      sendData();
     }
   }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK) {
    ESP.restart();
  }
  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  data[0]=analogRead(throttle);
  data[1]=digitalRead(hold);
  data[2]=digitalRead(modes);
  data[3]=digitalRead(flight);
  data[4]=digitalRead(rlight);
  data[5]=digitalRead(rotation);
  if (data[0] - datas[0] >= 2 || datas[0] - data[0] >= 2 || data[1] != datas[1] || data[2] != datas[2] || data[3] != datas[3] || data[4] != datas[4] || data[5] != datas[5]) {
    TransferProcess();
    datas[0] = data[0];
    datas[1] = data[1];
    datas[2] = data[2];
    datas[3] = data[3];
    datas[4] = data[4];
    datas[5] = data[5];
  }

  // Serial.println(data[0]);
  // Serial.println(data[1]);
  // Serial.println(data[2]);
  // Serial.println(data[3]);
  // Serial.println(data[4]);
  // Serial.println(data[5]);
}
