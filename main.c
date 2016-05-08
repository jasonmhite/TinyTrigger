// main.c
// 
// Active low edge trigger
//
// Input is pin 2 (PCINT3/PB1)
// Logic pulse on pin 2 for rising edge

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define PULSE_DURATION 50
#define N_DEBOUNCE 5
#define DEBOUNCE_TIME 2

int edge = 0;

void sleep() {
    GIMSK |= _BV(PCIE);
    PCMSK |= _BV(PCINT3);
    ADCSRA &= ~_BV(ADEN);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    sleep_enable();
    sei();
    sleep_cpu();

    cli();
    PCMSK &= ~_BV(PCINT3);
    sleep_disable();
    ADCSRA |= _BV(ADEN);

    sei();
}

void send_pulse() {
    PORTB = 0b00000001;
    _delay_ms(PULSE_DURATION);
    PORTB = 0b00000000; 
}

ISR(PCINT0_vect) {
    // Interrupt function
    if (bit_is_set(PINB, PB3) && edge == 0) {
        // Rising edge
        send_pulse();
        edge = 1;
    }
    else if (bit_is_clear(PINB, PB3)) {
        edge = 0;
    }
    
    _delay_ms(200);
}
 
int main (void)
{
  // set pin 5 to output and pull high
  DDRB =  0b00000001;
  PORTB = 0b00000000;

  while (1) { sleep(); }

  return 1;
}
