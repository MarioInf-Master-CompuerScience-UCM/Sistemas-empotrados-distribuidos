#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

#include "utils/Utils.h"
#include "message/Message.h"
#define NUMCONEXIONES 5
#define PUERTOSERVER 17255
#define TAMANOMENSAJE 100

int s, sEfimero;
void signal_callback_handler(int signum);

int main(void){
	system("clear");
	Utils::printSuccess("Inicializando programa servidor.\n");
	
	signal(SIGINT, signal_callback_handler);	//Controlamos la interrupción Crt + C
	const string ID = "ServerID";

	struct sockaddr_in sin;
		memset ((char *)&sin, 0, sizeof(struct sockaddr_in));
		sin.sin_family=PF_INET;
		sin.sin_port=htons(PUERTOSERVER);
		sin.sin_addr.s_addr=htonl(INADDR_ANY);	
	struct sockaddr_in datosCliente;
		memset ((char *)&datosCliente, 0, sizeof(struct sockaddr_in));
	char buf[TAMANOMENSAJE];
	int retorno, numBytes;
	socklen_t lonDatosCliente;
	


	//Creamos el socket
	s=socket(PF_INET, SOCK_STREAM, 0);
	if(s==-1){
		Utils::printError("No pudo crearse el socket de comunicación. Finalizando programa.\n");
		exit(0);
	}
	
	//Asociamos el socket con los parámetros de dirección	
	retorno=bind(s, (struct sockaddr *)&sin, sizeof(sin));
	if(retorno==-1){
		Utils::printError("No pudiendo bindearse los parámetros del socket. Finalizando programa.\n");
		exit(0);
	}	

	//Abilitamos el socket para recibir peticiones
	retorno=listen(s, NUMCONEXIONES);
	if(retorno==-1){
		Utils::printError("No pudo relizarse la escucha de mensajes mediante el socket. Finalizando programa.\n");
		exit(0);
	}
	Utils::printSuccess("Programa servidor inicializado con exito.\n");


	Message *mess, *resp;
	while(true){

		while(true){

			//Aceptamos la siguiente petición en cola								
			lonDatosCliente=sizeof(struct sockaddr);									
			sEfimero=accept(s,(struct sockaddr *)&datosCliente, &lonDatosCliente);		
			if(sEfimero==-1){															
				Utils::printWarning("Un vehículo a intentado conectarse sin éxito.\n");				
			}else{
				Utils::printSuccess("Un nuevo vehículo se ha conectado con éxito.\n");
				break;
			}					
		}

		while(true){
			//Esperamos a recibir un mensaje del cliente									
			numBytes=recv(sEfimero, buf, TAMANOMENSAJE, 0);
			if(numBytes==0){
				Utils::printWarning("Un vehículo ha sido desconectado abruptamente.\n");
				break;
			}
			mess = new Message(buf);
			Utils::print(mess->getAllContent());

			if(mess->getType()==Message::TypeMessage::EMERGENCY){
				//Enviamos un ACK_EMERGECY
				resp = new Message(ID, Message::TypeMessage::ACK_EMERGENCY, "Emergecia registrada con exito");
				numBytes=send(sEfimero, resp, TAMANOMENSAJE, 0);
				Utils::print("Mensaje enviado..: (" + resp->getAllContent() + ")");
			}


		}
	}	


return 0;
}


// Función signal_callback_handler
//*******************************************************
void signal_callback_handler(int signum) {
	char in;
	int retorno;

	do{
		Utils::printEvent("Se ha pulsado Ctr + C. ¿Quiere cerrar el programa servidor? (s/n) >>");
		fflush(stdin);
		in = '\0';
		scanf(" %c",&in);

		if(in == 's'){
			Utils::printEvent("Sistema..: Cerrando el programa servidor.\n");
			break;

		}else if(in =='n'){
			Utils::printEvent("Sistema..: Cierre del programa servidor cancelado.\n");
			return;
		}

	}while(true);

	//Cerramos el socket
	retorno=close(s);
	if(retorno==-1){
		Utils::printError("No pudo cerrarse el socket de comunicación\n");
		exit(0);
	}

	Utils::printEvent("Programa servidor finalizado correctamente. Adios.\n");
	exit(signum);
}