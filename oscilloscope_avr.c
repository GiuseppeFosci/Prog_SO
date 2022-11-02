#include <util/delay.h>
#include <stdio.h>
#include "avr_common/uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include<stdlib.h>
#include<stdio.h>
//#include <linux/init.h>
//#include <linux/module.h>
//MODULO GESTIONE FILE OPERATIONS
//#include <linux/fs.h>


#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUF 256
#define MAX_CHAR 5

void SetPrescaler(int c); //Prototipo funzione per prescaler
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

	cli();
	 SetPrescaler(c);
	 sei();

	}
				
ISR(USART0_UDRE_vect){
	UDR0 = (unsigned int) ADCH;
	UCSR0B &= ~(1 << UDRIE0);
	ADCSRA |= 0x40;
}
//Michele

//Giuseppe and Michele
ISR(ADC_vect){
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
	PRR0 &= ~mask_PRADC; //Disabilito power saving

	
	ADCSRB = 0x00;
	ADMUX = 0x60; // 0110 0000 - Reference voltage & right aligned
	
	sei();
	while(1){}
	//Giuseppe and Michele
}
//Giuseppe
void SetPrescaler(int c){
		
	switch (c)    //Imposto ADPS2 -ADPS1-ADPS0  
	{   case 48:
		
		UCSR0B &= ~(1 << UDRIE0); 	

		ADCSRA = 0x00;
		ADCSRA = 0x9F;  
		
		break; 

		case 49:  
		//0-0-0 - Division Factor 2
		
		ADCSRA = 0x98;
		ADCSRA |= 0x40; //Setto bit ADSC a 1 - Start conversion
		break; 
		case 50: 
	        //0-0-1 - Division Factor 2
		ADCSRA = 0x99;
		ADCSRA |= 0x40;
		break;
		case 51: 
		//0-1-0 -Division Factor 4
		ADCSRA = 0x9A;
		ADCSRA |= 0x40;
		break;
		case 52: 
		//"0-1-1 -Division Factor 8
		ADCSRA = 0x9B;
		ADCSRA |= 0x40;
		break;
		case 53: 
		//1-0-0 -Division Factor 16
		ADCSRA = 0x9C;
		ADCSRA |= 0x40;
		break;
		case 54: 
		//1-0-1 -Division Factor 32
		ADCSRA = 0x9D;
		ADCSRA |= 0x40;
		break;
		case 55: 
		//1-1-0 -Division Factor 64
		ADCSRA = 0x9E;
		ADCSRA |= 0x40;
		break;
		case 56: 
		//1-1-1 -Division Factor 128
		ADCSRA = 0x9F;
		ADCSRA |= 0x40;
		break;	
		default: 
		//Errore impostazione Prescaler;
		break;
	}	
} //Giuseppe

