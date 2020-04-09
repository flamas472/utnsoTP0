/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();
	log_info(logger, "soy un log\n");//Loggear "soy un log"

	config = leer_config();
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	printf("\nConfiguraciones:\nIP = %s\nPUERTO = %s\n",ip, puerto);//Muestro la configuracion actual IP/puerto



	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion
	conexion = crear_conexion(ip, puerto);
	if(conexion==-1){
		printf("Hubo un error\n");
	} else {
		printf("Conexión exitosa: %d\n", conexion);
	}


	//enviar mensaje
	char* mensaje = malloc(32);
	printf("Ingrese su mensaje;\n-> ");
	scanf("%s", mensaje);
	//strcpy(mensaje, "prueba");
	printf("Usted ingreso: %s\n\n", mensaje);


	printf("Se va a enviar su mensaje.\n");
	enviar_mensaje(mensaje, conexion);
	printf("en teoria se envio el mensaje\n");


	//recibir mensaje
	char* mensajerecibido = recibir_mensaje(conexion);
	printf("en teoria se recibio el mensaje\n");
	//loguear mensaje recibido

	printf("\nse procedera a liberar la memoria antes de terminar el programa.\n");
	//terminar_programa(conexion, logger, config);
	printf("memoria liberada\n");

}//main

//TODO
t_log* iniciar_logger(void)
{

	t_log* unLogger = log_create("tp0.log", "Game-watch-client", true, LOG_LEVEL_INFO);

	return unLogger;
}

//TODO
t_config* leer_config(void)
{
	return config_create("tp0.config");
}

//TODO
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
}
