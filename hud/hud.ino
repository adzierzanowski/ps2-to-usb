#include <Wire.h>
#include <Adafruit_NeoPixel.h>

constexpr uint8_t I2C_ADDR = 0x7c;
constexpr size_t ROLLOVER = 6;

Adafruit_NeoPixel leds(ROLLOVER, A0);
uint8_t keys[ROLLOVER] = {0};

void leds_hello(void)
{
  for (int i = 0; i < ROLLOVER; i++)
  {
    leds.setPixelColor(i, 0x00, 0x00, 0x10);
    delay(25);
    leds.show();
  }

  delay(100);
  
  for (int i = 0; i < ROLLOVER; i++)
  {
    leds.setPixelColor(i, 0x00, 0x00, 0x00);
    delay(25);
    leds.show();
  }
}

void i2cHandler(int byte_count)
{
  if (byte_count != ROLLOVER)
  {
    return;
  }

  for (int i = 0; i < ROLLOVER; i++)
  {
    uint8_t keycode = Wire.read();

    keys[i] = keycode;

    if (keycode)
    {
      leds.setPixelColor(ROLLOVER - i - 1, 0x00, 0x00, 0x10);
    }
    else
    {
      leds.setPixelColor(ROLLOVER - i - 1, 0x00, 0x00, 0x00);
    }
  }

  leds.show();
}

void setup()
{
  Serial.begin(9600);

  Wire.begin(I2C_ADDR);
  Wire.onReceive(i2cHandler);
  leds.begin();

  leds_hello();

  Serial.println("Ready.");
}

void loop()
{
}
