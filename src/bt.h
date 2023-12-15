/**
 * bt.h
 *
 *  Created on: 14 dec 2023
 *      Author: Marek Gergel
 */

#ifndef GLBOARD_BT_H_
#define GLBOARD_BT_H_

#include <stdbool.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_gap_bt_api.h>
#include <esp_spp_api.h>

#ifdef __cplusplus
extern "C" {
#endif

void btInit(char *name, bool isServer);
void btSetPin(char *pin);
bool btIsConnected();
void btConnect();
void btDisconnect();
bool btIsAvailable();
void btWrite(unsigned char *data, unsigned int len);
void btRead(unsigned char *data, unsigned int len);
void btDiscardReadBuffer();

#ifdef __cplusplus
}
#endif

#endif // GLBOARD_BT_H_
