#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libusb.h>

enum {
	A4TECH_VID = 0x09da,
	A4TECH_MAGIC = 0x07,
	BACKLIGHT_OPCODE = 0x11,
	BACKLIGHT_WRITE = 0x80,
	BACKLIGHT_READ = 0x00,
};

static int MOUSE_PIDS[] = {0x172a, 0xf613, 0x11f5, 0x1485, 0x14ee, 0x1a5a, 0xf633, 0xf643, 0x1003};
// static const char *MOUSE_NAMES[] = {"Bloody V5", "Bloody V7", "Bloody V8", "Bloody R7", "Bloody R8-1", "Bloody R3", "Bloody AL9", "Bloody R70", "Bloody A9"};

void usb_assert(int ret, const char *msg) {
	if (ret < 0) {
		fprintf(stderr, "%s: %s\n", msg, libusb_strerror(ret));
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2 || strlen(argv[1]) != 1 || *argv[1] < '0' || *argv[1] > '3') {
		fprintf(stderr, "Usage: %s [0|1|2|3]\n", argv[0]);
		return EXIT_FAILURE;
	}
	unsigned char level = *argv[1] - '0';

	usb_assert(libusb_init(NULL), "libusb_init");

	libusb_device **devs;
	ssize_t ndevs = libusb_get_device_list(NULL, &devs);
	usb_assert(ndevs, "libusb_get_device_list");

	for (ssize_t i = 0; i < ndevs; i++) {
		libusb_device *dev = devs[i];

		struct libusb_device_descriptor desc;
		usb_assert(libusb_get_device_descriptor(dev, &desc), "libusb_get_device_descriptor");

		if (desc.idVendor != A4TECH_VID) {
			continue;
		}

		size_t i;
		for (i = 0; i < sizeof MOUSE_PIDS / sizeof *MOUSE_PIDS; i++) {
			if (desc.idProduct != MOUSE_PIDS[i]) {
				break;
			}
		}
		if (i >= sizeof MOUSE_PIDS / sizeof *MOUSE_PIDS) {
			continue;
		}

		libusb_device_handle *handle;
		usb_assert(libusb_open(dev, &handle), "libusb_open");

		int isactive = libusb_kernel_driver_active(handle, 2);
		usb_assert(isactive, "libusb_kernel_driver_active");
		if (isactive) {
			usb_assert(libusb_detach_kernel_driver(handle, 2), "libusb_detach_kernel_driver");
		}

		unsigned char data[72] = {A4TECH_MAGIC, BACKLIGHT_OPCODE, 0, 0, BACKLIGHT_WRITE, 0, 0, 0, level};
		usb_assert(libusb_control_transfer(handle, 0x21, 9, 0x0307, 2, data, sizeof data, 10000), "libusb_control_transfer");
	}

	return EXIT_SUCCESS;
}
