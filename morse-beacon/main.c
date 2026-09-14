#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <ctype.h>


#define DOT_MS 200
#define DASH_MS 600
#define SYMBOL_GAP 200
#define LETTER_GRAP 600
#define WORD_GAP 1400

static const char *morse[] = {
    ".-",   "-...", "-.-.", "-..",  ".",    "..-.", /* A-F */
    "--.",   "....", "..",   ".---", "-.-",  ".-..", /* G-L */
    "--",   "-.",   "---",  ".--.", "--.-", ".-.",  /* M-R */
    "...",   "-",   "..-",  "...-", ".--",  "-..-", /* S-X */
    "-.--", "--..", "-----", ".----", "..---", "...--", /* Y-Z 0-3 */
    "....-", ".....", "-...." , "--... ", "---..", /* 4-8 */
    "----."    					/*9*/
};

static void led_on(void)  { PORTB |= (1 << PB5); }
static void led_off(void) { PORTB &= ~(1 << PB5); }

static void delay_ms(uint16_t ms)
{
	while (ms--) _delay_ms(1);
}

static void blink_dot(void)
{
	led_on();
	delay_ms(DOT_MS);
	led_off();
	delay_ms(SYMBOL_GAP);
}

static void blink_dash(void)
{
	led_on();
	delay_ms(DASH_MS);
	led_off();
	delay_ms(SYMBOL_GAP);
}

static void send_char(char c)
{
	if( c == ' ' ){
		delay_ms(WORD_GAP);
		return;
	}		
	c = toupper((unsigned char) c);
 	if( c < '0' || c > 'Z' ) return ;

	/* 0=26 */
	const char * code = morse[(isalpha(c))?c - 'A': c-22 ];

	for(uint8_t i = 0; code[i] ; i++){
		if( code[i] == '.' ) blink_dot();
		else		blink_dash();
	}
	delay_ms(LETTER_GRAP);
}





int main(){
	DDRB |= (1<<PB1);
	
	while(1){
	PORTB |= (1<<PB1);
	delay_ms(2000);
	PORTB &= ~(1<<PB1);	
	delay_ms(1000);
	}

	/*
	DDRB |= (1 << PB5);

	const char * message = "c   00000";

	while(1) {
		for(uint8_t i = 0 ; i <strlen(message) ; i++){
			send_char(message[i]);
		}
		delay_ms( WORD_GAP * 2 ); 
	}
*/

}



