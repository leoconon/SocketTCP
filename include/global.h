#ifndef GLOBAL
#define GLOBAL
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

QueueHandle_t bufferLuminosity;
EventGroupHandle_t wifiEventGroup;

#endif