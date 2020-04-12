# Aurora Firmware

This is the firmware for the Aurora IOT lighting system.

## Depedencies
 - make
 - python3
   - pio

## Getting Started
 - Clone this repo
 - For Debian or Ubuntu run `sudo make debian-install-dependencies` otherwise just install dependencies required
 - Copy `config.h.template` to `src/config.h` and adjust variables accordingly
 - Copy `variables.mk.template` to `variables.mk` and adjust variables accordingly
 - `make build-serial-monitor` to build and flash over the serial port and watch console output to make sure it connects to WiFi
 - After you have the firmware on the device you can run `make build-ip` to build and flash over IP
