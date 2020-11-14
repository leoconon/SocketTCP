#include "drawing.h"
#include "utils.h"
#include "global.h"
#include <wifi.h>

int historic[MAX_WIDTH];
u8g2_t u8g2;
int wifiState = 0;

void taskDisplay(void *pvParameters) {
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda = PIN_SDA;
	u8g2_esp32_hal.scl = PIN_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

    u8g2_Setup_ssd1306_i2c_128x64_noname_f (
		&u8g2,
		U8G2_R0,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb
    );

    u8x8_SetI2CAddress(&u8g2.u8x8, 0x78); 
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);

    u8g2_ClearBuffer(&u8g2);

    int readValue;
    bool sunBigger = true;
    bool wasShowingSun = false;
    bool isFirstIteraction = true;
    bool haveToShowSun;
    
    loop {
        xQueueReceive(bufferNewLuminosity, &readValue, pdMS_TO_TICKS(2000));
        drawHistoric(&u8g2, readValue);
        haveToShowSun = readValue > MAX_NIGHT_LUMINOSITY;
        if (haveToShowSun != wasShowingSun || isFirstIteraction) {
            wasShowingSun = haveToShowSun;
            clearDraw(&u8g2);
            if (wasShowingSun) {
                drawSun(&u8g2);
            } else {
                drawMoon(&u8g2);
            }
        }
        if (haveToShowSun) {
            animateSun(&u8g2, sunBigger);
        }
        sunBigger = !sunBigger;
        printValue(&u8g2, readValue);
        drawWifi(&u8g2);
        u8g2_SendBuffer(&u8g2);
        isFirstIteraction = false;
    }
}

void drawWifi(u8g2_t *u8g2) {
    EventBits_t flags = xEventGroupGetBits(wifiEventGroup);
    int newWifiState;
    if (flags & WIFI_CONNECTED_BIT) {
        newWifiState = CONNECTED_CHAR_HEX;
    } else if (flags & WIFI_FAIL_BIT) {
        newWifiState = DISCONNECTED_CHAR_HEX;
    } else {
        newWifiState = WAITING_CHAR_HEX;
    }
    if (newWifiState != wifiState) {
        wifiState = newWifiState;
        clearDrawWifi(u8g2);
        u8g2_SetDrawColor(u8g2, DRAW_COLOR_WHITE);
        u8g2_SetFont(u8g2, u8g2_font_unifont_t_symbols);
        u8g2_DrawGlyph(u8g2, 110, 15, newWifiState);
    }
}

void drawHistoric(u8g2_t *u8g2, int newValue) {
    if (newValue > LUMINOSITY_MAX_VALUE) {
        newValue = LUMINOSITY_MAX_VALUE;
    }
    int percent = (newValue / 1.0) / (LUMINOSITY_MAX_VALUE / 1.0) * 100.0;
    for (int i = 0; i < MAX_WIDTH - 1; i++) {
        historic[i] = historic[i + 1];
    }
    historic[MAX_WIDTH - 1] = percent;
    for (int i = 0; i < MAX_WIDTH; i++) {
        int size = historic[i] / 5.0;
        u8g2_SetDrawColor(u8g2, DRAW_COLOR_BLACK);
        u8g2_DrawVLine(u8g2, i, MAX_HEIGHT - HISTORY_MAX_HEIGHT, HISTORY_MAX_HEIGHT);
        u8g2_SetDrawColor(u8g2, DRAW_COLOR_WHITE);
        u8g2_DrawVLine(u8g2, i, MAX_HEIGHT - size, size);
    }
}

void printValue(u8g2_t *u8g2, int value) {
    char print[5];
    sprintf(print, "%d", value);
    u8g2_SetFont(u8g2, u8g2_font_6x10_mf);
    u8g2_DrawUTF8(u8g2, 64, 26, print);
}

void drawMoon(u8g2_t *u8g2) {
    u8g2_SetDrawColor(u8g2, DRAW_COLOR_WHITE);
    u8g2_DrawDisc(u8g2, 23, 23, 18, U8G2_DRAW_ALL);
    u8g2_SetDrawColor(u8g2, DRAW_COLOR_BLACK);
    u8g2_DrawDisc(u8g2, 30, 20, 18, U8G2_DRAW_ALL);
    u8g2_SetDrawColor(u8g2, DRAW_COLOR_WHITE);
    u8g2_SetFont(u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(u8g2, 39, 10, STAR_CHAR_HEX);
    u8g2_DrawGlyph(u8g2, 28, 26, STAR_CHAR_HEX);
    u8g2_SendBuffer(u8g2);
}

void drawSun(u8g2_t *u8g2) {
    //Circulo Central
    u8g2_SetDrawColor(u8g2, DRAW_COLOR_WHITE);
    u8g2_DrawDisc(u8g2, 23, 23, 8, U8G2_DRAW_ALL);
}

void animateSun(u8g2_t *u8g2, bool small) {
    if (small) {
        u8g2_SetDrawColor(u8g2, DRAW_COLOR_BLACK);
        //Vertical Superior
        u8g2_DrawLine(u8g2, 23, 5, 23, 11);
        //Vertical Inferior
        u8g2_DrawLine(u8g2, 23, 35, 23, 41);
        //Horizontal Esquerda
        u8g2_DrawLine(u8g2, 11, 23, 5, 23);
        //Horizontal Direita
        u8g2_DrawLine(u8g2, 35, 23, 41, 23);
        //Diagonal Norte Direita
        u8g2_DrawLine(u8g2, 35, 11, 31, 15);
        //Diagonal Norte Esquerda
        u8g2_DrawLine(u8g2, 11, 11, 15, 15);
        //Diagonal Sul Direita
        u8g2_DrawLine(u8g2, 35, 35, 31, 31);
        //Diagonal Sul Esquerda
        u8g2_DrawLine(u8g2, 11, 35, 15, 31);

        u8g2_SetDrawColor(u8g2, DRAW_COLOR_WHITE);
        //Vertical Superior
        u8g2_DrawLine(u8g2, 23, 7, 23, 11);
        //Vertical Inferior
        u8g2_DrawLine(u8g2, 23, 35, 23, 39);
        //Horizontal Esquerda
        u8g2_DrawLine(u8g2, 11, 23, 7, 23);
        //Horizontal Direita
        u8g2_DrawLine(u8g2, 35, 23, 39, 23);
        //Diagonal Norte Direita
        u8g2_DrawLine(u8g2, 33, 13, 31, 15);
        //Diagonal Norte Esquerda
        u8g2_DrawLine(u8g2, 13, 13, 15, 15);
        //Diagonal Sul Direita
        u8g2_DrawLine(u8g2, 33, 33, 31, 31);
        //Diagonal Sul Esquerda
        u8g2_DrawLine(u8g2, 13, 33, 15, 31);
    } else {
        u8g2_SetDrawColor(u8g2, DRAW_COLOR_WHITE);
        //Vertical Superior
        u8g2_DrawLine(u8g2, 23, 5, 23, 11);
        //Vertical Inferior
        u8g2_DrawLine(u8g2, 23, 35, 23, 41);
        //Horizontal Esquerda
        u8g2_DrawLine(u8g2, 11, 23, 5, 23);
        //Horizontal Direita
        u8g2_DrawLine(u8g2, 35, 23, 41, 23);
        //Diagonal Norte Direita
        u8g2_DrawLine(u8g2, 35, 11, 31, 15);
        //Diagonal Norte Esquerda
        u8g2_DrawLine(u8g2, 11, 11, 15, 15);
        //Diagonal Sul Direita
        u8g2_DrawLine(u8g2, 35, 35, 31, 31);
        //Diagonal Sul Esquerda
        u8g2_DrawLine(u8g2, 11, 35, 15, 31);
    }
}

void clearDraw(u8g2_t *u8g2) {
    u8g2_SetDrawColor(u8g2, DRAW_COLOR_BLACK);
    u8g2_DrawBox(u8g2, 0, 0, 50, MAX_HEIGHT - HISTORY_MAX_HEIGHT);
}

void clearDrawWifi(u8g2_t *u8g2) {
    u8g2_SetDrawColor(u8g2, DRAW_COLOR_BLACK);
    u8g2_DrawBox(u8g2, 100, 0, 120, MAX_HEIGHT - HISTORY_MAX_HEIGHT);
}