#include "InputManager.h"

InputManager::InputManager(byte pinUp, byte pinDown, byte pinLeft, byte pinRight)
{
  // Internal button assigment
  buttons[BTN_UP].pin = pinUp;
  buttons[BTN_DOWN].pin = pinDown;
  buttons[BTN_LEFT].pin = pinLeft;
  buttons[BTN_RIGHT].pin = pinRight;

  // Reset every value for all the buttons
  for (byte i = 0; i < BTN_COUNT; i++)
  {
    buttons[i].lastRawState = HIGH;
    buttons[i].debouncedState = HIGH;
    buttons[i].lastDebounceTime = 0;
    buttons[i].pressStartTime = 0;
    buttons[i].longPressHandled = false;
  }
}

// Initialize all the pins with INPUT_PULLUP (This for AVR-Based Arduino boards)
// --- Change pin initialization depending on your chip
void InputManager::begin() { for (byte i = 0; i < BTN_COUNT; i++) pinMode(buttons[i].pin, INPUT_PULLUP); }

// Update/Scan the button events
void InputManager::update()
{
  unsigned long currentMillis = millis();

  for (byte i = 0; i < BTN_COUNT; i++)
  {
    bool rawRead = digitalRead(buttons[i].pin);
    if (rawRead != buttons[i].lastRawState)
    {
      buttons[i].lastDebounceTime = currentMillis;
      buttons[i].lastRawState = rawRead;
    }

    if ((currentMillis - buttons[i].lastDebounceTime) > debounceDelay)
    {
      if (rawRead != buttons[i].debouncedState)
      {
        buttons[i].debouncedState = rawRead;
        if (buttons[i].debouncedState == LOW)
        {
          buttons[i].pressStartTime = currentMillis;
          buttons[i].longPressHandled = false;
        }
      }
    }
  }
}

// 
ButtonEvent InputManager::getEvent(ButtonID btn)
{
  if (btn >= BTN_COUNT) return BTN_EVENT_NONE;
  
  unsigned long currentMillis = millis();

  // Create an object with the referance of the button ID
  ButtonState &b = buttons[btn];

  if (b.debouncedState == LOW)
  {
    if (!b.longPressHandled && (currentMillis - b.pressStartTime >= longPressTime))
    {
      b.longPressHandled = true;
      return BTN_EVENT_LONG_PRESS;
    }
    
    else if (b.longPressHandled)
    {
      return BTN_EVENT_HOLD;
    }
  }
  else
  {
    if (b.pressStartTime > 0)
    {
      bool wasLong = b.longPressHandled;
      b.pressStartTime = 0;
      b.longPressHandled = false;

      if (!wasLong) return BTN_EVENT_CLICK;
    }
  }
  return BTN_EVENT_NONE;
}

// Helper function to increase/decrease values till the button is released
bool InputManager::isPressed(ButtonID btn) { return (buttons[btn].debouncedState == LOW); }
