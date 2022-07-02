OSCILLOSCOPIO SU ATM2650

L'oscilloscio connsiste nel campionamento di 8 canali su ADC. L'ADC è connesso a un multiplexer analogico
che permette 8/16 ingressi di tensione single-ended costruiti dai PIN della porta F e quelli della porta K.
L'ADC converte un voltaggio in input analogico in un valore digitale a 10BIT. Il minimo valore rappresenta 0V, ovvero GND, mentre il massimo rappresenta il voltaggio su AREF pin.
L'ADC viene attivato settando ADC Enable bit, ADEN in ADCSRA. IL risulato della conversione sarà un valore a 10-bit presente in ADC Data Register, ADCH and ADCL. Se non è richiesta una precisione maggiore di 8-bit è possibile leggere solamente il registro ADCH.

Una singola conversione inizia scrivendo un uno logico in ADC Start Conversion bit, ADSC. Questo bit rimane alto per tutto il tempo della conversione e sarà "pulito" dall'hardware quando la conversione è completata. Se sono selezionati differenti canali mentre la conversione è in corso, l'ADC finirà la conversione corrente prima di occuparsi degli altri. E' sempre necessario prima di far partire una conversione elezionare il canale, la selezione del canale può essere c


