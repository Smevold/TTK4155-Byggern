//Square test generator
#define F_CPU 3333333UL

#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>

#define PINTO 3

void main(){
	PORTA.DIR |= (1 << PINTO);
	while(1){
		//DDxn 1, PORTxn 0 || 1, idc
		PORTA.OUT ^=(1 << PINTO);

		_delay_ms(500);
	}
}
