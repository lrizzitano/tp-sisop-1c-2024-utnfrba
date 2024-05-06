#include <stdint.h>

typedef enum{
	INICIAR_PROCESO,
	FINALIZAR_PROCESO=3,
	EJECUTAR_SCRIPT,
	DETENER_PLANIFICACION,
	INICIAR_PLANIFICACION,
	PROCESO_ESTADO=-7
}fConsola;

typedef enum{
	FINALIZACION=1,
	QUANTUM,
	RECURSO,
	IO
}codSalida;
typedef enum{
	NEW,
	READY,
	RUNNING,
	BLOCKED,
	FINISHED
}estados;

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
	int estado;
	char** instrucciones;
} t_pcb;