///poczatki projektu, pierwsze zamysły
///źródła: laboratoria TMP2

#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "TPM.h"
#include "tsi.h"
#include "uart0.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CR	0xd		// Powrót karetki
#define MOD_100Hz	6554		// MOD dla czestotliwosci 100Hz
#define ZEGAR 655360
uint8_t rx_buf_pos=0;
//char temp,buf;
uint8_t temp;
uint8_t rx_buf;
uint8_t rx_FULL=0;
uint8_t too_long=0;

void UART0_IRQHandler()
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		temp=UART0->D;	// Odczyt wartosci z bufora odbiornika i skasowanie flagi RDRF
		if(!rx_FULL)
		{
			if(temp!=CR)	//to mój pimpeczek napisal spytac sie go <3
			{	//LAB7 zadanie 2 spoko to tlumaczy
					rx_buf= temp;	// Zapisz liczbe podana z arduino
					rx_FULL =1;		//ta zmienna sprawi, ze w petli glownej zacznie sie akcja
			}
		}
	NVIC_EnableIRQ(UART0_IRQn);
	}
}


int main(){
	float freq;
	uint16_t	mod_curr=MOD_100Hz;	// Poczatkowy MOD dla f=100Hz
	uint8_t k_curr=50;					// Poczatkowy wspólczynnik wypelnienia K=50%, ustawiany przez pole dotykowe 0-100%
	uint16_t	ampl;
	uint8_t choice = 0;

	//uint8_t w=0; //do czytania slidera
	TSI_Init();				// Inicjalizacja pola dotykowego - Slider
	PWM_Init();				// Inicjalizacja licznika TPM0 (PWM „Low-true pulses”)
	
	TPM0->MOD = mod_curr;		// Poczatkowa wartosc MOD dla f=10Hz
	ampl=((int)mod_curr*k_curr)/100;	// Wspólczynnik wypelnienia k przeliczony dla aktualnego MOD - realna warotsc CnV
	TPM0->CONTROLS[3].CnV = 0; 	// Sygnal o nowym k podany na diode R LED //na poczatku dajemy 0
	TPM0->CONTROLS[2].CnV = 0; 	//PTB9 dioda zielona /na poczatku dajemy 0
	freq=(float)ZEGAR/(float)mod_curr;	// Wyliczenie czestotliwosci na podstawie wartosci zegara i rejestru MOD
	
	while(1)
	{
		//w=TSI_ReadSlider();
		switch(choice){
			case 1: //robot do przodu i do tylu
			k_curr=40;
			ampl=((int)mod_curr*k_curr)/100;
			TPM0->CONTROLS[3].CnV = ampl;	// Nowa wartosc kreujaca wspólczynnik wypelnienia PWM
			TPM0->CONTROLS[2].CnV = ampl;
			//_sleep_(10);
			
			//do tylu pin 7 i 8 dac na 1
			TPM0->CONTROLS[3].CnV = ampl;	// Nowa wartosc kreujaca wspólczynnik wypelnienia PWM
			TPM0->CONTROLS[2].CnV = ampl;
			//sleep(10);
			TPM0->CONTROLS[3].CnV = 0;	// Nowa wartosc kreujaca wspólczynnik wypelnienia PWM
			TPM0->CONTROLS[2].CnV = 0;
			//sleep(10);
			DELAY(1000) //sproboj
			__nop(1000); //spruboj
			
			break;
		}
		
			//k_curr=w;
			//ampl=((int)mod_curr*k_curr)/100;
			//TPM0->CONTROLS[3].CnV = ampl;	// Nowa wartosc kreujaca wspólczynnik wypelnienia PWM		
	}
	
}
