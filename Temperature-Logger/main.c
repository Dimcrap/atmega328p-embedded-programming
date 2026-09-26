#include <stdint.h>
#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VAL ((F_CPU) / (16UL*BAUD)) - 1

#include <avr/io.h>
#include <util/delay.h>
#include <time.h>
//#include


#define TX_BUF_SIZE 64

static volatile char tx_buff[TX_BUF_SIZE];
static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;

static void uart_init(void);

static void getfromsensor(void);

static void uart_putc(char c);
static void uart_getc(void);
static void uart_puts(const char *s);
static void uart_put16(uint16_t val);
static void uart_put32(uint16_t val);

//adc handlig
static void  adc_init(void);


static void tx_enqueue(char c);

ISR(USART_UDRE_vect)
{
    if(tx_head != tx_tail){
        UDR0 = tx_buff[tx_tail];
        tx_tail = (tx_tail + 1) % TX_BUF_SIZE;
    }else{
        UCR0B &= ~( 1 << UDRIE0 );//disalb intrupt
    }
}

static void tx_puts(const char *s){
    while(*s) tx_enqueue(*s++);
}





int main(void){



}



static void uart_init(void){
        UBRR0H = (uint8_t)(UBRR_VAL >> 8);
        UBRR0L = (uint8_t)(UBBRL_VAL);
        /* enable end and recieve */
        UCSR0B = ( 1 << TXEN0) | (1  << RXEN0 );
        /*frame format*/
        UCSR0C = ( 1 << UCSZ01 ) | (1<<UCSZ00);
};

static void getfromsensor(void){

};


static void uart_putc(char c){
    while( !UCSR0A & (1 << UDRE0)));
    UDR0= c;
};

static void uart_getc(void){
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
};

static void uart_puts(const char *s){
    while(*s)uart_putc(*s++);
}

static void uart_put16(uint16_t val){
    char buff[6];
    utoa(val,buff,10);
    uart_puts(buff);
};

static void uart_put32(uint16_t val){
    char buff[11];
    ultoa(val,buff,10);
    uart_puts(buff);
};

static void  adc_init(void){
    ADMUX = (1 << REFS0);
    ADC
};


static void tx_enqueue(char c){
    uint8_t next = (tx_head + )% TX_BUF_SIZE;
    while(next == tx_tail); //if buffer is full
    tx_buff[tx_head] = c ;
    tx_head = next ;
    UCSR0B |= ( 1 << UDRIE0 );
};
