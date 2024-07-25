#ifndef __CONFIG_H__
#define __CONFIG_H__

// Input / Output pins
#define INPUT_PIN PCINT3
#define OUTPUT_PIN PB0

// Times in ms
#define PULSE_DURATION 30
#define DEBOUNCE_TIME  50  // was 100

// RISING, FALLING, EITHER
#define TRIGGER_TYPE RISING
// RISING = being disconnected
// FALLING = being connected

// If defined, active low, else active high
#define OFF_HIGH

// Pull-up resistor configuration
// Enable or disable the internal pull-up resistor for the input pin
#define PULLUP_ENABLED 1

#endif /* __CONFIG_H__ */
