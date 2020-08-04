#!/usr/bin/env zsh

cp hid/Inc/usbd_hid.h Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc/usbd_hid.h
cp hid/Src/usbd_hid.c Middlewares/ST/STM32_USB_Device_Library/Class/HID/Src/usbd_hid.c
make
openocd -f board/st_nucleo_f0.cfg -c 'program build/ps2-to-usb.elf verify reset exit'
