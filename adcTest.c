#include <avr/io.h>

// 9.6 MHz, built in resonator
#define F_CPU 9600000

void adc_setup (void)
{
    // Set the ADC input to PB2/ADC1
    ADMUX |= (1 << MUX0);
    ADMUX |= (1 << ADLAR);
    // Set the prescaler to clock/128 & enable ADC
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
}

int main (void)
{
    adc_setup();
    
    while (1) {
        // Start the conversion
        ADCSRA |= (1 << ADSC);
        // Wait for it to finish
        while (ADCSRA & (1 << ADSC));
        
        if (ADCH > 128) {
            // turn on LED
            DDRB  |=  (1 << PB1);
            PORTB &= ~(1<<PB1);
            PORTB |=  (1 << PB1);
        }
        else {
            // turn off LED
            PORTB &= ~(1<<PB1);
        }
    }
}