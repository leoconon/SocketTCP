#ifndef UTILS
#define UTILS

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define true 1
#define false 0
#define loop while(1)
#define PIN_LUM ADC_WIDTH_BIT_12
#define PIN_SDA 5
#define PIN_SCL 4
#define MAX_NIGHT_LUMINOSITY 100

void delay(int millis);

#endif
