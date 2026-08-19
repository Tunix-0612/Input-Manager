#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>

// Internal button IDs
enum ButtonID
{
  BTN_UP = 0,
  BTN_DOWN,
  BTN_LEFT,
  BTN_RIGHT,
  BTN_COUNT // Total Button Count
};

// Possible button events
enum ButtonEvent
{
  BTN_EVENT_NONE = 0,
  BTN_EVENT_CLICK,
  BTN_EVENT_LONG_PRESS,
  BTN_EVENT_HOLD
};

// Button stats
struct ButtonState
{
  byte pin;
  bool lastRawState;
  bool debouncedState;
  unsigned long lastDebounceTime;
  unsigned long pressStartTime;
  bool longPressHandled;
};

class InputManager
{
  private:
    ButtonState buttons[BTN_COUNT];
    const unsigned long debounceDelay = 35;   // Software based debounce protection delay
    const unsigned long longPressTime = 800;  // Long press timer

  public:
    InputManager(byte pinUp, byte pinDown, byte pinLeft, byte pinRight);

    void begin();
    void update();
    ButtonEvent getEvent(ButtonID btn);
    bool isPressed(ButtonID btn);
};

extern InputManager inputManager;

#endif
