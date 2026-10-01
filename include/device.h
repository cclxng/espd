#ifndef ESP_DEVICE_H
#define ESP_DEVICE_H

#define MAX_DEVICES 8
#define ID_LEN 8
#define PORT_LEN 32
#define ESP_VENDOR_ID  0x10c4
#define ESP_PRODUCT_ID 0xea60


struct esp_device {
    char port[PORT_LEN];
    char vendor_id[ID_LEN];
    char product_id[ID_LEN];
};

int esp_scan(struct esp_device *list, int max);

#endif
