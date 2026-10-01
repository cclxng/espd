#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include "device.h"
#include "serial.h"

static struct blob_buf b = {};

enum {
	CTRL_PORT,
	CTRL_PIN,
	__CTRL_MAX
};

enum {
	GET_PORT,
	GET_PIN,
	GET_MODEL,
	GET_SENSOR,
	__GET_MAX
};

static const struct blobmsg_policy ctrl_policy[__CTRL_MAX] = {
	[CTRL_PORT] = { .name = "port", .type = BLOBMSG_TYPE_STRING },
	[CTRL_PIN] = { .name = "pin", .type = BLOBMSG_TYPE_INT32 },
};

static const struct blobmsg_policy get_policy[__GET_MAX] = {
        [GET_PORT] = { .name = "port", .type = BLOBMSG_TYPE_STRING },
        [GET_PIN] = { .name = "pin", .type = BLOBMSG_TYPE_INT32 },
	[GET_MODEL] = { .name = "model", .type = BLOBMSG_TYPE_STRING },
        [GET_SENSOR] = { .name = "sensor", .type = BLOBMSG_TYPE_STRING },
};

static int devices_handler(struct ubus_context *ctx, struct ubus_object *obj,
			struct ubus_request_data *req, const char *method,
			struct blob_attr *msg){
		
	struct esp_device list[MAX_DEVICES];
	int n = esp_scan(list, MAX_DEVICES);
	void *array;

	if(n<0){
		syslog(LOG_ERR, "esp_scan failed");
		return UBUS_STATUS_UNKNOWN_ERROR;
	}
	blob_buf_init(&b, 0);

	array = blobmsg_open_array(&b, "devices");

	for(int i=0; i < n; i++){
		void *table = blobmsg_open_table(&b, NULL);
		blobmsg_add_string(&b, "port", list[i].port);
		blobmsg_add_string(&b, "vendor_id", list[i].vendor_id);
		blobmsg_add_string(&b, "product_id", list[i].product_id);

		blobmsg_close_table(&b, table);
	}
	blobmsg_close_array(&b, array);
	
	ubus_send_reply(ctx, req, b.head);
	return UBUS_STATUS_OK;
}

static int control_pin(struct ubus_context *ctx, struct ubus_request_data *req,
                        struct blob_attr *msg, const char *action){
	struct blob_attr *tb[__CTRL_MAX];
	char cmd[128];
	char resp[512];

	blobmsg_parse(ctrl_policy, __CTRL_MAX, tb, blob_data(msg), blob_len(msg));
	if(!tb[CTRL_PORT] || !tb[CTRL_PIN]) return UBUS_STATUS_INVALID_ARGUMENT;

	const char *port = blobmsg_get_string(tb[CTRL_PORT]);
	int pin = blobmsg_get_u32(tb[CTRL_PIN]);

	snprintf(cmd, sizeof(cmd), "{\"action\":\"%s\",\"pin\":%d}",action, pin);

	if (esp_cmd(port, cmd, resp, sizeof(resp)) < 0) {
    		syslog(LOG_ERR, "esp_cmd failed");
		return UBUS_STATUS_UNKNOWN_ERROR;
	}
	
	blob_buf_init(&b, 0);
	blobmsg_add_json_from_string(&b, resp);
	ubus_send_reply(ctx, req, b.head);

	return UBUS_STATUS_OK;
}

static int get_handler(struct ubus_context *ctx, struct ubus_object *obj,
			struct ubus_request_data *req, const char *method,
                	struct blob_attr *msg){
        struct blob_attr *tb[__GET_MAX];
        char cmd[128];
        char resp[512];

        blobmsg_parse(get_policy, __GET_MAX, tb, blob_data(msg), blob_len(msg));
        if(!tb[GET_PORT] || !tb[GET_PIN] || !tb[GET_MODEL] || !tb[GET_SENSOR]) return UBUS_STATUS_INVALID_ARGUMENT;

        const char *port = blobmsg_get_string(tb[GET_PORT]);
        int pin = blobmsg_get_u32(tb[GET_PIN]);
	const char *sensor = blobmsg_get_string(tb[GET_SENSOR]);
	const char *model = blobmsg_get_string(tb[GET_MODEL]);

	snprintf(cmd, sizeof(cmd), "{\"action\":\"get\",\"sensor\":\"%s\",\"pin\":%d,\"model\":\"%s\"}", sensor, pin, model);

        if (esp_cmd(port, cmd, resp, sizeof(resp)) < 0) {
                syslog(LOG_ERR, "esp_cmd failed");
        	return UBUS_STATUS_UNKNOWN_ERROR;
	}

        blob_buf_init(&b, 0);
        blobmsg_add_json_from_string(&b, resp);
        ubus_send_reply(ctx, req, b.head);

        return UBUS_STATUS_OK;
}

static int on_handler(struct ubus_context *ctx, struct ubus_object *obj,
                struct ubus_request_data *req, const char *method,
                struct blob_attr *msg){
        return control_pin(ctx, req, msg, "on");
}

static int off_handler(struct ubus_context *ctx, struct ubus_object *obj,
                struct ubus_request_data *req, const char *method,
                struct blob_attr *msg){
        return control_pin(ctx, req, msg, "off");
}

static const struct ubus_method esp_methods[] = {
        UBUS_METHOD_NOARG("devices", devices_handler),
	UBUS_METHOD("on", on_handler, ctrl_policy),
	UBUS_METHOD("off", off_handler, ctrl_policy),
	UBUS_METHOD("get", get_handler, get_policy),
};

static struct ubus_object_type esp_obj_type = UBUS_OBJECT_TYPE("esp", esp_methods);

static struct ubus_object esp_object = {
        .name = "esp",
        .type = &esp_obj_type,
        .methods = esp_methods,
        .n_methods = ARRAY_SIZE(esp_methods),
};

int esp_ubus_register(struct ubus_context *ctx){
	return ubus_add_object(ctx, &esp_object);
}

void esp_ubus_cleanup(){
	blob_buf_free(&b);
}
