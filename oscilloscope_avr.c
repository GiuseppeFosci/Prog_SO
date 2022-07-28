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

void UART_init(void){
	UBRR0H = (uint8_t)(MYUBRR>>8);
	UBRR0L = (uint8_t)MYUBRR;
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
}

void UART_putChar(uint8_t c){
  while ( !(UCSR0A & (1<<UDRE0)) );
  UDR0 = c;
}

void UART_putString(uint8_t* buf){
  while(*buf){
    UART_putChar(*buf);
    ++buf;
  }
}
//Michele
ISR(USART0_RX_vect){
	uint8_t c=UDR0;
	if(c == 'y'){	//y per far partire l'ADC
			ADCSRA = 0xBF;
			ADCSRA |= 0x40;
	}
	if(c == 'n'){	//n per far fermare l'ADC
			ADCSRA = 0x00;
			ADCSRA = 0xBF;
	}
}
//Michele

//Gabriele e Michele
ISR(ADC_vect){
	measurement=(uint8_t)ADC;
	interrupt_flag=1;
}
//Gabriele e Michele

int main(void){
	//Gabriele e Michele
	cli();
	UART_init();
	ADCSRB = 0x00;
	ADMUX = 0x40;
	sei();
	while(1){
		if(interrupt_flag){
		sprintf(str_to_get, "%d\n",(int)measurement);
		UART_putString((uint8_t*)str_to_get);
		interrupt_flag=0;
		}
	}
	//Gabriele e Michele
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
 * cat /dev/ttyXXXX (only after 1# & 2#)
 * 
 * 
 * ATTENTION! Note that XXXX is your arduino device code on your PC
 * ATTENTION! Don't forget to stop writing
*/
