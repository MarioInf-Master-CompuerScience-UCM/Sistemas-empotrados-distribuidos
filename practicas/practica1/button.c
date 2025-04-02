/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "def.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>

/*--- variables globales ---*/
int symbol = 0;

/*--- declaracion de funciones ---*/
void Eint4567_ISR(void) __attribute__ ((interrupt ("IRQ")));
void Eint4567_init(void);
bool pressButton1_check();
bool pressButton2_check();
bool pressAnyButton_check();

/*--- funciones externas ---*/
extern void leds_switch ();
extern void D8Led_symbol(int value);
//extern void D8Led_Symbol(int value);


/*--- codigo de funciones ---*/
void button_init(){
	rPCONG = rPCONG & 0x0;
	return;
}

bool pressButton1_check(){
	int aux = rPDATG & 0b00000000000000000000000001000000;
	return (aux==0);
}

bool pressButton2_check(){
	int aux = rPDATG & 0b00000000000000000000000010000000;
	return (aux==0);
}

bool pressAnyButton_check(){
	if(pressButton1_check() || pressButton2_check()){
		return true;
	}else{
		return false;
	}
}


void Eint4567_init(void){
	//Configuracion controlador de interrupciones externas
	rEXTINTPND = ~(rEXTINTPND & 0x0); 			// Indicamos que todas las interrupciones externas han sido atendidas 	Borra EXTINTPND escribiendo 1s en el propio registro

	//Configuracion controlador de interrupciones internas
	rINTCON = 0x1;								// Configuramos los bits de control de las interrupciones internas	 	Habilita int. vectorizadas y la linea IRQ (FIQ no) mediante INTCON
	rINTMOD = rINTMOD & ~(0x11111<<21); 		// Configuramos el tipo de interrupciones	 							Configura las lineas como de tipo IRQ mediante INTMOD
	rI_ISPC = ~(rI_ISPC & 0x0);					// Indicamos que todas las interrupciones internas han sido atendidas 	Borra INTPND escribiendo 1s en I_ISPC
	rINTMSK = ~(0x1<<21);						// Habilitamos las interrupciones de las líneas que vamos a utilizar	Enmascara todas las lineas excepto Eint4567 y el bit global (INTMSK)

	//Establecer rutinas para las interrupciones
	pISR_EINT4567 = (unsigned) *Eint4567_ISR;	// Establecer la rutina de servicio para Eint4567

	//Configuracion del puerto G
	rPCONG = ~(rPCONG & 0x0); 					// Establece la funcion de los pines (EINT7-EINT0)
	rPUPG = rPUPG & 0x0;						// Habilita las resistencias de pull-up
	rEXTINT = 0x22222222;						// Configura las lineas de int. como de flanco de bajada mediante EXTINT

	//Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rEXTINTPND = ~(rEXTINTPND & 0x0);
	rI_ISPC = ~(rI_ISPC & 0x0);

	return;
}

/*COMENTAR PARA LA PARTE DEL 8-SEGMENTOS DESCOMENTAR PARA LA PRIMERA PARTE CON INTERRUPCIONES*/
void Eint4567_ISR(void){
	leds_switch();
	DelayMs(100);
	
	/*Atendemos interrupciones*/
	rEXTINTPND = ~(rEXTINTPND & 0x0);		//Borramos EXTINTPND ambas líneas EINT7 y EINT6
	rI_ISPC = ~(rI_ISPC & 0x0);				//Borramos INTPND usando ISPC
}

/*DESCOMENTAR PARA LA PARTE DEL 8-SEGMENTOS COMENTAR PARA LA PRIMERA PARTE CON INTERRUPCIONES
int which_int;
void Eint4567_ISR(void)
{
	/*Identificar la interrupcion*/
	//which_int = rEXTINTPND;
	/* Actualizar simbolo*/
	//switch (which_int) {
	//
	//}
	// muestra el simbolo en el display
	// espera 100ms para evitar rebotes
	
	// borra los bits en EXTINTPND  
	// borra el bit pendiente en INTPND
/*}
*/
