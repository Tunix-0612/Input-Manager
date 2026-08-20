# Input Manager

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Build-orange?style=flat-square&logo=platformio)](https://platformio.org/)

A lightweight, memory-efficient input handling module designed to manage button states and handle edge events seamlessly in embedded projects.

This Module can be updated easily in an existing program as long as only the pre-defined 4 buttons are used.

---

## Key Features

- **Optimized Memory Footprint**
  - Centralized pin-polling architecture drastically cuts down FLASH and RAM overhead.
  - Pre-configured for up to 4 concurrent inputs, easily scalable directly in the header configuration.

- **Integrated Software Debouncing**
  - Configurable debounce thresholds eliminate noisy contacts and prevent false triggers.

- **Event-Driven Architecture**
  - Non-blocking state processing powered by a single `update()` loop.
  - Built-in detection for critical press types including `BTN_EVENT_CLICK`, `BTN_EVENT_HOLD`, `BTN_EVENT_LONG_PRESS` and `BTN_EVENT_NONE`.
 
---

## Usage

**Declare and initialize the module:**
```cpp
// Include the file
#include "InputManager.h" 

// Define Up, Down, Left, and Right pins in order
InputManager inputManager(2, 3, 4, 5);

void setup() 
{
  // Initialize internal pin configurations (INPUT_PULLUP)
  inputManager.begin();
}
```

**Basic Button Handling:**
```cpp
// UI is prefered to be in a loop and break with any button or condition
while (true) 
{
  // Always update the button states at the beginning of the loop
  inputManager.update();

  // Poll only the required buttons to save stack memory
  ButtonEvent eventLeft  = inputManager.getEvent(BTN_LEFT);
  ButtonEvent eventRight = inputManager.getEvent(BTN_RIGHT);

  // Handle standard click
  if (eventLeft == BTN_EVENT_CLICK) 
  {
    // Action on single click
  }

  // Handle long press (triggers once after threshold)
  if (eventRight == BTN_EVENT_LONG_PRESS) 
  {
    // Action on long press trigger
  }
}
```

---

**Handling Continuous Button Hold**
  - **v1.1.x and Newer**

```cpp
ButtonEvent evt = inputManager.getEvent(BTN_UP);

if (evt == BTN_EVENT_HOLD) 
{
  // Fires on every update cycle after long press threshold is reached
  // Add a non-blocking interval or small delay to control repeat rate
}

```

  - **v1.0.x Legacy**

```cpp
unsigned long currentMillis = millis();

if (currentMillis - lastFastChangeMillis >= FAST_CHANGE_INTERVAL) 
{
  lastFastChangeMillis = currentMillis;

  if (inputManager.isPressed(BTN_UP)) 
  {
    // Increment value continuously while held
  }
}
```

---
