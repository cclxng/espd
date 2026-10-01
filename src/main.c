#include <libubus.h>
#include <libubox/uloop.h>
#include <syslog.h>
#include <signal.h>
#include "esp_ubus.h"

static void signal_handler(int sig){
	uloop_end();
}

int main(void){
	struct ubus_context *ctx;

	uloop_init();
	ctx = ubus_connect(NULL);
	if(!ctx){
		syslog(LOG_ERR, "Failed to connect ubus");
		return UBUS_STATUS_CONNECTION_FAILED;
	}

	ubus_add_uloop(ctx);

	if(esp_ubus_register(ctx)){
    		syslog(LOG_ERR, "Failed to register ubus object");
    		ubus_free(ctx);
    		return 1;
	}

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);


	uloop_run();
	
	esp_ubus_cleanup();
	ubus_free(ctx);
	uloop_done();

	return 0;

}
