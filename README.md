OSCILLOSCOPIO SU ATM2650

L'oscilloscio consiste nel campionamento di 8 canali su ADC. L'ADC è connesso a un multiplexer analogico che permette 8/16 ingressi di tensione single-ended costruiti dai PIN della porta F e quelli della porta K.
L'ADC converte un voltaggio in input analogico in un valore digitale a 10BIT. Il minimo valore rappresenta 0V, ovvero GND, mentre il massimo rappresenta il voltaggio su AREF pin.


Per far partire una singola conversione è necessario scrivere un uno logico in ADC Start Conversion bit, ADSC ( Ovvero il BIT 6  dell' ADC Control e and status register ADCSRA). 
Questo bit rimane alto per tutto il tempo che la conversione è in corso, e sarà successivamente cancellato dall'hardware quando la conversione è terminata. Se sono selezionati differenti canali mentre la conversione è in corso, l'ADC finirà la conversione corrente prima di occuparsi degli altri.

In alternativa, una conversione può essere triggherata automaticamente  da varie sorgenti. Per abilitare l'auto trigghering è necessario settare ADC Auto Trigger Enable bit. ovvero ADATE in ADCSRA. Le sorgente del trigger viene selezionata settando ADC Trigger Select bits, ADTS in ADCSRB. Quando arriva un rising edge del clock, l'ADC prescaler è resettato e parte una nuova conversione. Questo fornisce un metodo per far partire conversioni ad intervalli prefissati. Se il segnale di triggher è ancora settato quando la conversione è terminata, allora non può partire una nuova conversione. Se un altro rising edge arriva sul triggher signal durante la conversione, esso verrà ignorato

L'ADC viene attivato settando ADC Enable bit, ADEN in ADCSRA. IL risulato della conversione sarà un valore a 10-bit presente in ADC Data Register, ADCH and ADCL. Se non è richiesta una precisione maggiore di 8-bit è possibile leggere solamente il registro ADCH.


IL REGISTRO ADCSRA - ADC Control and status Register A

BIT-7 - ADEN: ADC Enable
Scrivendo questo bit a 1 attiviamo ADC, mentre scrivendolo a 0 lo spegniamo. LO spegnimento mentre è in corso una cnversione farà terminare quella conversione.

BIT-6 - ADATE: ADC Start Conversion
Nella modalità a singola conversione, scrivendo questo bit a 1 faremo partire la conversione. 

BIT-4 - ADIF : ADC Interrupt Flag 
Questo Bit è settato quando la conversione è terminata e il Data Register si è aggiornato. L'interrupt è eseguito solo se il bit di ADIE e 1-bit di SREG sono settati. ADIF viene cancellato dall'hardware quando si sta esegue il corrispondente handling vector relativa all'interruzione.

BIT-3 - ADIE: ADC Interrupt Enable
Quando questo bit è scritto 1 e il bit uno in SREG è impostato, viene attivata l'interruzione riguardante il completamento della conversione.

IL REGISTRO ADCSRA - ADC Control and status Register B

-BIT 2:0 - ADTS2:0 - ADC Auto Trigger Source
 Se ADATE in ADCSRA è settato a 1, il valore di questi bit seleziona quale sorgente attiverà una conversione ADC.






