#ifndef WIFI
#define WIFI

#include <stdio.h>
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_flash.h"

void initNvs();
void initWifi();
void eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

#endif