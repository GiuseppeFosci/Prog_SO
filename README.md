OSCILLOSCOPIO SU ATM2650

L'oscilloscio connsiste nel campionamento di 8 canali su ADC. L'ADC è connesso a un multiplexer analogico
che permette 8/16 ingressi di tensione single-ended costruiti dai PIN della porta F e quelli della porta K.
L'ADC converte un voltaggio in input analogico in un valore digitale a 10BIT. Il minimo valore rappresenta 0V, ovvero GND, mentre il massimo rappresenta il voltaggio su AREF pin.
L'ADC viene attivato settando ADC Enable bit, ADEN in ADCSRA. IL risulato della conversione sarà un valore a 10-bit presente in ADC Data Register, ADCH and ADCL. Se non è richiesta una precisione maggiore di 8-bit è possibile leggere solamente il registro ADCH.

Una singola conversione inizia scrivendo un uno logico in ADC Start Conversion bit, ADSC ( Ovvero il BIT 6  dell' ADC Control e and status register ADCSRA). Questo bit rimane alto per tutto il tempo della conversione e sarà "pulito" dall'hardware quando la conversione è completata. Se sono selezionati differenti canali mentre la conversione è in corso, l'ADC finirà la conversione corrente prima di occuparsi degli altri.

IL REGISTRO ADCSRA - ADC Control and status Register A

BIT-7 - ADEN: ADC Enable
Scrivendo questo bit a 1 attiviamo ADC, mentre scrivendolo a 0 lo spegniamo. LO spegnimento mentre è in corso una cnversione farà terminare quella conversione.

BIT-6 - ADATE: ADC Start Conversion
Nella modalità a singola conversione, scrivendo questo bit a 1 faremo partire la conversione. 

BIT-4 - ADIF : ADC Interrupt Flag 
Questo Bit è settato quando la conversione è terminata e il Data Register si è aggiornato. L'interrupt è eseguito solo se il bit di ADIE e 1-bit di SREG sono settati. ADIF viene cancellato dall'hardware quando si sta esegue il corrispondente handling vector relativa all'interruzione.

BIT-3 - ADIE: ADC Interrupt Enable
Quando questo bit è scritto 1 e il bit uno in SREG è impostato, viene attivata l'interruzione riguardante il completamento della conversione.




