#include <util/delay.h>
#include <stdio.h>
#include "avr_common/uart.h" // Inizializza seriale per supportare printf
#include <avr/io.h>
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
  printf("PRADC Value :  %x\n", (int) PRADC);
  
  //ADC enabled settando ADC Enable bit (ADEN) ad 1, in ADCSRA. ADEN è il bit 7 di ADCSRA
  const uint8_t mask_ADEN = 0x80;
  ADCSRA |= mask_ADEN; 
  /*Scelta canale analogico - MUX 4 in ADMUX e MUX5 in ADCRSB - 00000000 per ADC0*/
  ADCSRB = 0x00;
  ADMUX = 0x00;
   if(ADCSRB==0x00 && ADMUX==0x00){
      printf("Conversione sul canale scelto ADC0\n");
    }
  //Abilitazione ADC Interrupt Enable bit (ADIE) in ADCSRA
  const uint8_t mask_ADIE = 0x08;
  ADCSRA |= mask_ADIE;
  //Inoltre bisogna attivare Global Interrupt Enable Bit - (BIT 7-I) in SREG - AVR Status Register
  
  const uint8_t mask_Gint = 0x80;
  SREG |= mask_Gint;
  //impostazione voltaggio riferimento REFS1:0 in ADMUX.
  ADMUX |= 0xc0;
  

  while(1){
    //Voglio leggere gli 8 bit in input
    const uint8_t input_bit = PINF;
    printf("PINF :  %x\n", (int) input_bit);
    /*La conversione inizia se setto ADC Auto Trigger Enable bit. ADATE in ADATE in ADCSRA.
    ADATE è il BIT 5 di ADCSRA*/
    const uint8_t mask_ADATE = 0x20;
    ADCSRA |= mask_ADATE;
    //IMPOSTO ADLAR - bit 5 in ADMUX per risultato con left adjustment
    ADMUX |=0x20;
    //Lettura risultato su ADC
    if(ADIF==1){ //Se ADIF è 1, la conversione è terminata

    printf("Conversion Value :  %lf\n", (double) ADCH);
   
    }

    _delay_ms(1000); // from delay.h, wait 1 sec
  

  }
}
  


