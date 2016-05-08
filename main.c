#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define PULSE_DURATION 50
#define INPUT_PIN PCINT3
#define OUTPUT_PIN PB0
#define DEBOUNCE_TIME 200

#define EITHER 0
#define RISING 1
#define FALLING 2
#define TRIGGER_TYPE RISING

#define OFF_STATE 1 

// Clear bit
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
// Set bit
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

typedef enum { OFF, ON } state_t;
state_t edgeState = OFF;

void pulse() {
    cbi(PORTB, OUTPUT_PIN);
    _delay_ms(PULSE_DURATION);
    sbi(PORTB, OUTPUT_PIN);
}

/*ISR(PCINT0_vect) {*/
    /*if (bit_is_set(PINB, INPUT_PIN) && edgeState == OFF) {*/
        /*pulse();*/
        /*edgeState = ON;*/
    /*}*/
    /*else if (bit_is_clear(PINB, INPUT_PIN)) {*/
        /*edgeState = OFF;*/
    /*}*/
    
    /*_delay_ms(DEBOUNCE_TIME);*/
/*}*/

ISR(PCINT0_vect) {
#if TRIGGER_TYPE == EITHER
    pulse();
#else
    uint8_t v1;
    uint8_t v2; 
#if TRIGGER_TYPE == RISING
    v1 = bit_is_set(PINB, INPUT_PIN);
    v2 = bit_is_clear(PINB, INPUT_PIN);
#elif TRIGGER_TYPE == FALING
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
    PORTB = _BV(OUTPUT_PIN);

    while(1) { sleep(); };

    return 0;
}
