INC_DIRS := ../include
CFLAGS += $(foreach DIR, $(INC_DIRS), -I$(DIR)) -Wall

SRC := devices.c serial.c esp_ubus.c main.c

LDLIBS := -lserialport -lubus -lubox -lblobmsg_json

espd: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) $(LDLIBS) -o espd

clean:
	rm -f espd

.PHONY: clean
