#!/bin/bash
if (whiptail --yesno "Clean before compiling?" 8 78); then
	rm main.eep main.elf main.hex main.o
fi
make all
if (whiptail --yesno "Compiled, continue with upload?" 8 78); then
	sudo avrdude -p m328p -c usbtiny -U flash:w:main.hex:i -F -P usb
fi
