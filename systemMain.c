/* Name: systemMain.c
 * Author: John Cook
 */

#include <avr/io.h>
#include "USI_TWI_Master.h"
#include <util/delay.h>
#include <stdint.h>

#define I2C_MAX_MSG_SIZE 5

bool static PASS = true;
bool static FAIL = false;

void long_delay_ms(uint16_t ms) {
    for (ms /= 10;ms>0;ms--) {
        _delay_ms(10);
    }
}

// returns true if downward acceleration is too fast (need exact value)
bool checkAccel() {
    // construct a command to read channel 0 of a 9150 accelerometer
    i2cMessageBuf[0] = (uint8_t) 0xD0;
    
    // send the message
    USI_TWI_Start_Read_Write(i2cMessageBuf,(uint8_t) 0x02);
    USI_TWI_Start_Read_Write(0x75, (uint16_t) 0x43); // gyro x - ????
    
    // wait
    long_delay_ms(50);
    
    // read the msg
    i2cMessageBuf[0]= 0xD1; // lsb == 1 => read
    USI_TWI_Start_Read_Write(i2cMessageBuf,(uint8_t) 0x03);
    
    // wait
    long_delay_ms(500);
    
    // check value
    if (/*i2cMessageBuf[x] > y*/) {
        return FAIL;
    }
    
    return PASS;
}

// initialize onboard ADC
void initADC() {
    DDRB   |=  (1<<PB5);     ///PB5/digital 13 is an output
    ADCSRA |=  ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //Prescaler at 128 so we have an 125Khz clock source
    ADMUX  |=  (1<<REFS0);
    ADMUX  &= ~(1<<REFS1);   //Avcc(+5v) as voltage reference
    ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));    //ADC in free-running mode
    ADCSRA |=  (1<<ADATE);   //Signal source, in this case is the free-running
    ADCSRA |=  (1<<ADEN);    //Power up the ADC
    ADCSRA |=  (1<<ADSC);    //Start converting
}

// get voltage reading
bool checkVoltage() {
    adc_value = ADCW;    // read the ADC value
    if(adc_value > 0){
        return PASS;
    }
    else {
        return FAIL;
    }
}

// activates parachute servo via PWM on PB4
void deployParachute() {
    PLLCSR |= (1 << PLLE) | (1 << PCKE);
    
    // Set prescaler to PCK/2048
    // (Setting CS (clock select) to 3)
    TCCR1 |= (1 << CS10) | (1 << CS11) | (0 << CS12) | (0 << CS13);
    
    // Output Compare Register
    OCR1B = 128;
    OCR1C = 255;
    
    // Enable OCRB output on PB4, configure compare mode and enable PWM B
    DDRB  |= (1 << PB4);
    GTCCR |= (1 << COM1B0) | (1 << COM1B1);
    GTCCR |= (1 << PWM1B);
    
    TCCR1 |= (1 << COM1A0);
    
    while (1) {} // run infinitely
    
    // never reached
    return 0;
}

int main(void) {
    uint8_t i2cMessageBuf[I2C_MAX_MSG_SIZE];
    USI_TWI_Master_Initialise();
    initADC();
    
    // runs continuously
    for(;;){
        if (checkAccel() == FAIL || checkVoltage() == FAIL) {
            deployParachute();
        }
    }
    // never reached
    return 0;
}