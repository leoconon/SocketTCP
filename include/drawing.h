#ifndef DRAWING
#define DRAWING

#include <u8g2_esp32_hal.h>

#define MAX_WIDTH 128
#define MAX_HEIGHT 64
#define LUMINOSITY_MAX_VALUE 1000
#define STAR_CHAR_HEX 0x2605
#define CONNECTED_CHAR_HEX 0x2713
#define DISCONNECTED_CHAR_HEX 0x2715
#define WAITING_CHAR_HEX 0x23f3
#define HISTORY_MAX_HEIGHT 20
#define DRAW_COLOR_BLACK 0
#define DRAW_COLOR_WHITE 1

void taskDisplay(void *pvParameters);
void drawWifi(u8g2_t *u8g2);
void drawHistoric(u8g2_t *u8g2, int newValue);
void drawMoon(u8g2_t *u8g2);
void drawSun(u8g2_t *u8g2);
void animateSun(u8g2_t *u8g2, bool small);
void clearDraw(u8g2_t *u8g2);
void clearDrawWifi(u8g2_t *u8g2);
void printValue(u8g2_t *u8g2, int value);

#endif