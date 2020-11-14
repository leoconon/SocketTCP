#ifndef SOCKET
#define SOCKET

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "esp_log.h"
#include <lwip/netdb.h>
#include "utils.h"

void tcp_server_task(void *pvParameters);

#endif