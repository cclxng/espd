#include <stdio.h>
#include <string.h>
#include <libserialport.h>
#include "device.h"
#include <syslog.h>

int esp_scan(struct esp_device *list, int max){
	struct sp_port **ports;
	int count = 0;

	enum sp_return result = sp_list_ports(&ports);
	if(result != SP_OK){
	       syslog(LOG_ERR,"Failed to list serial ports");
       	       return -1;
	}

	for(int i=0; ports[i] != NULL && count < max; i++){
		if(sp_get_port_transport(ports[i]) != SP_TRANSPORT_USB) continue;

		int vid, pid;
	
		if(sp_get_port_usb_vid_pid(ports[i], &vid, &pid) != SP_OK) continue;
		if(vid != ESP_VENDOR_ID) continue;
		if(pid != ESP_PRODUCT_ID) continue;
	
		snprintf(list[count].vendor_id,  ID_LEN, "%04x", vid);
    		snprintf(list[count].product_id, ID_LEN, "%04x", pid);
		strncpy(list[count].port, sp_get_port_name(ports[i]), PORT_LEN);
		count++;
	}
	sp_free_port_list(ports);
	return count;
}
