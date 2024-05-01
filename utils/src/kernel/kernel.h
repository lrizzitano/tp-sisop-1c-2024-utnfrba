#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <sockets/sockets.h>
#include <pthread.h>
#include <commons/string.h>
#include <readline/readline.h>
#include <commons/collections/queue.h>

typedef enum{
	INICIAR_PROCESO,
	FINALIZAR_PROCESO=3,
	EJECUTAR_SCRIPT,
	DETENER_PLANIFICACION,
	INICIAR_PLANIFICACION,
	PROCESO_ESTADO=-7
}fConsola;

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
	char** instrucciones;
} t_pcb;

extern t_log* logger;
extern t_config* config;
extern t_queue* cProcesos;
extern int conexion_memoria;
extern int conexion_cpu;
extern int idPCB;


/**
*@fn 		inicializar
*@brief		Inicializa logger, config, (posiblemente agreguemos más)	
*/
void inicializar_kernel();

/**
*@fn 		finalizar
*@brief		Finaliza logger, config, y conexiones
*/
void finalizar_kernel();

/**
*@fn 		enviar_proceso
*@brief		Envia la dirreccion del de proceso a la memoria
*@param 	path path del proceso relativo a la memoria
*@return	puntero a la lista de instrucciones (aun no está programado)	
*/
void enviar_proceso(char*);


/**
*@fn 		syscall_IO_GEN
*@brief		realiza llamado a IO generica, la hace dormir y espera un mensaje de vuelta
*@param 	socket de IO
*@param     tiempo que queremos que duerma
*@return	nada
*/

void syscall_IO_GEN_SLEEP(int, char*);