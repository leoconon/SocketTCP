#include <wifi.h>

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define WIFI_USER "Eliete"
#define WIFI_PASS "#elisnon13"

EventGroupHandle_t wifiEventGroup;
static const char *TAG = "wifi";

void initNvs();
void initWifi();
void eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

/**
 * Inicializa a Non-volatile storage
 * 
 * Este espaço de memória reservado armazena dados necessários para a calibração do PHY.
 * Devido ao fato de o ESP não possuir EEPROM é necessário separar um pedaço da memória 
 * de programa para armazenar dados não voláteis
 */
void initNvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

/**
 * Função que faz a configuração de wifi e inicializa os eventos relacionados
 */
void initWifi() {
    wifiEventGroup = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &eventHandler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &eventHandler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_USER,
            .password = WIFI_PASS,
	        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

/**
 * Event handler dos eventos de wifi
 */
void eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "Tentando se conectar ao wifi...");
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Wifi desconectado!");
        xEventGroupSetBits(wifiEventGroup, WIFI_FAIL_BIT);
        xEventGroupClearBits(wifiEventGroup, WIFI_CONNECTED_BIT);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Conectado! O IP atribuido é: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifiEventGroup, WIFI_CONNECTED_BIT);
        xEventGroupClearBits(wifiEventGroup, WIFI_FAIL_BIT);
    } else {
        ESP_LOGI(TAG, "Erro de wifi");
        xEventGroupSetBits(wifiEventGroup, WIFI_FAIL_BIT);
        xEventGroupClearBits(wifiEventGroup, WIFI_CONNECTED_BIT);
    }
}