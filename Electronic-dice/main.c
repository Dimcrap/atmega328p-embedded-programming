#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <time.h>

static void delay_ms(uint16_t ms){
	while(ms--) _delay_ms(1);
};

#define LED_MASK ( (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD5) )
#define BTN_PIN PD6



static uint16_t lfsr = 0xAEB1;

static uint8_t roll_dice(void){

	uint8_t lsb = lfsr & 1;
	lfsr >>= 1;
	if(lsb) lfsr ^= 0xB400;

	return ( lfsr % 7 ) + 1;
}

static void show_dice(uint8_t value)
{

	PORTD &= ~LED_MASK;
	
	uint8_t pattern = 0;
	switch(value){
		case 1: pattern = (1<<PD3); break;
		case 2: pattern = (1<<PD2) | (1<<PD5); break;
		case 3: pattern = (1<<PD2) | (1<<PD4) | (1<<PD5); break;
		case 4: pattern = (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD5); break;
		case 5: 
			pattern = (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD5);
			PORTD |= pattern;
			delay_ms(250);
			PORTD &= ~LED_MASK;
			delay_ms(250);
			break;
		case 6:
			pattern = (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD5);
			PORTD |= pattern;
			delay_ms(200);
			PORTD &= ~LED_MASK;
			delay_ms(200);
			PORTD |= pattern;
			delay_ms(200);
			PORTD &= ~LED_MASK;
			delay_ms(200);
			break;
		case 7:
			pattern = (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD5);
			PORTD |= pattern;
			delay_ms(200);
			PORTD &= ~LED_MASK;
			delay_ms(200);
			PORTD |= pattern;
			delay_ms(200);
			PORTD &= ~LED_MASK;
			delay_ms(200);
			PORTD |= ( 1 << PD2 ) | (1 << PD3) | (1 << PD4) ;
			delay_ms(300); 
			break;

	}
	PORTD |= pattern;

}


static uint8_t button_pressed(void){
	if(!(PIND & (1 << BTN_PIN))){
		_delay_ms(50);
		if(!(PIND & (1 << BTN_PIN))){
			return 1;
		}
	}
	return 0;
}

static uint8_t button_holdcheet(void){
	if(!(PIND & ( 1 << BTN_PIN ))){
		_delay_ms(820);
		if(!(PIND & ( 1 << BTN_PIN))){
			return 1;
		}
	}
	return 0;
}


int main(void){
	DDRD |= LED_MASK;

	DDRD &= ~(1 << BTN_PIN);
	PORTD |= (1 << BTN_PIN);

	uint8_t last_value = 1;
	show_dice(last_value);

	while(1){
		(void)roll_dice();
		if(  button_pressed() ){
			for(uint8_t i = 0; i < 10 ; i++){
				show_dice((i%6)+1);
				delay_ms( 50 + i * 20 );
			}
		last_value = roll_dice();
		show_dice(last_value);

		while (!(PIND & (1 << BTN_PIN))){
			if(button_holdcheet()){
				show_dice( (uint8_t) 3);
			}
		};
		_delay_ms(50);
		};
	}
}

