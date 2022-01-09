#include "led.h"

void Zumo_Led_Init(void)
{	//zeby nie dublowac instrukcji to pominalem te,
	//ktore sa w motors init
	//SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; 
	PORTB->PCR[0] |= PORT_PCR_MUX(1); //port diody ALT1
	PTB->PDDR |= (1<<0); //ptb0 rola:wyjscie
	PTB->PDOR |= (1<<0); //zgas diode
}


void LED(int a)
{
	if(a==1) PTB->PDOR |= (1<<0); //gasi //chyba jednak to na odwrot jest 
		else	PTB->PDOR &= ~(1<<0); //swieci
}
