#include <avr/io.h>
#include <avr/delay.h>

int main() {
    
    PLLCSR |= (1 << PLLE) | (1 << PCKE);

    // Set prescaler to PCK/2048
    TCCR1 |= (1 << CS10) | (1 << CS11) | (0 << CS12) | (0 << CS13);

    OCR1B = 128;
    OCR1C = 255;

    // Enable OCRB output on PB4, configure compare mode and enable PWM B
    DDRB  |= (1 << PB4);
    GTCCR |= (1 << COM1B0) | (1 << COM1B1);
    GTCCR |= (1 << PWM1B);

    TCCR1 |= (1 << COM1A0);
        
	while (1) {} // run infinitely

	return 0;
}