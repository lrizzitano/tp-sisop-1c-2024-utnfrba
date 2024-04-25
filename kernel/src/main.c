#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <sockets/sockets.c>
#include <pthread.h>

typedef struct
{
	uint32_t PC;
	uint8_t AX;
	uint8_t BX;
	uint8_t CX;
	uint8_t DX;
	uint32_t EAX;
	uint32_t EBX;
	uint32_t ECX;
	uint32_t EDX;
	uint32_t SI;
	uint32_t DI;
} t_registros;

typedef struct 
{
	int pid;
	int pc;
	int quantum;
	t_registros registros;
	char* estado;
} t_pcb;

t_log* logger;
t_config* config;

void enviar_Proceso(int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = NUEVO_PROCESO;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen("instru.txt") + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, "instru.txt", paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}


int main(int argc, char* argv[]) {
    int conexion_memoria;
    int conexion_cpu;
	int kernel_servidor;
	int socket_IO;
	char* ip;
	char* puerto;
	pthread_t hilo_IO;

	// inicializamos logger y config
	logger = log_create("logKernel.log", "LOGS Kernel", 1, LOG_LEVEL_INFO);
	config = config_create("kernel.config");
    
	// buscamos datos en config y conectamos a memoria
	ip = buscar("IP_MEMORIA");
	puerto = buscar ("PUERTO_MEMORIA");
	conexion_memoria = crear_conexion(ip, puerto, "Memoria"); 
	enviar_mensaje("Saludos desde el Kernel",conexion_memoria);

	// buscamos datos en config y conectamos a cpu
	/*ip = buscar("IP_CPU");
	puerto = buscar("PUERTO_CPU_DISPATCH");
	conexion_cpu = crear_conexion(ip, puerto, "CPU");
	enviar_mensaje("Saludos desde el Kernel",conexion_cpu);

	//tambien sera servidor, con el I/O como cliente
	puerto = buscar("PUERTO_ESCUCHA");
	kernel_servidor = iniciar_servidor(puerto, "Kernel");

	socket_IO = esperar_cliente("I/O", kernel_servidor);
	pthread_create(&hilo_IO, NULL, interactuar, (void*)socket_IO);

	pthread_join(hilo_IO, NULL);
	*/
	enviar_Proceso(conexion_memoria);
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion_memoria);
	//liberar_conexion(conexion_cpu);
    return 0;
}
