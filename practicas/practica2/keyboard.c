/*--- Ficheros de cabecera ---*/
#include "44b.h"
#include "44blib.h"
#include "def.h"

/*--- Definición de macros ---*/
#define KEY_VALUE_MASK 0xF

/*--- Variables globales ---*/
volatile UCHAR *keyboard_base = (UCHAR *)0x06000000;
int key;
/* mapeo de botones con sus numeros 0-9 y simbolos */
int numberMapping[] = {7, 4, 1, 0,
					   8, 5, 2, -6,
					   9, 6, 3, -5,
					   -4, -3, -2, -1};
int digit;
int op1, op2;
int operation;
int state = 1;
int result;
int displayNumber;

/*--- Funciones externas ---*/
void D8Led_symbol(int value);
void D8Led_init();

/*--- Declaracion de funciones ---*/
void keyboard_init();
void KeyboardInt(void) __attribute__ ((interrupt ("IRQ")));
int key_read();

/*--- Codigo de las funciones ---*/
void keyboard_init()
{
	/* Configurar el puerto G (si no lo estuviese ya) */
		// Establece la funcion de los pines (EINT0-7)
	rPCONG = rPCONG | (0xF<<12);
		// Habilita el "pull up" del puerto
	rPUPG = rPUPG & ~(0x3<<6);
		// Configura las lineas de int. como de flanco de bajada mediante EXTINT
	rEXTINT = rEXTINT | (0x1<<29) | (0x1<<25);
	/* Establece la rutina de servicio para EINT1 */
	pISR_EINT1 = (unsigned) KeyboardInt;
	/* Configurar controlador de interrupciones */
		// Borra INTPND escribiendo 1s en I_ISPC
	rI_ISPC = rI_ISPC | BIT_EINT1;
		// Configura las lineas como de tipo IRQ mediante INTMOD
	rINTMOD = rINTMOD & ~BIT_EINT1;
	rINTCON = (rINTCON & ~(0x3<<1)) | 0x1;
		// Habilita int. vectorizadas y la linea IRQ (FIQ no) mediante INTCON
	/* Habilitar linea EINT1 */
	rINTMSK = rINTMSK & ~(BIT_EINT1 | BIT_GLOBAL);
		//
	/* Por precaucion, se vuelven a borrar los bits de INTPND correspondientes*/
	rI_ISPC = rI_ISPC | BIT_EINT1;

}

void KeyboardInt(void){
	/* Esperar trp mediante la funcion DelayMs()*/
	DelayMs(20);
	/* Identificar la tecla */
	key = key_read();

	if (key > -1){
		digit = numberMapping[key];

		// Se comprueba si es una cifra u otra cosa
		if (digit < 0){
			if (digit == -5){
				switch (operation){
					case -1: result = op1 + op2; break;
					case -2: result = op1 - op2; break;
					case -3: result = op1 * op2; break;
					// case -4: result = con cuidado;
					default: break;
				}

				// Llamar a la funcion que se encargue de mostrar el resultado por pantalla en 8led.c
				D8Led_symbol(result);
				state++;
				op1 = 0;
				op2 = 0;
				operation = 0;
			} else {
				// Se puede considerar la sequencia "+2" como "0+2" hasta que se cambie este comportamiento
				if (state > 2){
					// Mostrar de alguna manera que no se puede
					// e.g. leds parpadeando durante 2 segundos?
				} else {
					operation = digit;
					state++;
				}
			}
		} else {
			D8Led_symbol(digit);
			// Se modifica el operando correspondiente dependiendo del estado de la calculadora
			// Meter como en una pila y luego calcular el numero cuando se seleccione una operacion
			if (state == 1){
				op1 = op1 * 10 + digit;
			} else if (state == 2) {
				op2 = op2 * 10 + digit;
			}
		}
	}

	/* Esperar a se libere la tecla: consultar bit 1 del registro de datos del puerto G */
	while ((rPDATG & (0x1<<1)) == 0){
		//NOTHING
	}
	/* Esperar trd mediante la funcion Delay() */
	DelayMs(100);

	D8Led_init();
	/* Borrar interrupción de teclado */
	rI_ISPC = rI_ISPC | BIT_EINT1;
}

int key_read()
{
	int value= -1;
	char temp;
	// Identificar la tecla mediante ''scanning''
	// Si la identificación falla la función debe devolver -1
	// Usamos KEY_VALUE_MASK para quedarnos con los 4 bits menos significativos

	// 1a fila
	temp = *(keyboard_base + 0xfd) & KEY_VALUE_MASK;
	switch (temp) {
		case 0x7:  value = 0; break;
		case 0xB:  value = 4; break;
		case 0xD:  value = 8; break;
		case 0xE:  value = 12; break;
		default: break;
	}

	// 2a fila
	temp = *(keyboard_base + 0xfb) & KEY_VALUE_MASK;
	switch (temp) {
		case 0x7:  value = 1; break;
		case 0xB:  value = 5; break;
		case 0xD:  value = 9; break;
		case 0xE:  value = 13; break;
		default: break;
	}

	// 3a fila
	temp = *(keyboard_base + 0xf7) & KEY_VALUE_MASK;
	switch (temp) {
		case 0x7:  value = 2; break;
		case 0xB:  value = 6; break;
		case 0xD:  value = 10; break;
		case 0xE:  value = 14; break;
		default: break;
	}

	// 4a fila
	temp = *(keyboard_base + 0xef) & KEY_VALUE_MASK;
	switch (temp) {
		case 0x7:  value = 3; break;
		case 0xB:  value = 7; break;
		case 0xD:  value = 11; break;
		case 0xE:  value = 15; break;
		default: break;
	}

	return value;
}

void next_digit(){
	if(state == 3){
		displayNumber = result % 10;
		D8Led_symbol(displayNumber);
		result = (result - displayNumber) / 10;
		if (result == 0){
			state = 1;
		}
	}
}
