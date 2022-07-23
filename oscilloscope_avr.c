#include <util/delay.h>
#include <stdio.h>
#include "avr_common/uart.h" // Inizializza seriale per supportare printf
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h> //Mi permette di definire interi a 8, 16 o 32 bit

//Giuseppe
int main(void){

 // Inizializzazione printf/uart
  printf_init();
  
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
  //imposto ADCSRA, ovvero abilito l'ADC ponendo ADEN = 1, pongo il flag ADIF=1 per dire che non sta convertendo attualmente, 
  //configuro il clock dell'ADC con cui viene campionato in base alla frequenza dell'arduinomega2650
  ADCSRA = 0x97;

  //imposto il voltaggio di riferimento di ADMUX = AVCC e modalità right aligned.
  ADMUX = 0x40;
  //Michele

  while(1){
	//Gabriele e Michele
    const uint8_t mask_ADSC = 0x40;
    //Faccio iniziare la conversione ponendo ADSC = 1
    ADCSRA |= mask_ADSC;
    while(ADIF==0){ /*Se ADIF = 1, la conversione è terminata, perciò se ADIF = 0 aspetto finché non finisce la conversione */ }
    printf("Valore di conversione (1024=5V, 0=0V) :  %d\n", (int) ADC);
    _delay_ms(1000);
    //Gabriele e Michele
  }
}
