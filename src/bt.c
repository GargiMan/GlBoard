/**
 * bt.c
 *
 *  Created on: 14 dec 2023
 *      Author: Marek Gergel
 */

#include "bt.h"

bool connected = false;
bool available = false;

esp_spp_cb_handle_t spp_server_handle = 0;
uint8_t pin_code[6] = {1, 2, 3, 4, 5, 6};

void btEventCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    switch (event) {
        case ESP_SPP_INIT_EVT:
            ESP_LOGI("BT", "ESP_SPP_INIT_EVT");
            esp_bt_dev_set_device_name(param->init.name);
            esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
            break;
        case ESP_SPP_DISCOVERY_COMP_EVT:
            ESP_LOGI("BT", "ESP_SPP_DISCOVERY_COMP_EVT");
            break;
        case ESP_SPP_OPEN_EVT:
            ESP_LOGI("BT", "ESP_SPP_OPEN_EVT");
            connected = true;
            break;
        case ESP_SPP_CLOSE_EVT:
            ESP_LOGI("BT", "ESP_SPP_CLOSE_EVT");
            connected = false;
            break;
        case ESP_SPP_START_EVT:
            ESP_LOGI("BT", "ESP_SPP_START_EVT");
            break;
        case ESP_SPP_CL_INIT_EVT:
            ESP_LOGI("BT", "ESP_SPP_CL_INIT_EVT");
            break;
        case ESP_SPP_DATA_IND_EVT:
            ESP_LOGI("BT", "ESP_SPP_DATA_IND_EVT");
            available = true;
            break;
        case ESP_SPP_CONG_EVT:
            ESP_LOGI("BT", "ESP_SPP_CONG_EVT");
            break;
        case ESP_SPP_WRITE_EVT:
            ESP_LOGI("BT", "ESP_SPP_WRITE_EVT");
            break;
        case ESP_SPP_SRV_OPEN_EVT:
            ESP_LOGI("BT", "ESP_SPP_SRV_OPEN_EVT");
            break;
        default:
            break;
    }
}

void btInit(char *name, bool isServer) {
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&bt_cfg);
    esp_bt_controller_enable(ESP_BT_MODE_BTDM);

    esp_bluedroid_init();
    esp_bluedroid_enable();

    esp_bt_dev_set_device_name(name);

    esp_bt_gap_set_pin(ESP_BT_PIN_TYPE_FIXED, 6, pin_code);

    esp_spp_register_callback(btEventCallback);
    esp_spp_init(ESP_SPP_MODE_CB);

    if (isServer) {
        esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, name);
    }
}

void btSetPin(char *pin) {

}

bool btIsConnected() {

}

void btConnect() {

}
void btDisconnect() {

}

bool btIsAvailable() {

}

void btWrite(unsigned char *data, unsigned int len) {

}

void btRead(unsigned char *data, unsigned int len) {

}

void btDiscardReadBuffer() {

}