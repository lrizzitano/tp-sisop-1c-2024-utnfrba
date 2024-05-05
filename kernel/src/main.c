#include <kernel/kernel.h>

//Variables globales
t_log* logger;
t_config* config;
t_queue* cNEW;
t_queue* cREADY;
t_queue* cEXIT;
pthread_mutex_t mNEW;
pthread_mutex_t mREADY;
pthread_mutex_t mEXIT;
sem_t semPCP;
sem_t semPLP;
sem_t semEXIT;
sem_t sMultiprogramacion;
t_pcb generica;
int conexion_memoria;
int conexion_cpu;
int idPCB;
int multiprogramacion;
int quantum;

// de prueba, despues se borra.
void prueba_IO_GEN(int socket);

int main() {
	int kernel_servidor;
	int socket_IO;
	char* ip;
	char* puerto;
	pthread_t hilo_IO;
	pthread_t hilo_pcp;
	pthread_t hilo_carnicero;

	inicializar_kernel();
    
	// buscamos datos en config y conectamos a memoria
	ip = buscar("IP_MEMORIA");
	puerto = buscar ("PUERTO_MEMORIA");
	//conexion_memoria = crear_conexion(ip, puerto, "Memoria"); 
	//enviar_mensaje("Saludos desde el Kernel",conexion_memoria);

	// buscamos datos en config y conectamos a cpu
	ip = buscar("IP_CPU");
	puerto = buscar("PUERTO_CPU_DISPATCH");
	conexion_cpu = crear_conexion(ip, puerto, "CPU");

	//tambien sera servidor, con el I/O como cliente
	//puerto = buscar("PUERTO_ESCUCHA");
	//kernel_servidor = iniciar_servidor(puerto, "Kernel");

	//socket_IO = esperar_cliente("I/O", kernel_servidor);
	//pthread_create(&hilo_IO, NULL, prueba_IO_GEN, (void*)socket_IO);
	pthread_create(&hilo_pcp, NULL, PLP, NULL);
	pthread_create(&hilo_pcp, NULL, planificadorCP, NULL);
	pthread_create(&hilo_carnicero, NULL, carnicero, NULL);

	char* buffer;
	char** mensaje;
	int consola;
	while(1){
		buffer  = readline(">");
		mensaje = string_split(buffer, " ");
		consola = strcmp ("INICIAR_PROCESO", mensaje[0]);
		switch (consola){
			case INICIAR_PROCESO:
				crear_proceso (mensaje[1]);
				break;
			case FINALIZAR_PROCESO:
				log_info(logger, "Proceso finalizado");
				break;
			case EJECUTAR_SCRIPT:
				log_info(logger, "Script ejecutado");
				break;
			case DETENER_PLANIFICACION:
				log_info(logger, "Planificacion detenida");
				break;
			case INICIAR_PLANIFICACION:
				log_info(logger, "Planificacion iniciada");
				break;
			case PROCESO_ESTADO:
				log_info(logger, "El estado del proceso es:");
				break;
			default:
				log_info(logger, "Código invalido");
				break;
		}
	}
	pthread_join(hilo_IO, NULL);
	finalizar_kernel();

	
    return 0;
}

void prueba_IO_GEN(int socket) {
	for (int i = 0; i<5; i++) {
		syscall_IO_GEN_SLEEP(socket, "10");
		sleep(15);
	}
}