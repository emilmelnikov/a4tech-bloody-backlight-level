CFLAGS  += $(shell pkg-config --cflags libusb-1.0)
LDFLAGS += $(shell pkg-config --libs   libusb-1.0)

CFLAGS += -std=c99 -pedantic -Wall -Wextra

bloody: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) bloody
