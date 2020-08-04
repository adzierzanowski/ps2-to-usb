#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdlib.h>

#include "stm32f0xx.h"
#include "queue.h"
#include "keymap.h"
#include "usbd_hid.h"

// Maximum number of simultaneously pressed keys
#define ROLLOVER 6

// PS/2 special scancodes
#define PREFIX_CODE 0xe0
#define BREAK_CODE 0xf0

// Size of the HID report in bytes
#define KHIDSZ 8

// Time in ms after which the queue of keycodes is handled
#define KEY_QUEUE_TIMEOUT 15

#define HUD_I2C_ADDR 0x7c


struct keyboard_hid_t
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[ROLLOVER];
};

void put_key(struct keyboard_hid_t *khid, uint8_t keycode);
void pop_key(struct keyboard_hid_t *khid, uint8_t keycode);
void ps2_clk_callback(
  struct queue_t *keyq,
  uint32_t *keyq_timeout,
  GPIO_TypeDef *din_port,
  uint16_t din_pin);
void handle_keys(
  USBD_HandleTypeDef *usbd,
  struct keyboard_hid_t *khid,
  struct queue_t *keyq,
  uint32_t keyq_timeout,
  I2C_HandleTypeDef *hi2c);

#endif
