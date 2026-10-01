#include <stdio.h>
#include <string.h>
#include <libserialport.h>
#include "serial.h"

int esp_cmd(const char *port_name, const char *cmd, char *resp, size_t resp_len){
	struct sp_port *port;
	if(sp_get_port_by_name(port_name, &port) != SP_OK) return -1;

	if(sp_open(port,SP_MODE_READ_WRITE) != SP_OK){
		sp_free_port(port);
		return -1;
	}

	sp_set_baudrate(port, 9600);
	sp_set_bits(port, 8);
	sp_set_parity(port, SP_PARITY_NONE);
	sp_set_stopbits(port, 1);
	sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);
	sp_flush(port, SP_BUF_BOTH);
	if(sp_blocking_write(port, cmd, strlen(cmd), 1000) < 0){
		sp_close(port);
		sp_free_port(port);
		return -1;
	}
	int n = sp_blocking_read(port, resp, resp_len - 1, 2000);
	sp_close(port);
	sp_free_port(port);

	if(n <= 0) return -1;

	resp[n] = '\0';
	return 0;

}
