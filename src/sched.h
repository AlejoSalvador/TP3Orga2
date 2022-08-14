/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"
#include "game.h"


void sched_inicializar();
unsigned short sched_proximo_indice();
unsigned short sched_proxima_bandera();
unsigned short atender_isr32();
void atender_isr33(char c);
void atender_isr50(unsigned int service, unsigned int dir1, unsigned int dir2);
void atender_isr66(unsigned int buffer_bandera);


unsigned int ind_gdt_tarea_actual();
void desalojar_tarea(unsigned int tarea, const char* msg);

unsigned int tarea_actual();

int es_bandera(unsigned int tarea);

void isr_exception(const char * msg);

int navios_activos();

unsigned int proximo_navio();


extern unsigned int navios[CANT_TAREAS];

extern int impresiones;

#endif	/* !__SCHED_H__ */
