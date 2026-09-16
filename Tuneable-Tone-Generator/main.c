#include <stdint.h>
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

typedef uint16_t midint;


volatile uint8_t ms_ticks=0;
ISR(TIMER0_OVF_vect){ms_ticks++;};


static const uint16_t notes[] = {
    3816, 3607, 3400, 3213, 3029, 2862,  /* C4  to F4  */
    2702, 2550, 2406, 2272, 2144, 2023,  /* F#4 to B4  */
    1910, 1803, 1702, 1607, 1516, 1431,  /* C5  to F5  */
    1350, 1275, 1203, 1135, 1072, 1011,  /* F#5 to B5  */
    954                                    /* C6         */
};

enum {
    C4, Cs4, D4, Ds4, E4, F4,
    Fs4, G4, Gs4, A4, As4, B4,
    C5, Cs5, D5, Ds5, E5, F5,
    Fs5, G5, Gs5, A5, As5, B5,
    C6
};


static const uint8_t melody[] = {
    E4, E4, F4, G4,   G4, F4, E4, D4,
    C4, C4, D4, E4,   E4, D4, D4,
    E4, E4, F4, G4,   G4, F4, E4, D4,
    C4, C4, D4, E4,   D4, C4, C4
};


static const uint16_t durations[] = {
    300, 300, 300, 300,  300, 300, 300, 300,
    300, 300, 300, 300,  450, 150, 600,
    300, 300, 300, 300,  300, 300, 300, 300,
    300, 300, 300, 300,  450, 150, 600
};



#define NUM_NOTES ( sizeof(notes) / sizeof(notes[0]) )
#define BTN_UP PD6
#define BTN_DOWN PD7
#define BTN_MUTE PB0

void delayms(midint delay){
    while(delay--){ _delay_ms(1); };
}

static uint8_t debounce(uint8_t pin){
    uint8_t start= ms_ticks;
    if(!(PIND & (1 << pin))){
        //_delay_ms(50);
        while((uint8_t)(ms_ticks - start) < 50 ){};
        if(!(PIND & (1 << pin))){
            return 1;
        }
    }

    return 0;
}

static uint8_t offpush(void){
    if(!(PINB & (1 << BTN_MUTE))){
        _delay_ms(50);
        if(!(PINB & (1 << BTN_MUTE))){
            return 1;
        }
    }
    return 0;
}


static void play_note( uint8_t * melodylist , midint length );



int main(void){

    DDRB |= (1<<PB1) ;
    DDRB &= ~( ( 1 << BTN_MUTE ) );
    PORTB |= ( 1 << BTN_MUTE );

    DDRD &= ~( (1<<BTN_UP) | (1 << BTN_DOWN) ) ;
    PORTD |= ( 1 << BTN_UP) | (1 << BTN_DOWN) ;
    /*timer0*/
    TCCR0A = 0x00;//( 1 << WGM01 );
    TCCR0B &= ~( 1 << WGM02 );

    TCCR0B |= (1 << CS00) ;  //( 1<< CS01) | ( 1 << CS00 );
    TIMSK0 |=  ( 1 << TOIE0 );
    //TCNT0 = 0x00;

    TCCR1A = ( 1 << COM1A0 );
    TCCR1B = ( 1 << WGM12 ) | ( 1 << CS11);

    uint8_t note_idx = 9;
    OCR1A = notes[note_idx];
    sei();

    uint8_t mute = 0;

    while(1){

        if(offpush()){
            mute = ( mute==0 ) ? 1 : 0 ;
                if(mute==1)OCR1A=0;
                else OCR1A=notes[note_idx];
            while(!(PINB & (1 << BTN_MUTE)));
            _delay_ms(50);
        }

        if(mute == 0){

            //play_note((uint8_t *)melody[0],sizeof(melody)/sizeof(melody[0]));
            if(debounce(BTN_UP)){

            if( note_idx < NUM_NOTES - 1 ){
                note_idx++;
                OCR1A = notes[note_idx];
            }
            while(!(PIND & ( 1 << BTN_UP)));
            _delay_ms(50);
        }

        if(debounce(BTN_DOWN)){
            if(note_idx>0){
                note_idx--;
                OCR1A = notes[note_idx];
            }
            while(!(PIND & (1 << BTN_DOWN)));
            _delay_ms(50);
        }

        }
        };

        cli();
}



static void play_note( uint8_t * melodylist , midint length )
{
    //midint start;
    for( midint i = 0 ; i < length ; i++  ){
            OCR1A = notes[*(melodylist+i)]  ;
            //start = ms_ticks;
            //while( (uint8_t )(ms_ticks - start)  < /*durations[i]*/255 ){};
            delayms(durations[i]);


    }

};

