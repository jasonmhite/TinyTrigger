#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

#define EITHER 0
#define RISING 1
#define FALLING 2 

// Clear bit
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
// Set bit
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#if TRIGGER_TYPE != EITHER
typedef enum { OFF, ON } state_t;
state_t edgeState = OFF;
#endif

void pulse() {
#ifdef OFF_HIGH
    cbi(PORTB, OUTPUT_PIN);
    _delay_ms(PULSE_DURATION);
    sbi(PORTB, OUTPUT_PIN);
#else
    sbi(PORTB, OUTPUT_PIN);
    _delay_ms(PULSE_DURATION);
    cbi(PORTB, OUTPUT_PIN); 
#endif
}

ISR(PCINT0_vect) {
#if TRIGGER_TYPE == EITHER
    pulse();
#else
    uint8_t v1;
    uint8_t v2; 
#if TRIGGER_TYPE == RISING
    v1 = bit_is_set(PINB, INPUT_PIN);
    v2 = bit_is_clear(PINB, INPUT_PIN);
#elif TRIGGER_TYPE == FALLING
    v1 = bit_is_clear(PINB, INPUT_PIN); 
    v2 = bit_is_set(PINB, INPUT_PIN);
#endif
    if (v1 && edgeState == OFF) {
        pulse();
        edgeState = ON;
    }
    else if (v2) {
        edgeState = OFF;
    } 
#endif
    _delay_ms(DEBOUNCE_TIME);    
}

void sleep() {
    // Set up sleep and interrupts
    cli();

    sbi(GIMSK, PCIE); // Enable pin change interrupt
    sbi(PCMSK, INPUT_PIN); // Set it up on input pin
    cbi(ADCSRA, ADEN); // Turn off the ADC
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    sleep_enable();
    sei();
    sleep_cpu();

    cli();

    cbi(PCMSK, INPUT_PIN);
    sleep_disable();
    sbi(ADCSRA, ADEN);

    sei();
}


int main() {
    // Set ports
    DDRB = _BV(OUTPUT_PIN);
#ifdef OFF_HIGH
    sbi(PORTB, OUTPUT_PIN);
#else
    cbi(PORTB, OUTPUT_PIN);
#endif

#ifdef PULLUP_ENABLED
    sbi(PORTB, INPUT_PIN);
#endif

    while (1) {
        sleep();
    };

    return 0;
}
