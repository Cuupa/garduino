#include "LowPower.h"
#include "PowerSaving.h"

void sleep(int minutes) {
    int counter = minutes * 15;
    for(int i = 0; i < counter; i++) {
        delay(4000);
        //LowPower.idle(SLEEP_4S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    }
}