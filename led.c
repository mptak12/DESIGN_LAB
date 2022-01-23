#include "led.h"

void Zumo_Led_Init(void)
{
	PORTB->PCR[0] |= PORT_PCR_MUX(1); //port diody ALT1
	PTB->PDDR |= (1<<0); //ptb0 rola:wyjscie
	PTB->PDOR |= (1<<0); //zgas diode
}


void LED(int a)
{
	if(a==1) PTB->PDOR |= (1<<0);	//swieci i gasi diode 
		else	PTB->PDOR &= ~(1<<0);
}
