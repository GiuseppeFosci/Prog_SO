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

char str_to_get[MAX_BUF];
volatile uint8_t interrupt_flag = 0;
volatile uint8_t measurement;

char str_array_to_stream [256][5];
uint8_t str_idx_usart = 0;
uint8_t char_idx_usart = 0;
uint8_t str_idx_adc = 0;

void UART_init(void){
	UBRR0H = (uint8_t)(MYUBRR>>8);
	UBRR0L = (uint8_t)MYUBRR;
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
}

//Michele
ISR(USART0_UDRE_vect){
	if(str_array_to_stream[str_idx_usart][char_idx_usart]){
		UDR0=str_array_to_stream[str_idx_usart][char_idx_usart];
		char_idx_usart++;
	}else{
		if(str_idx_usart>255) {
				str_idx_usart=0;
				char_idx_usart=0;
			}else {
				str_idx_usart+=1;
				char_idx_usart=0;
				}
		}
	UCSR0B &= ~(1 << UDRIE0);
}

ISR(USART0_RX_vect){
	uint8_t c=UDR0;
	if(c == 'y'){	//y to start the ADC
		ADCSRA = 0xBF;
		ADCSRA |= 0x40;
	}
	if(c == 'n'){	//n to stop the ADC
		ADCSRA = 0x00;
		ADCSRA = 0xBF;
	}
}
//Michele

//Gabriele and Michele
ISR(ADC_vect){
	measurement=(uint8_t)ADC;
	if(str_idx_adc>255){
		str_idx_adc=0;
	}
	sprintf(str_array_to_stream[str_idx_adc], "%d\n",(int)measurement);
	str_idx_adc+=1;
	UCSR0B |= (1 << UDRIE0);
}
//Gabriele and Michele

//Gabriele and Michele
int main(void){
	cli();
	UART_init();
	ADCSRB = 0x00;
	ADMUX = 0x40;
	sei();
	while(1){}
}
//Gabriele and Michele

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
