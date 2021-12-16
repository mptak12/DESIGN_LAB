#include "motors.h"
#include <stdlib.h>

#define MOD_100Hz	6554		// MOD dla czestotliwosci 100Hz

void Motor_Init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;      // Wlaczenie portu B
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;				//wlaczam port A

	// Wybór odpowiedniej roli pinu portu B ALT2
	PORTB->PCR[11] |= PORT_PCR_MUX(2);		// prawa gasienica 	TPM0_CH3
	PORTA->PCR[5] |= PORT_PCR_MUX(2); 	//lewa gasienica
	
	//rola portow ALT1
	PORTB->PCR[7] |=PORT_PCR_MUX(1);
	PORTB->PCR[10] |=PORT_PCR_MUX(1);
	
	// rola jako wyjscia:
	PTB->PDDR |= (1<<11);	
	PTA->PDDR |= (1<<5);
	//wyjscia(gasienice)
	PTB->PDDR |= (1<<7)|(1<<10);
	
	///////////konfiguracja licznika TPM0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;				// Dolaczenie sygnalu zegara do TPM0
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);			  // Wybierz zródlo taktowania TPMx MCGFLLCLK=41943040Hz
	
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;					//	TPM0 w trybie zliczanie "w przód"
	TPM0->SC |= TPM_SC_PS(6);								//	Dzielnik zegara wejsciowegoo równy 64; zegar=655360Hz
	TPM0->MOD = MOD_100Hz;													//	Rejestr MODULO=6553 - fwy=100Hz
	
	TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK;	//	TPM0, kanal 1 (PTB11) tryb "Edge-aligned PWM Low-true pulses (set Output on match, clear Output on reload)"
	TPM0->CONTROLS[0].CnV = 0x3334;					// Wsólczynnik wypelnienia poczatkowo full
	
	TPM0->CONTROLS[5].CnSC = TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK;	//	TPM0, kanal 5 (PTA5) tryb "Edge-aligned PWM Low-true pulses (set Output on match, clear Output on reload)"
	TPM0->CONTROLS[5].CnV = 0x3334;					// 
	
	TPM0->SC |= TPM_SC_CMOD(1);							// Wlacz licznik TPM0
}

void set_motor_R(int k_curr){ //k-curr im wieksze tym wolniej
	if(k_curr>=0) PTB->PDOR &= ~(1<<7); //kierunek
		else PTB->PDOR |=(1<<7);
	
	uint16_t	ampl;
	uint16_t	mod_curr=MOD_100Hz;
	ampl=((int)mod_curr*(100-abs(k_curr)))/100;
	TPM0->CONTROLS[0].CnV = ampl; //Nowa wartosc kreujaca wspólczynnik wypelnienia PWM
}

void set_motor_L(int k_curr){
	if(k_curr>=0) PTB->PDOR &= ~(1<<10); //kierunek
		else PTB->PDOR |=(1<<10);
	
	uint16_t	ampl;
	uint16_t	mod_curr=MOD_100Hz;
	ampl=((int)mod_curr*(100-abs(k_curr)))/100;
	TPM0->CONTROLS[5].CnV = ampl;
}
