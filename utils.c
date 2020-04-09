/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	*bytes = paquete->buffer->size + sizeof(int) + sizeof(op_code);
	void* aEnviar = malloc(*bytes);
	int offset = 0;

	memcpy(aEnviar + offset, &paquete->codigo_operacion, sizeof(op_code));//copiando codigo de operacion
	offset += sizeof(op_code);
	printf("\nserializado: op = %d\n", paquete->codigo_operacion);
	printf("aenviar(1) = %d\n", *(int*)aEnviar);
	printf("offset = %d\n", offset);

	memcpy(aEnviar + offset, &paquete->buffer->size, sizeof(int));//copia tamaño del stream (del contenido)
	printf("\nserializado: size = %d\n", paquete->buffer->size);
	printf("aenviar(2) = %d\n", *(int*)(aEnviar+offset));
	offset += sizeof(int);//desplazamiento
	printf("offset = %d\n", offset);


	memcpy(aEnviar + offset, &paquete->buffer->stream, paquete->buffer->size);//copia el stream (el contenido)
	printf("\nserializado: stream = %s\n", (char*)paquete->buffer->stream);
	printf("aenviar(3) = %s\n", (char*)(aEnviar+offset));
	printf("serialización completada, no más offset\n\n");

	return aEnviar;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = MENSAJE;
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);
	printf("\nse empaqueto el stream: %s\n", (char*)paquete->buffer->stream);
	int bytes = 0;
	void* aEnviar = serializar_paquete(paquete, &bytes);
	send(socket_cliente, aEnviar, bytes, 0);
	printf("se enviaron %d bytes\n", bytes);

	// limpieza
	/*free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);*/
}

//TODO
char* recibir_mensaje(int socket_cliente)
{
	int codigo_operacion = 0;
	recv(socket_cliente, &codigo_operacion, sizeof(op_code), 0);
	int size;
	void* stream;
	char* string;
	switch (codigo_operacion) {
		case MENSAJE:
			printf("codigo de operación recibido: %d\n", codigo_operacion);
			recv(socket_cliente,&size, sizeof(int), 0);
			stream = malloc(size);
			string = malloc(size);
			printf("Se recibirá un string de %d bytes.\n", size);
			recv(socket_cliente,stream, size, 0);
			memcpy(string, stream, size);
			printf("se recibio el string: %s\nlongitud: %d\n", string, strlen(string));
			break;
		default:
			printf("Se recibio un codigo de operacion erroneo: codigo recibido:%d\n", codigo_operacion);
			break;
	}
	return string;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
