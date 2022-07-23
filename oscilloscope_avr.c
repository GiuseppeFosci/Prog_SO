#include <util/delay.h>
#include <stdio.h>
#include "avr_common/uart.h" // Inizializza seriale per supportare printf
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h> //Mi permette di definire interi a 8, 16 o 32 bit
//Michele
//Ogni volta che c'è la fine di una conversione viene avviato un interrupt e salvata la misura in una variabile
volatile uint16_t misura;
ISR(ADC_vect){
	misura=ADC;
}
//Michele

//Giuseppe
int main(void){
 // Inizializzazione printf/uart
  printf_init();
  //disabilito tutti gli interrupt
  cli();
 //Configuro i vari PIN come input, sono i bit 0:7 della porta F
  const uint8_t mask=0xFF; 
  DDRF  &= ~mask;
  
  //Imposto valore di default dei PIN basso (0V)
  PORTF &= ~mask;

  /*Disabilito Power Reduction ADC Bit  (PRADC) nel registro PRR0 (Power REduction Register)
   Lasciando però inalterato il valore degli altri bit di PRR0*/
   const uint8_t mask_PRADC =0x01; 
  PRR0 &= ~mask_PRADC;

  
  /*Scelta canale analogico - MUX 4 in ADMUX e MUX5 in ADCRSB - 00000000 per ADC0*/
  ADCSRB = 0x00;
  ADMUX = 0x00;
  //Giuseppe
  
  //Michele
  //imposto ADCSRA, ovvero abilito l'ADC ponendo ADEN = 1, pongo il flag ADIF=1 per dire che la conversione non è in corso, 
  //configuro il clock dell'ADC con cui viene campionato in base alla frequenza dell'arduinomega2650
  //abilitiamo anche l'interrupt ponendo ADIE=1
  ADCSRA = 0x9F;

  //imposto il voltaggio di riferimento di ADMUX = AVCC e modalità right aligned.
  ADMUX = 0x40;
  //Michele
  //riabilito gli interrupt
  sei();
  while(1){
	//Giuseppe e Michele
    const uint8_t mask_ADSC = 0x40;
    //Faccio iniziare la conversione ponendo ADSC = 1
    ADCSRA |= mask_ADSC;
    printf("Valore di conversione (1024=5V, 0=0V) :  %d\n", (int) misura);
    _delay_ms(100);
    //Giuseppe e Michele
  }
}
