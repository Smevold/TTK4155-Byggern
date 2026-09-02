//Square test generator
#define F_CPU 3333333UL

#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>

void main(){
	DDRA |= (1 << PA0);

	//DDxn 1, PORTxn 0 || 1, idc
	PORTA |= (1 << PA0) ;
}
