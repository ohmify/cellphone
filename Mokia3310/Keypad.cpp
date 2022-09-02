#include "esp32-hal.h"
#include "sys/_types.h"
#include "esp32-hal-adc.h"
#include "Keypad.h"

int previousKey = 0;
int pressedKey = 0;
int value;

int counter = 5;
unsigned long newPressDelay = 300;
unsigned long lastPressMillis = millis();
unsigned long lastNumPressMillis = millis();

int getFunctionKey()
{
  analogReadResolution(11);
  unsigned long currentMillis = millis();

  if ((unsigned long)(currentMillis - lastPressMillis) >= newPressDelay)
  {
    while (counter > 0)
    {
      value = analogRead(39);
      pressedKey = value / 100;

      if (counter == 5)
        previousKey = pressedKey;
           
      if (previousKey == pressedKey)
        counter--;
      else
        counter = 5;
    }
    
    counter = 5;
    lastPressMillis = millis();
    if (previousKey != 20)
      return previousKey;
    else
      return 0;
  }

  else 
    return 0;
}

int getNumberKey()
{
  analogReadResolution(10);
  unsigned long currentMillis = millis();

  if ((unsigned long)(currentMillis - lastNumPressMillis) >= newPressDelay)
  {
    while (counter > 0)
    {
      value = analogRead(36);
      pressedKey = value / 100;

      if (counter == 5)
        previousKey = pressedKey;
           
      if (previousKey == pressedKey)
        counter--;
      else
        counter = 5;
    }
    
    counter = 5;
    lastNumPressMillis = millis();
    return previousKey;
  }

  else
    return 10;
}