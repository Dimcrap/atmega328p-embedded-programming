
#include <stdint.h>
#include <sys/types.h>
#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VAL ((F_CPU) / (16UL*BAUD)) - 1

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>


#define TX_BUF_SIZE 64

static volatile char tx_buff[TX_BUF_SIZE];
static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;

static void uart_init(void);

static void getfromsensor(void);

static void uart_putc(char c);
//static void uart_getc(void);
static void uart_puts(const char *s);
static void uart_putu16(uint16_t val);
static void uart_putu32(uint16_t val);

//adc handlig
static void  adc_init(void);
static uint16_t adc_read(void);

static void tx_enqueue(char c);

ISR(USART_UDRE_vect)
{
    if(tx_head != tx_tail){
        UDR0 = tx_buff[tx_tail];
        tx_tail = (tx_tail + 1) % TX_BUF_SIZE;
    }else{
        UCSR0B &= ~( 1 << UDRIE0 );//disalb intrupt
    }
}

static void tx_puts(const char *s){
    while(*s) tx_enqueue(*s++);
}

volatile uint32_t ms_ticks=0;

ISR(TIMER1_COMPA_vect)
{
    ms_ticks++;
}

static void timer1_init(void);
static uint32_t get_ms(void);


int main(void){
    uart_init();
    adc_init();
    timer1_init();
    sei();

    DDRD |= ( 1 << PD5);


    uart_puts("timestamp_ms,adc_raw,temp_c\r\n");

    uint32_t next_sample=0;

    while(1){
        PORTD |= ( 1 << PD5 );

        uint32_t now = get_ms();
        if(now >= next_sample){
            next_sample = now + 1000;

            uint16_t adcvalue = adc_read();
            //apply change on input


            uart_putu32(now);
            uart_putc(',');

            uart_putu16(adcvalue);
            uart_putc(',');

            uint16_t celcius = adcvalue * (5.0 / 1023.0) * 100.0;
            uart_putu16(celcius);
            uart_putc('c');
            uart_puts("\r\n");*/
            //tx_puts("from tx is this");
        }

        //PORTD &= ~(1 << PD5);
        _delay_ms(1);
    }


}



static void uart_init(void){
        UBRR0H =  (uint8_t)(UBRR_VAL >> 8);
        UBRR0L = (uint8_t)(UBRR_VAL);
        /* enable send and recieve */
        UCSR0B = ( 1 << TXEN0);  //| (1  << RXEN0 );
        /*frame format*/
        UCSR0C = ( 1 << UCSZ01 ) | ( 1 << UCSZ00 );
};


static void uart_putc(char c){
    while( !(UCSR0A & (1 << UDRE0)));
    UDR0= c;
};
/*
static void uart_getc(void){
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
};*/

static void uart_puts(const char *s){
    while(*s)uart_putc(*s++);
}

static void uart_putu16(uint16_t val){
    char buff[6];
    utoa(val,buff,10);
    uart_puts(buff);
};

static void uart_putu32(uint16_t val){
    char buff[11];
    ultoa(val,buff,10);
    uart_puts(buff);
};

static void  adc_init(void){
    ADMUX = (1 << REFS0);
    ADCSRA = ( 1<<ADEN) | ( 1<< ADPS2 ) | (1 << ADPS1) | ( 1 << ADPS0 );
};


static uint16_t adc_read(void){
        ADCSRA |=  ( 1 << ADSC );
        while(ADCSRA & ( 1 << ADSC ));
        return ADC;
};


static void tx_enqueue(char c){
    uint8_t next = (tx_head + 1 )% TX_BUF_SIZE;
    while(next == tx_tail); //if buffer is full
    tx_buff[tx_head] = c ;
    tx_head = next ;
    UCSR0B |= ( 1 << UDRIE0 );
};


static void timer1_init(void){
    TCCR1A= 0;
    TCCR1B = (1 << WGM12) | (1 << CS11) | ( 1 << CS10);
    OCR1A = 249;
    TIMSK1 = (1 << OCIE1A);
};


static uint32_t get_ms(void){
    uint32_t val;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        val=ms_ticks;
    }
    return val;
};
