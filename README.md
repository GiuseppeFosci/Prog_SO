
OSCILLOSCOPIO SU ATM2650

L'oscilloscio consiste nel campionamento di un canali su ADC. L'ADC è connesso a un multiplexer analogico che permette 8/16 ingressi di tensione costituiti dai PIN della porta F e quelli della porta K.
L'ADC converte un voltaggio in input analogico in un valore digitale a 10BIT. Il minimo valore rappresenta 0V, ovvero GND, mentre il massimo rappresenta il voltaggio su AREF pin.


Per far partire una singola conversione è necessario scrivere un uno logico in ADC Start Conversion bit, ADSC ( Ovvero il BIT 6  dell' ADC Control e and status register ADCSRA). 
Questo bit rimane alto per tutto il tempo che la conversione è in corso, e sarà successivamente cancellato dall'hardware quando la conversione è terminata. Se sono selezionati differenti canali mentre la conversione è in corso, l'ADC finirà la conversione corrente prima di occuparsi degli altri.

In alternativa, una conversione può essere triggherata automaticamente  da varie sorgenti. Per abilitare l'auto trigghering è necessario settare ADC Auto Trigger Enable bit. ovvero ADATE in ADCSRA. Le sorgente del trigger viene selezionata settando ADC Trigger Select bits, ADTS in ADCSRB, nel nostro caso si è selezionato il pin "A0".
Quando arriva un rising edge del clock, l'ADC prescaler è resettato e parte una nuova conversione. Questo fornisce un metodo per far partire conversioni ad intervalli prefissati.
Se il segnale di triggher è ancora settato quando la conversione è terminata, allora non può partire una nuova conversione. Se un altro rising edge arriva sul triggher signal durante la conversione, esso verrà ignorato

L'ADC viene attivato settando ADC Enable bit, ADEN in ADCSRA. IL risulato della conversione sarà un valore a 10-bit presente in ADC Data Register, ADCH and ADCL. Se non è richiesta una precisione maggiore di 8-bit è possibile leggere solamente il registro ADCH.


IL REGISTRO ADCSRA - ADC Control and status Register A

BIT-7 - ADEN: ADC Enable
Scrivendo questo bit a 1 attiviamo ADC, mentre scrivendolo a 0 lo spegniamo. LO spegnimento mentre è in corso una conversione farà terminare quella conversione.

BIT-6 - ADATE: ADC Start Conversion
Nella modalità a singola conversione, scrivendo questo bit a 1 faremo partire la conversione. 

BIT-5 – ADATE: Abilitazione del trigger automatico. Se impostato, l'ADC si avvierà automaticamente sul trigger selezionato.

BIT-4 - ADIF : ADC Interrupt Flag 
Questo Bit è settato quando la conversione è terminata e il Data Register si è aggiornato. L'interrupt è eseguito solo se il bit di ADIE e 1-bit di SREG sono settati.

ADIF viene cancellato dall'hardware quando è stato eseguito il corrispondente handling vector relativa all'interruzione, nel nostro caso l' Interrupt Service Routine sarà ISR ISR(ADC_vect).


BIT-3 - ADIE: ADC Interrupt Enable
Quando questo bit è scritto 1 e il bit uno in SREG è impostato, viene attivata l'interruzione riguardante il completamento della conversione.

Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits
Questi bit servono ad impostare la frequenza di conversione, in altre parole questi tre bit determinano il fattore di divisione tra la frequenza XTAL e l’input clock dell’ADC.




IL REGISTRO ADCSRB - ADC Control and status Register B


-BIT 7,5,4 Riservati, devono essere settati a 0 quando si scrive il registro

-BIT 6 – Analog Comprarator Multiplexer Enable – Non usato per le normali conversioni.

-BIT 3 – MUX 5 – Insieme a MUX4:0 in ADMUX, seleziona la combinazione di pin in input che sono collegati all’ADC.

-BIT 2:0 - ADTS2:0 - ADC Auto Trigger Source
Se ADATE in ADCSRA è settato a 1, il valore di questi bit seleziona quale sorgente attiverà una conversione ADC.
La conversione è triggherata dal rising edge dell’interrupt flag selezionato,se è impostato ADEN in ADCSRA, verrà avviata una conversione.
Imposteremo questi bit a “0”, in questo modo si setterà il Trigger Sourse a “Free Running Mode”.


IL REGISTRO ADMUX – ADC Multiplexer Selection Register


Bit 7:6 – REFS1:0: Reference Selection Bits
Questi bit selezionano il voltaggio di riferimento per l’ADC, nel nostro caso setteremo REFS1 a “0” e REFS0 a “1”.

Bit 5 – ADLAR: ADC Left Adjust Result
Usato per lo shift a sinistra del risulato, se si stabilisce che è sufficiente utilizzare una precisione a 8 Bit anziche 10 Bit che poi andremo a visualizzare in ADC  o in (ADCH/ADCL) in base a come si è impostato ADLAR.
In questo caso stabiliziamo che è sufficiente utlizzare una persione a 8Bit, quindi visualizzeremo il risultato solamente nella parte alta del registro ADC. 

Bits 4:0 – MUX4:0: Analog Channel and Gain Selection Bits

Questi valori sono utilizzati insieme al Bit MUX5 per stabilire quale/i sia/siano i bit analogici in input collegati all’ADC.


Funzionamento:
 
1) Scrivere il comando “./serial “ nella cartella del progetto.
2) Digitare un numero da 1 a 7, in base al numero inserito si setterà i Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits in ADCSRA, così facendo si imposterà il prescaler della conversione.
3) Una volta inerito il numero, verrà stampato a schermo il risultato della conversione e contemporanemante scritto in un file, chiamato “Misurazioni.txt”.
4) Per far terminare la conversione premetere CTRL+C, serial.c lo interpreterà come “0” finendo così la conversione.
5)serial.c terminerà, e imposterà il terminale già con GNUPLOT, a questo punto scrivere plot “Misutazioni.txt” w l, per fare il plot dei risultati presenti in Misurazioni.txt.







