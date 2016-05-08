# TinyTrigger

AVR firmware for the ATtiny85 to act as a simple pulse trigger. Can be configured
at compile time to trigger on rising edge, falling edge or both and also to an
active high or active low output pulse.

Uses interrupts and sleeps the uC except when a change is detected, so current
draw is extremely small (few uA).

Originally meant to wake an ESP8266 on an external change, since the ESP8266
requires a brief low pulse to wake from deep sleep.
