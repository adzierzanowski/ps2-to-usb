# PS/2 to USB converter

This piece of software converts PS/2 keyboard signal into USB HID reports so
wonderful mechanical keyboards from the past can function in our modern world.

## Operation priciple

On every PS/2 clock pin's falling edge the incoming bit is handled. Some sanity
checks are performed on the incoming data and if everything is alright,
the 8-bit keycode is put into a queue.

In the main loop the queue is handled (if it's full or after a short timeout).
PS/2 scancodes are mapped onto corresponding USB codes and a HID report is sent.
If I2C handle is not NULL, the codes are also sent to the slave device for
further processing (there's a little arduino script in the `hud` folder which
flashes some LEDs on every keystroke).

## Caveats

The initialization code has been generated with Cube MX. Unfortunately, every
generation erases the modified `usbd_hid.c` file. After the code generation,
files from `hid` folder must be copied into the
`Middlewares/ST/STM32_USB_Device_Library/Class/HID` folder.
You can build and upload the project using `build.zsh` script (it handles
this "hid" situation).

I didn't put the `Drivers` and other files generated with Cube into the
repository so you'll have to regenerate the project by yourself. Just open
`ps2-to-usb.ioc` with Cube MX and you'll be good.

I use STM32F072C8 for this project but any STM32 with an USB peripheral will
suffice (some pin remapping can be needed).

Mind that the Caps Lock key is mapped to ESC because i use Vim. You can change
that in `keymap.c`.
