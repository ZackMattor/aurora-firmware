SHELL=/bin/bash
include variables.mk

BUILD_FLAGS="-D'OTA_PASSWORD=\"$(OTA_PASSWORD)\"'"
UPLOAD_FLAGS="-a'$(OTA_PASSWORD)'"

debian-install-dependencies:
	apt -y install python3-pip
	pip3 install --user platformio
	@echo
	@echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	@echo "!! You may need to add ~/.local/bin to your PATH !!"
	@echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"

build-serial-monitor: build-serial monitor
build-ip-monitor: build-ip monitor

monitor:
	pio device monitor --port=$(SERIAL_DEV)

build-monitor:
	PLATFORMIO_BUILD_FLAGS=$(BUILD_FLAGS) pio run --upload-port $(SERIAL_DEV) --target monitor

build-serial:
	PLATFORMIO_BUILD_FLAGS=$(BUILD_FLAGS) pio run --upload-port $(SERIAL_DEV) --target upload

build-ip:
	PLATFORMIO_UPLOAD_FLAGS=$(UPLOAD_FLAGS) PLATFORMIO_BUILD_FLAGS=$(BUILD_FLAGS) pio run --upload-port $(IP_ADDRESS) --target upload
