/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "stdio.h"
/*--- funciones externas ---*/
extern void leds_off();
extern void led1_on();
extern void timer_init();
extern void keyboard_init();

//Declarar funciones externas de inicialización

/*--- declaracion de funciones ---*/
void Main(void);

/*--- codigo de funciones ---*/
void Main(void){

	/* Inicializar controladores */
	sys_init(); 		// Inicializacion de la placa, interrupciones y puertos
	timer_init();		// Inicializacion del temporizador
	keyboard_init();	// Inicialización del teclado matricial

	/* Establecer valor inicial de los LEDs */
	leds_off();
	led1_on();
	while (1); // espera

	return;
}
