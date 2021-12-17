//korzystamy z kodow z labow TMP2

#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "motors.h"
#include "led.h"
#include "uart0.h" ////przerobic pozniej
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CR	0xd		// Powrót karetki
#define MOD_100Hz	6554		// MOD dla czestotliwosci 100Hz
#define ZEGAR 655360

//wartosci globalne
uint8_t rx_buf; //tutaj jest wartosc odebrana
uint8_t rx_FULL=0;
/////////////////

void UART0_IRQHandler(void); //previous delcaraton warning nwm o co kaman
void UART0_IRQHandler()
{
	uint8_t temp;
	
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
	uint16_t	mod_curr=MOD_100Hz;	// Poczatkowy MOD dla f=100Hz	
	uint8_t choice=1;
	Motor_Init();
	Zumo_Led_Init();
	
	TPM0->MOD = mod_curr;		// Poczatkowa wartosc MOD dla f=10Hz
	
	while(1){
		
		if(rx_FULL ==1){	//przegladnac jak z zerowaniem RX_FULL - dalem na koncu
			choice = rx_buf;
		}
			DELAY(2000)
		
			switch(choice)
				{
				case 1:
					set_motor_L(70);		//0-10 stoi raczej 100 - max
					set_motor_R(70);
					DELAY(1000)
				
					set_motor_L(0);
					set_motor_R(0);
					DELAY(500)
		
					set_motor_L(-70);
					set_motor_R(-70);
					DELAY(1000)
					
					set_motor_L(0);
					set_motor_R(0);
					DELAY(3000)
					choice=2;
					break;
				
				case 2:
					set_motor_L(40);
					set_motor_R(-40);
					DELAY(3000)
					set_motor_L(0);
					set_motor_R(0);
					DELAY(1000)
					set_motor_L(-40);
					set_motor_R(40);
					DELAY(3000)
					set_motor_L(0);
					set_motor_R(0);
					DELAY(3000)
					choice =0;
				break;
				
				default:
					__NOP();
				LED(1);
				DELAY(500)
				LED(0);
			}
	rx_FULL=0;
	}

}
