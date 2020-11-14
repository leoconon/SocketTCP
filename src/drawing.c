#include "drawing.h"
#include "utils.h"

int historic[MAX_WIDTH];

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
    u8g2_DrawGlyph(u8g2, 78, 31, STAR_CHAR_HEX);
    u8g2_DrawGlyph(u8g2, 108, 38, STAR_CHAR_HEX);
    u8g2_DrawGlyph(u8g2, 39, 10, STAR_CHAR_HEX);
    u8g2_DrawGlyph(u8g2, 28, 26, STAR_CHAR_HEX);
    u8g2_DrawGlyph(u8g2, 60, 15, STAR_CHAR_HEX);
    u8g2_DrawGlyph(u8g2, 71, 41, STAR_CHAR_HEX);
    u8g2_DrawGlyph(u8g2, 95, 17, STAR_CHAR_HEX);
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
    u8g2_DrawBox(u8g2, 0, 0, MAX_WIDTH, MAX_HEIGHT - HISTORY_MAX_HEIGHT);
}
