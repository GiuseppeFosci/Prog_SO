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

  



  //----------------------------------------------
    //IMPOSTO REGISTRO ADMUX (Eccetto ADLAR che verrà impostato in seguito)
  
    //Imposto Bit 4:0 IN ADMUX Analog channel and Gain selection Bits (PAG 282)
    //Seleziono ADC0 come canale 
    const uint8_t mask_MUX4 = 0x00;
    ADMUX |=mask_MUX4;


  //---------------------------------------------------

  
//impostazione voltaggio riferimento REFS1:0 in ADMUX.
     ADMUX |= 0xc0;
     

   //ADC enabled settando ADC Enable bit (ADEN) ad 1, in ADCSRA. ADEN è il bit 7 di ADCSRA
    const uint8_t mask_ADEN = 0x80;
    ADCSRA |= mask_ADEN;

    
  //Abilitazione ADC Interrupt Enable bit (ADIE) in ADCSRA
  const uint8_t mask_ADIE = 0x08;
  ADCSRA |= mask_ADIE;

  /*La conversione inizia se setto ADC Auto Trigger Enable bit. ADATE in ADATE in ADCSRA.
    ADATE è il BIT 5 di ADCSRA*/
    const uint8_t mask_ADATE = 0x20;
    ADCSRA |= mask_ADATE;

    // Faccio partire la prima conversione manualmente, poi sarà l'autotrigger a far partire le successive
    //imposto quindi ADSC a 1 
    const uint8_t mask_ADSC = 0x40;
    ADCSRA |= mask_ADSC;

    printf("ADCSRA Value :  %d\n", (int) ADCSRA);
 

  //---------------------------------------------------------------------
  
  //Imposto Free running mode bit ADTS2:0 in ADCSRB (Pag 287), MUX5*, va fatto dopo aver impostato ADATE
    const uint8_t mask_freerunning = 0x00;
    ADCSRB |= mask_freerunning; 

  printf("ADCSRA Value :  %d\n", (int) ADCSRA);

    

  

  while(1){
     //Inoltre bisogna attivare Global Interrupt Enable Bit - (BIT 7-I) in SREG - AVR Status Register
  
      const uint8_t mask_Gint = 0x80;
      SREG |= mask_Gint;
       
    
    //Voglio leggere gli 8 bit in input
    const uint8_t input_bit = PINF;
    printf("PINF :  %x\n", (int) input_bit);
    
        
    //Lettura risultato su ADC
    if(ADIF==1){ //Se ADIF è 1, la conversione è terminata
    //IMPOSTO ADLAR - bit 5 in ADMUX per risultato con left adjustment
      ADMUX |=0x20;
      printf("Conversion Value :  %lf\n", (double) ADCH);
   
    }
    
    _delay_ms(200); // from delay.h, wait 1 sec
  

  }
  
}
  


