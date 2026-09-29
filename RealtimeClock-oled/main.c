#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.c>
#include <util/atomic.h>


volatile uint32_t seconds = 0 ;


ISR(TIMER1_COMPA_vect)
{
    static uint16_t subsec = 0;
    subsec++;
    if(subsec >= 1000){
        subsec = 0;
        seconds++;
    }
}

static void timer1_init(void){
    TCCR1A = 0 ;

}


#define OLD_DC PB1
#define OLED_RST PB0
#define OLDED_CS PB2


static void oled_cmd(uint8_t cmd);


