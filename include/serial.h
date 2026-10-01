#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>

int esp_cmd(const char *port_name, const char *cmd, char *resp, size_t resp_len);

#endif
