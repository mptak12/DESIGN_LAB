//korzystamy z kodow z labow TMP2

#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "motors.h"
#include "uart0.h" ////przerobic pozniej
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CR	0xd		// Powrót karetki
#define MOD_100Hz	6554		// MOD dla czestotliwosci 100Hz
#define ZEGAR 655360

int main(){
	uint16_t	mod_curr=MOD_100Hz;	// Poczatkowy MOD dla f=100Hz
	//uint8_t k_curr=200;					// Poczatkowy wspólczynnik wypelnienia ustawiony zeby stal
	//uint16_t	ampl;
	
	Motor_Init();
	
	TPM0->MOD = mod_curr;		// Poczatkowa wartosc MOD dla f=10Hz
	//ampl=((int)mod_curr*k_curr)/100;	// Wspólczynnik wypelnienia k przeliczony dla aktualnego MOD - realna warotsc CnV

	while(1){
			
			set_motor_L(80);		//0-10 stoi raczej 100 - max
			set_motor_R(80);
			DELAY(500)
			
			set_motor_L(0);
			set_motor_R(0);
			DELAY(500)
		
			set_motor_L(-50);
			set_motor_R(50);
			DELAY(500)
		
			set_motor_L(50);
			set_motor_R(-50);
			DELAY(1500)
	}

}
