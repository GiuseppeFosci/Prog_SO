#include <util/delay.h>
#include <stdio.h>
#include "avr_common/uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUF 256
#define MAX_CHAR 5

//char array[MAX_BUF][MAX_CHAR];
//uint8_t idx=0;
//uint8_t idx_read=0;
uint8_t char_idx=0;
char str[256];

void UART_init(void){
	UBRR0H = (uint8_t)(MYUBRR>>8);
	UBRR0L = (uint8_t)MYUBRR;
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
}

//Michele
ISR(USART0_RX_vect){
	uint8_t c=UDR0;
	if(c == 'y'){	//y to start the ADC
			ADCSRA = 0x9F;
			ADCSRA |= 0x40;
	}
	if(c == 'n'){	//n to stop the ADC
		UCSR0B &= ~(1 << UDRIE0);
		ADCSRA = 0x00;
		ADCSRA = 0x9F;
	}
}
ISR(USART0_UDRE_vect){
	/*if(array[idx_read][char_idx]){
		UDR0=array[idx_read][char_idx];
		char_idx+=1;
	} else {
		idx_read+=1;
		char_idx=0;
		ADCSRA |= 0x40;
	}
	if(char_idx >= MAX_CHAR){
		idx_read+=1;
		if(idx_read >= MAX_BUF){
			idx_read=0;
			char_idx=0;
		}
		UCSR0B &= ~(1 << UDRIE0);
		ADCSRA |= 0x40;
	}*/
	if(str[char_idx]){
		UDR0=str[char_idx];
		char_idx+=1;
	} else {
		char_idx=0;
		UCSR0B &= ~(1 << UDRIE0);
		ADCSRA |= 0x40;
	}
}
//Michele

//Giuseppe and Michele
ISR(ADC_vect){
	/*sprintf(array[idx],"%d\n",(int)ADCH);
	idx+=1;
	if(idx >= MAX_BUF){
		idx=0;
	}
	UCSR0B |= (1 << UDRIE0);*/
	sprintf(str,"%d\n",(int)ADCH);
	UCSR0B |= (1 << UDRIE0);
}
//Giuseppe and Michele

int main(void){
	//Giuseppe and Michele
	cli();
	UART_init();
	const uint8_t mask=0xFF; 
	DDRF  &= ~mask;
	PORTF &= ~mask;
	const uint8_t mask_PRADC =0x01; 
	PRR0 &= ~mask_PRADC;
	ADCSRB = 0x00;
	ADMUX = 0x60;
	sei();
	while(1){}
	//Giuseppe and Michele
}

/* If you want to put measurements into a file
 * write this on the terminal.
 * 
 * 1.Put the process of writing on file in background
 * cat /dev/ttyXXXX > text.txt &     
 * 
 * 
 * 2.Start writing on the file
 * echo "y">/dev/ttyXXXX
 * 
 * 
 * 3.Stop writing on the file 
 * echo "n">/dev/ttyXXXX
 * 
 * 
 * 4.To see what are the measurements on the terminal
 * cat /dev/ttyXXXX (only after 1. & 2.)
 * 
 * 5.Kill the process open once finished
 * kill -9 $$$$$$
 * (where $$$$$$ is the code of the process opened in 1.)
 * 
 * ATTENTION! Note that XXXX is your arduino device code on your PC
 * ATTENTION! Don't forget to stop writing
*/
