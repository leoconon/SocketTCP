#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "wifi.h"
#include "utils.h"
#include "socket.h"

static const char *TAG = "main";

void app_main() {
    ESP_LOGI(TAG, "Iniciando...");
    initNvs();
    initWifi();
    ESP_LOGI(TAG, "Iniciado!");
    delay(5000);
    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void*)AF_INET, 5, NULL);
}