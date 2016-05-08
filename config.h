#ifndef __CONFIG_H__
#define __CONFIG_H__

// Input / Output pins
#define INPUT_PIN PCINT3
#define OUTPUT_PIN PB0

// Times in ms
#define PULSE_DURATION 30
#define DEBOUNCE_TIME 100

// RISING, FALLING, EITHER
#define TRIGGER_TYPE RISING

// If defined, active low, else active high
#define OFF_HIGH

#endif
