#include "44b.h"
#include "iic.h"

extern uint32 mclk;			   // Frecuencia actual del reloj (Hz)

extern void DelayMs(int ms_time);

void iic_init( void )
{
	// Enable IIC & GPIO & BDMA blocks (si no se activa BDMA, el controlador IIC no es accesible)
	
	// bit 13 -> 1
	// bit 10 -> 1
	// bit 7  -> 1
	rCLKCON = rCLKCON | (0x1 << 13) | (0x1 << 10) | (0x1 << 7);
	
	
	// PF[1] = IICSDA, PF[0] = IICSCL
	
	// bits 2-3 -> 10
	// bits 0-1 -> 10
	rPCONF = rPCONF & ~(0x1) | (0x1 << 1) & ~(0x1 << 2) | (0x1 << 3);
		 
	// Pull-up enable
	rPUPF = rPUPF & ~(0x3);
	
	
    //Configurar la direcci�n del slave
	rIICADD = S3C44B0X_SLAVE_ADDRESS<<1;

    //Generaci�n ACK habilitada, interrupciones habilitadas
    
    // bit 5 -> 1
    // bit 7 -> 1
    rIICCON = rIICCON | (0x1 << 5) | (0x1 << 7);
	
    rIICCON = rIICCON | ((mclk / 4000000 - 1) & 0xf); 	//Valor de preescalado, PREESCALER = mclk/16/250000 -1
	
    // Activa Tx/Rx
	rIICCON = rIICCON | (0x1 << 4);
}

void iic_putByte_start( uint8 byte )
{
	// Escribe el dato
	// M�ster Tx, start condition, Tx/Rx habilitada
	// Comienza la transmisi�n (borrando pending bit del IICCON)
    // Espera la recepci�n de ACK  
}

void iic_putByte( uint8 byte )
{
    // Escribe el dato
	// Comienza la transmisi�n del dato (borrando pending bit del IICCON)
    // Espera la recepci�n de ACK  
};

void iic_putByte_stop( uint8 byte )
{
    // Escribe el dato
	// Copiar del anterior
	rIICDS = byte;

	// Comienza la trasmisi�n del dato (borrando pending bit del IICCON)
	rIICCON = rIICCON & ~(0x1 << 4);

	// Espera la recepci�n de ACK  
    // Copiar de los anteriores


    // M�ster Tx, stop condition, Tx/Rx habilitada
	// Copiar de los anteriores salvo en la stop condition

    // Comienza la trasmisi�n de STOP (borrando pending bit del IICCON)
	rIICCON = rIICCON & ~(0x1 << 4);

    // Espera a que la stop condition tenga efecto (5 ms para la at24c04)
	DelayMs(5);
}

void iic_getByte_start( uint8 byte )
{
	// Escribe el dato
	// Copiado de lo anterior

    // M�ster Rx, start condition, Tx/Rx habilitada
	// Copiado de lo anterior

    // Comienza la transmisi�n (borrando pending bit del IICCON)
	rIICCON = rIICCON & ~(0x1 << 4);

    // Espera la rececpi�n de ACK
}

uint8 iic_getByte( void )
{
    // Reanuda la recepci�n (borrando pending bit del IICCON)
	rIICCON = rIICCON & ~(0x1 << 4);

	// Espera la recepci�n del dato
    return rIICDS;// Lee el dato
}

uint8 iic_getByte_stop( int8 ack )
{
	uint8 byte;

    rIICCON = (rIICCON & ~(1 << 7)) | (ack << 7); // Habilita/deshabilita la generaci�n de ACK

    // Reanuda la recepci�n (borrando pending bit del IICCON)
	
	// Espera la recepci�n del dato
	
    byte = rIICDS;	// Lee el dato

   	// M�ster Rx, stop condition, Tx/Rx habilitada
   	// Comienza la trasmisi�n de STOP (borrando pending bit del IICCON)
   	// Espera a que la stop condition tenga efecto (5 ms para la at24c04)

	rIICCON |= (1<<7); // Habilita la generaci�n de ACK
   	return byte;
}
