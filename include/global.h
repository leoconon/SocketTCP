#ifndef GLOBAL
#define GLOBAL
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define DATA_BUFFER_SIZE 20

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

QueueHandle_t bufferNewLuminosity;
QueueHandle_t bufferLuminosityNotification;
EventGroupHandle_t wifiEventGroup;
char wifiIp[20];

#endif