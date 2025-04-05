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
#include <thread>
#include <pthread.h>
#include <future>
#include <pigpio.h>
#include <cstdio>
using namespace std;

#include "utils/Utils.h"
#include "message/Message.h"
#define NUMCONEXIONES 5
#define PUERTOCLIENTE 17255
#define TAMANOMENSAJE 100
#define RED_LED 17
#define BUTTON_PIN 18
#define PI_OUTPUT 1
#define PI_INPUT 0

int s;
bool flatLoop_HELLO;
void TH_sendMessage_emergecy(string ID);
void signal_callback_handler(int signum);
void SIGPIPE_callback_handler(int signum);


int main(void){
	system("clear");
	Utils::printSuccess("Sistema arrancado con éxito. Inicializando programa vehiculo.\n");

	gpioInitialise();
	gpioSetMode(RED_LED, PI_OUTPUT);
	gpioSetMode(BUTTON_PIN, PI_INPUT);
	signal(SIGINT, signal_callback_handler);	//Controlamos la interrupción Crt + C
	signal(SIGPIPE, SIGPIPE_callback_handler);	//Controlamos la ruptura de la tubería
	
	const string ID = "VehiculoID";

	struct sockaddr_in direcServer;
		memset ((char *)&direcServer, 0, sizeof(struct sockaddr_in));
		direcServer.sin_family=PF_INET;
		direcServer.sin_port=htons(PUERTOCLIENTE);
		direcServer.sin_addr.s_addr=inet_addr("192.168.1.178");
	char buf[TAMANOMENSAJE];
	int retorno, lonDirecServer, numBytes;


	//Creamos el socket
	s=socket(PF_INET, SOCK_STREAM, 0);
	if(s==-1){
		Utils::printError("No pudo crearse el socket de comunicación. Finalizando programa.\n");
		exit(0);
	}
	Utils::printSuccess("Programa inicializado con éxito.\n");

	
	while(true){

		lonDirecServer=sizeof (struct sockaddr);
		do{
			//Realizamos la petición de recibido
			retorno=connect(s, (struct sockaddr *)&direcServer, lonDirecServer);
			if(retorno==-1){
				Utils::printWarning("No pudo conectarse con el servidor. Reintentando...\n");
				sleep(5);
			}		
		}while(retorno == -1);
		Utils::printSuccess("Conexión con el servidor establecida con éxito.\n");


		Message *mess;
		flatLoop_HELLO = true;
		do{
			//Enviamos en mensaje por el socket									
			mess = new Message(ID, Message::TypeMessage::HELLO, "Esto es el mensaje periodico.");
			numBytes=send(s, mess->getAllContent().c_str(), TAMANOMENSAJE, 0);								
			Utils::print("Mensaje enviado..: (" + mess->getAllContent() + ")");
			sleep(2);

			
			//Comprovamos si se ha producido una emergencia y notificamos al servidor
			if(gpioRead(BUTTON_PIN)){
				std::future<void> fut = std::async(std::launch::async, TH_sendMessage_emergecy, ID);
			}
	
		}while(flatLoop_HELLO);
	
	}
	
return 0;
}



// Función signal_callback_handler
//*******************************************************
void TH_sendMessage_emergecy(string ID){

	Message *mess;
	mess = new Message(ID, Message::TypeMessage::EMERGENCY, "Emergencia producida. Un usuario ha accionado la emergencia");
	char buf[TAMANOMENSAJE];
	int numBytes = 0;

	numBytes=send(s, mess->getAllContent().c_str(), TAMANOMENSAJE, 0);								
	Utils::print("Mensaje enviado..: (" + mess->getAllContent() + ")");

	//Esperamos la respuesta del servidor								
	numBytes=recv(s, buf, TAMANOMENSAJE, 0);	
	Utils::print("Mensaje ACK recibido");
	
	return;
}





// Función SIGPIPE_callback_handler
//*******************************************************
void SIGPIPE_callback_handler(int signum){
	flatLoop_HELLO = false;
	Utils::printWarning("La conexión con el servidor se ha roto abruptamente.\n");
	return;
}



// Función signal_callback_handler
//*******************************************************
void signal_callback_handler(int signum) {
	char in;
	int retorno;
	bool flatLoop = true;

	do{
		Utils::printEvent("Se ha pulsado Ctr + C. ¿Quiere cerrar el programa vehículo? (s/n) >>");
		fflush(stdin);
		in = '\0';
		scanf(" %c",&in);

		if(in == 's'){
			Utils::printEvent("Cerrando el programa vehículo.\n");
			flatLoop = false;

		}else if(in =='n'){
			Utils::printEvent("Cierre del programa vehículo cancelado.\n");
			return;
		}

	}while(flatLoop);

	//Cerramos el socket
	retorno=close(s);
	if(retorno==-1){
		Utils::printError("No pudo cerrarse el socket de comunicación.\n");
		exit(0);
	}

	Utils::printEvent("Programa vehículo finalizado correctamente. Adios.\n");
	exit(signum);
}



