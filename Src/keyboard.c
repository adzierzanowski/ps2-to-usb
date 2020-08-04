#include "keyboard.h"


// Puts a keycode into a HID report if there's an empty slot available
void put_key(struct keyboard_hid_t *khid, uint8_t keycode)
{
  switch (keycode)
  {
    case 0xe0: // LCtrl
      khid->modifiers |= (1 << 0);
      return;
    
    case 0xe1: // LShift
      khid->modifiers |= (1 << 1);
      return;

    case 0xe2: // LAlt
      khid->modifiers |= (1 << 2);
      return;

    case 0xe3: // LMeta
      khid->modifiers |= (1 << 3);
      return;

    case 0xe4: // RCtrl
      khid->modifiers |= (1 << 4);
      return;
    
    case 0xe5: // RShift
      khid->modifiers |= (1 << 5);
      return;

    case 0xe6: // RAlt
      khid->modifiers |= (1 << 6);
      return;

    case 0xe7: // RMeta
      khid->modifiers |= (1 << 7);
      return;

    default:
      break;
  }
  
  int free_spot = 0;
  for (int i = 0; i < ROLLOVER; i++)
  {
    if (khid->keys[i] == keycode)
      return;

    if (khid->keys[i] == 0)
      free_spot = i;
  }

  khid->keys[free_spot] = keycode;
}

// Removes any occurrence of a keycode from a HID report
void pop_key(struct keyboard_hid_t *khid, uint8_t keycode)
{
  switch (keycode)
  {
    case 0xe0: // LCtrl
      khid->modifiers &= ~(1 << 0);
      return;
    
    case 0xe1: // LShift
      khid->modifiers &= ~(1 << 1);
      return;

    case 0xe2: // LAlt
      khid->modifiers &= ~(1 << 2);
      return;

    case 0xe3: // LMeta
      khid->modifiers &= ~(1 << 3);
      return;

    case 0xe4: // RCtrl
      khid->modifiers &= ~(1 << 4);
      return;
    
    case 0xe5: // RShift
      khid->modifiers &= ~(1 << 5);
      return;

    case 0xe6: // RAlt
      khid->modifiers &= ~(1 << 6);
      return;

    case 0xe7: // RMeta
      khid->modifiers &= ~(1 << 7);
      return;

    default:
      break;
  }

  for (int i = 0; i < ROLLOVER; i++)
  {
    if (khid->keys[i] == keycode)
      khid->keys[i] = 0;
  }
}

// Handles incoming bits
// This function is called on every PS/2 CLK falling edge
void ps2_clk_callback(
  struct queue_t *keyq,
  uint32_t *keyq_timeout,
  GPIO_TypeDef *din_port,
  uint16_t din_pin)
{
  static uint16_t raw = 0;
  static int bit_shift = 0;

  uint8_t bit = HAL_GPIO_ReadPin(din_port, din_pin);

  switch (bit_shift)
  {
    case 0:
      if (bit == 1)
        goto reset_frame;
      break;
      
    case 9:
      // __builtin_parity is a GCC extension
      if (__builtin_parity(raw) == bit)
        goto reset_frame;
      break;

    case 10:
      raw |= bit << bit_shift++;
      queue_put(keyq, (raw >> 1) & 0xff);
      goto reset_frame;
      break;

    default:
      break;
  }

  raw |= bit << bit_shift++;
  *keyq_timeout = HAL_GetTick();
  return;

  reset_frame:
    raw = 0;
    bit_shift = 0;
    *keyq_timeout = HAL_GetTick();
}

void handle_keys(
  USBD_HandleTypeDef *usbd,
  struct keyboard_hid_t *khid,
  struct queue_t *keyq,
  uint32_t keyq_timeout,
  I2C_HandleTypeDef *hi2c)
{
  static bool prefix_code = false;
  static bool break_code = false;

  if (queue_full(keyq) || HAL_GetTick() - keyq_timeout > KEY_QUEUE_TIMEOUT)
  {
    while (!queue_empty(keyq))
    {
      uint8_t keycode = queue_get(keyq);

      if (keycode == PREFIX_CODE)
      {
        prefix_code = true;
        return;
      }
      else if (keycode == BREAK_CODE)
      {
        break_code = true;
        return;
      }

      uint8_t usb_keycode = 0;

      if (prefix_code)
      {
        usb_keycode = keymap_prefixed[keycode];
        prefix_code = false;
      }
      else
      {
        usb_keycode = keymap[keycode];
      }

      if (break_code)
      {
        pop_key(khid, usb_keycode);
        break_code = false;
      }
      else
      {
        put_key(khid, usb_keycode);
      }

      USBD_HID_SendReport(usbd, (uint8_t *) khid, KHIDSZ);

      if (hi2c != NULL)
      {
        // Send current keycodes to some device for further processing
        HAL_I2C_Master_Transmit_DMA(hi2c, HUD_I2C_ADDR << 1, khid->keys, ROLLOVER);
      }
    }
  }
}
