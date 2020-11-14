#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "wifi.h"
#include "utils.h"
#include "socket.h"
#include <u8g2_esp32_hal.h>
#include <driver/adc.h>
#include "drawing.h"
#include "global.h"

static const char *TAG = "main";

void taskReadLum(void *pvParameters);

void app_main() {
    xTaskCreate(taskReadLum, "task_lum", 2048, NULL, 1, NULL);
    xTaskCreate(taskDisplay, "task_display", 3000, NULL, 1, NULL);
    ESP_LOGI(TAG, "Iniciando...");
    initNvs();
    initWifi();
    ESP_LOGI(TAG, "Iniciado!");
    delay(5000);
    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void*)AF_INET, 5, NULL);
}

void taskReadLum(void *pvParameters) {
    bufferNewLuminosity = xQueueCreate(1, sizeof(int));
    bufferLuminosity = xQueueCreate(20, sizeof(int));
    adc1_config_width(PIN_LUM);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_0);
    int val, valHist;
    loop {
        val = adc1_get_raw(PIN_LUM);
        valHist = adc1_get_raw(PIN_LUM);
        xQueueSend(bufferNewLuminosity, &val, pdMS_TO_TICKS(0));
        xQueueSend(bufferLuminosity, &valHist, pdMS_TO_TICKS(0));
        delay(500);
    }
}