//korzystamy z kodow z labow TMP2

#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "motors.h"
#include "led.h"
#include "uart0.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CR	0xd		// Powr�t karetki
#define MOD_100Hz	6554		// MOD dla czestotliwosci 100Hz
#define ZEGAR 655360

//wartosci globalne
char rx_buf[8]; //tutaj jest wartosc odebrana
uint8_t rx_FULL=0;
uint8_t rx_buf_pos=0;
/////////////////

void UART0_IRQHandler()
{
	char temp;
	
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		temp=UART0->D;	// Odczyt wartosci z bufora odbiornika i skasowanie flagi RDRF
		if(!rx_FULL)
		{
			if(temp!=CR)
			{
				rx_buf[rx_buf_pos] = temp;	// Kompletuj komende
				rx_buf_pos++;
				if(rx_buf_pos==1)
					rx_FULL=1;		// ok ciag
			}
			
		}
	}
	NVIC_EnableIRQ(UART0_IRQn);	//wlaczenie przerwan
}

int main(){
	uint16_t	mod_curr=MOD_100Hz;	// Poczatkowy MOD dla f=100Hz	
	uint8_t choice=0;
	Motor_Init();
	Zumo_Led_Init();
	UART0_Init();
	
	TPM0->MOD = mod_curr;		// Poczatkowa wartosc MOD dla f=10Hz
	
	while(1){
		
		if(rx_FULL ==1){
			choice=atoi(rx_buf);
			//DELAY(2000)
			switch(choice)
				{
				case 1:
					LED(0);
					set_motor_L(70);		//0-10 stoi 100 - max
					set_motor_R(70);
					DELAY(1000)
					set_motor_L(0);
					set_motor_R(0);
					DELAY(500)
					
					set_motor_L(30); //obroty
					set_motor_R(-30);
					DELAY(1200)
					set_motor_L(-30);
					set_motor_R(30);
					DELAY(1200)
					set_motor_L(-30);
					set_motor_R(30);
					DELAY(1000)
					set_motor_L(30); //obroty
					set_motor_R(-30);
					DELAY(1200)
					set_motor_L(-70);
					set_motor_R(-70);
					DELAY(1000)
					set_motor_L(0);
					set_motor_R(0);
					DELAY(500)
					choice=0;
					break;
				
				case 2:
					LED(0);
					set_motor_L(40);
					set_motor_R(-40);
					DELAY(2000)
					set_motor_L(0);
					set_motor_R(0);
					DELAY(1000)
					set_motor_L(-40);
					set_motor_R(40);
					DELAY(2000)
					set_motor_L(0);
					set_motor_R(0);
					DELAY(1000)
					choice =0;
				break;
				
				case 3:
					LED(0);
					set_motor_L(25);
					set_motor_R(60);
					DELAY(2500)
					set_motor_L(-25);
					set_motor_R(-60);
					DELAY(3000)
					set_motor_L(0);
					set_motor_R(0);
					DELAY(500)
					set_motor_L(65);
					set_motor_R(30);
					DELAY(2500)
					set_motor_L(-65);
					set_motor_R(-30);
					DELAY(2500)
					set_motor_L(0);
					set_motor_R(0);
					DELAY(1000)
					choice=0;
				break;
				
				case 4:
					LED(0);
					set_motor_L(60);	//obrot
					set_motor_R(-60);
					DELAY(3000)
					set_motor_L(0);
					set_motor_R(0);
					DELAY(400)
				
					set_motor_L(100);		//do przodu
					set_motor_R(100);
					DELAY(500)
				
					set_motor_L(0);
					set_motor_R(0);
					DELAY(500)
				choice=0;
				break;
					
				default:
					__NOP();
				LED(1);
			}
			rx_buf_pos=0;
			rx_FULL=0;
		}
	}

}
