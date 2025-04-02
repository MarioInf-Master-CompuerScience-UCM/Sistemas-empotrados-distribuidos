/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "stdio.h"
#include "stdbool.h"

/*--- funciones externas ---*/
//extern void sys_init();
extern void leds_off();
extern void led1_on();
extern void leds_switch();
extern void Eint4567_init(void);
extern void button_init();
extern bool pressButton1_check();
extern bool pressButton2_check();
extern bool pressAnyButton_check();

/*--- declaracion de funciones ---*/


/*--- codigo de funciones ---*/
void Main(void){

	sys_init(); 	// Inicializacion de la placa, interrupciones y puertos
	button_init();	//Inicializamos los botones
	leds_off();		// Establecer valor inicial de los LEDs
	led1_on();
	Eint4567_init();

	//6.1 Hola Mundo Led
/*	while (1){
		leds_switch();
		DelayMs(1000);
	}
*/
	//6.2.1 Entrada/Salida programada
/*	while(1){
		if(pressButton1_check() || pressButton2_check() || pressAnyButton_check() ){
			leds_switch();
			DelayMs(100);
		}
	}
*/
	//6.2.2 Entrada/Salida con interrupciones
	while(1){

	}

	return;
}








