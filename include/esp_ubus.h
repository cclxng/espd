#ifndef ESP_UBUS_H
#define ESP_UBUS_H

#include <libubus.h>

int esp_ubus_register(struct ubus_context *ctx);
int on_handler(struct ubus_context *ctx, struct ubus_object *obj,
                struct ubus_request_data *req, const char *method,
                struct blob_attr *msg);
int off_handler(struct ubus_context *ctx, struct ubus_object *obj,
                struct ubus_request_data *req, const char *method,
                struct blob_attr *msg);
void esp_ubus_cleanup();
#endif
