#include "utils.h"

/**
 * Função que interrompe a execução por determinado tempo
 * @param millis Tempo em milissegundos 
 */
void delay(int millis) {
    vTaskDelay(millis / portTICK_RATE_MS);
}