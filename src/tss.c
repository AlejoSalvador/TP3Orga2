/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tarea_inicial;
tss tarea_idle;

tss tss_navios[CANT_TAREAS];
tss tss_banderas[CANT_TAREAS];

void tss_inicializar() {
	// LLenar arreglo
	tss_init_idle();
	tss_init_inicial();
	tss_init_tasks();

}

// Esta función inicializa los tss de las tareas y las banderas
void tss_init_tasks(){
	int i;
	for (i = 0; i < CANT_TAREAS; i++){
		unsigned int cr3 = mmu_inicializar_dir_tarea(i);
		tss_init_navio(i, cr3); // FALTA VER QUE ONDA ESP0!!!!!!
		tss_init_bandera(i,cr3);
	}
}


void tss_reinit_banderas(){
	int i;
	for (i = 0; i < CANT_TAREAS; i++){
	tss_banderas[i].cs = SEG_COD_0; 	
	tss_banderas[i].es = SEG_DAT_0;   
	tss_banderas[i].gs = SEG_DAT_0;
	tss_banderas[i].ds = SEG_DAT_0;
	tss_banderas[i].ss = SEG_DAT_0;
	tss_banderas[i].fs = SEG_DAT_0;
	tss_banderas[i].eax = 0x0;
	tss_banderas[i].ecx = 0x0;
	tss_banderas[i].edx = 0x0;
	tss_banderas[i].ebx = 0x0;
	tss_banderas[i].esi = 0x0;
	tss_banderas[i].edi = 0x0; 
	tss_banderas[i].eip = TASK_SRC_CODE(i) + *((unsigned int*)(TASK_SRC_CODE(i) + 0x1FFC));
	//tss_banderas[i].eip = TASK_DST_CODE(i) + *((unsigned int*)(TASK_DST_CODE(i) + 0x1FFC));
	tss_banderas[i].esp = TASK_BANDERA_STACK_V;				// ESTA: 0x4001FFC 
	tss_banderas[i].ebp = TASK_BANDERA_STACK_V;				// ESTA: 0x4001FFC 
	tss_banderas[i].eflags = 0x202;
	tss_banderas[i].esp0 = (tss_banderas[i].cr3 + 0x3000) + 0x2000; 
	tss_banderas[i].ss0 = SEG_DAT_0;
	}
}


// Esta función inicializa el tss de una tarea
void tss_init_navio(int i, unsigned int cr3){
	tss_navios[i].cs = SEG_COD_3; 	
	tss_navios[i].es = SEG_DAT_3;   
	tss_navios[i].gs = SEG_DAT_3;
	tss_navios[i].ds = SEG_DAT_3;
	tss_navios[i].ss = SEG_DAT_3;
	tss_navios[i].fs = SEG_DAT_3;
	tss_navios[i].eax = 0x0;
	tss_navios[i].ecx = 0x0;
	tss_navios[i].edx = 0x0;
	tss_navios[i].ebx = 0x0;
	tss_navios[i].esi = 0x0;
	tss_navios[i].edi = 0x0;
	tss_navios[i].eip = 0x40000000; //
	tss_navios[i].esp = TASK_NAVIO_STACK_END;				// ESTA: 0x40001C00
	tss_navios[i].ebp = TASK_NAVIO_STACK_END;				// ESTA: 0x40001C00
	tss_navios[i].eflags = 0x202;
	tss_navios[i].cr3 = cr3; 
	tss_navios[i].esp0 = (cr3 + 0x3000) + 0x1000; //DEFINIDO EN MMU_INICIALIZAR_DIR_TAREA => 3 PAGINAS ABAJO DEL CR3 DE LA TAREA
	tss_navios[i].ss0 = SEG_DAT_0;	
	
	tss_navios[i].ptl      = 0x0;
 	tss_navios[i].unused0  = 0x0;
 	tss_navios[i].unused1  = 0x0;
 	tss_navios[i].esp1     = 0x0;
 	tss_navios[i].ss1      = 0x0;
 	tss_navios[i].unused2  = 0x0;
 	tss_navios[i].esp2     = 0x0;
 	tss_navios[i].ss2      = 0x0;
 	tss_navios[i].unused3  = 0x0;
 	tss_navios[i].unused4  = 0x0;
 	tss_navios[i].unused5  = 0x0;
 	tss_navios[i].unused6  = 0x0;
 	tss_navios[i].unused7  = 0x0;
 	tss_navios[i].unused8  = 0x0;
 	tss_navios[i].unused9  = 0x0;
 	tss_navios[i].ldt      = 0x0;
 	tss_navios[i].unused10 = 0x0;
 	tss_navios[i].dtrap    = 0x0;  
 	tss_navios[i].iomap    = 0xffff;  //?????

}

void tss_init_bandera(int i, unsigned int cr3){
	tss_banderas[i].cs = SEG_COD_0; 	
	tss_banderas[i].es = SEG_DAT_0;   
	tss_banderas[i].gs = SEG_DAT_0;
	tss_banderas[i].ds = SEG_DAT_0;
	tss_banderas[i].ss = SEG_DAT_0;
	tss_banderas[i].fs = SEG_DAT_0;
	tss_banderas[i].eax = 0x0;
	tss_banderas[i].ecx = 0x0;
	tss_banderas[i].edx = 0x0;
	tss_banderas[i].ebx = 0x0;
	tss_banderas[i].esi = 0x0;
	tss_banderas[i].edi = 0x0; 
	tss_banderas[i].eip = TASK_SRC_CODE(i) + *((unsigned int*)(TASK_SRC_CODE(i) + 0x1FFC));
	//tss_banderas[i].eip = TASK_DST_CODE(i) + *((unsigned int*)(TASK_DST_CODE(i) + 0x1FFC));
	tss_banderas[i].esp = TASK_BANDERA_STACK_V;				// ESTA: 0x4001FFC 
	tss_banderas[i].ebp = TASK_BANDERA_STACK_V;				// ESTA: 0x4001FFC 
	tss_banderas[i].eflags = 0x202;
	tss_banderas[i].cr3 = cr3; 
	tss_banderas[i].esp0 = (cr3 + 0x3000) + 0x2000; // Al pedo porqque no hay cambio de privilegio
	tss_banderas[i].ss0 = SEG_DAT_0;	

 	tss_banderas[i].ptl      = 0x0;
 	tss_banderas[i].unused0  = 0x0;
 	tss_banderas[i].unused1  = 0x0;
 	tss_banderas[i].esp1     = 0x0;
 	tss_banderas[i].ss1      = 0x0;
 	tss_banderas[i].unused2  = 0x0;
 	tss_banderas[i].esp2     = 0x0;
 	tss_banderas[i].ss2      = 0x0;
 	tss_banderas[i].unused3  = 0x0;
 	tss_banderas[i].unused4  = 0x0;
 	tss_banderas[i].unused5  = 0x0;
 	tss_banderas[i].unused6  = 0x0;
 	tss_banderas[i].unused7  = 0x0;
 	tss_banderas[i].unused8  = 0x0;
 	tss_banderas[i].unused9  = 0x0;
 	tss_banderas[i].ldt      = 0x0;
 	tss_banderas[i].unused10 = 0x0;
 	tss_banderas[i].dtrap    = 0x0;  
 	tss_banderas[i].iomap    = 0xffff; 

}



void tss_init_idle(){ //
	mmu_mapear_pagina(0x40000000, CR3_KERNEL, 0x00020000, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0);
	mmu_mapear_pagina(0x40001000, CR3_KERNEL, 0x00021000, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0);
	tarea_idle.cs = SEG_COD_0; 	
	tarea_idle.es = SEG_DAT_0;   
	tarea_idle.gs = SEG_DAT_0;
	tarea_idle.ds = SEG_DAT_0;
	tarea_idle.ss = SEG_DAT_0;
	tarea_idle.fs = SEG_DAT_0;
	tarea_idle.eax = 0x0;
	tarea_idle.ecx = 0x0;
	tarea_idle.edx = 0x0;
	tarea_idle.ebx = 0x0;
	tarea_idle.esi = 0x0;
	tarea_idle.edi = 0x0;
	tarea_idle.eip = 0x40000000; // O TIENE QUE IR ESTA: 0x20000 TAMBIEN?
	tarea_idle.esp = TASK_IDLE_STACK;				// ESTA: 0x40001C00 ES PARA LAS TAREAS QUE CORREN EN NIVEL 3 NO?
	tarea_idle.ebp = TASK_IDLE_STACK;				// ESTA: 0x40001C00 ES PARA LAS TAREAS QUE CORREN EN NIVEL 3 NO?
	tarea_idle.eflags = 0x202;
	tarea_idle.cr3 = CR3_KERNEL; // MISMO Cr3 QUE EL KERNEL 
	tarea_idle.esp0 = TASK_IDLE_STACK_RING_0;			

 	tarea_idle.ptl      = 0x0;
 	tarea_idle.unused0  = 0x0;
 	tarea_idle.unused1  = 0x0;
 	tarea_idle.esp1     = 0x0;
 	tarea_idle.ss1      = 0x0;
 	tarea_idle.unused2  = 0x0;
 	tarea_idle.esp2     = 0x0;
 	tarea_idle.ss2      = 0x0;
 	tarea_idle.unused3  = 0x0;
 	tarea_idle.unused4  = 0x0;
 	tarea_idle.unused5  = 0x0;
 	tarea_idle.unused6  = 0x0;
 	tarea_idle.unused7  = 0x0;
 	tarea_idle.unused8  = 0x0;
 	tarea_idle.unused9  = 0x0;
 	tarea_idle.ldt      = 0x0;
 	tarea_idle.unused10 = 0x0;
 	tarea_idle.dtrap    = 0x0;  
 	tarea_idle.iomap    = 0xffff;

}

// CAMBIARLO LUEGO POR BASURA
void tss_init_inicial(){
	tarea_inicial.cs = SEG_COD_0; 	
	tarea_inicial.es = SEG_DAT_0;   
	tarea_inicial.gs = SEG_DAT_0;
	tarea_inicial.ds = SEG_DAT_0;
	tarea_inicial.ss = SEG_DAT_0;
	tarea_inicial.fs = SEG_DAT_0;
	tarea_inicial.eax = 0x0;
	tarea_inicial.ecx = 0x0;
	tarea_inicial.edx = 0x0;
	tarea_inicial.ebx = 0x0;
	tarea_inicial.esi = 0x0;
	tarea_inicial.edi = 0x0;
	tarea_inicial.eip = 0x40000000; // O TIENE QUE IR ESTA: 0x20000 TAMBIEN?
	tarea_inicial.esp = TASK_IDLE_STACK;				// ESTA: 0x40001C00 ES PARA LAS TAREAS QUE CORREN EN NIVEL 3 NO?
	tarea_inicial.ebp = TASK_IDLE_STACK;				// ESTA: 0x40001C00 ES PARA LAS TAREAS QUE CORREN EN NIVEL 3 NO?
	tarea_inicial.eflags = 0x202;
	tarea_inicial.cr3 = CR3_KERNEL; // MISMO Cr3 QUE EL KERNEL 
	tarea_inicial.esp0 = TASK_IDLE_STACK_RING_0 + 0x1000;			//TIENE Q SER IGUAL QUE ESP?
	tarea_inicial.ss0 = SEG_DAT_0;	

 	tarea_inicial.ptl      = 0x0;
 	tarea_inicial.unused0  = 0x0;
 	tarea_inicial.unused1  = 0x0;
 	tarea_inicial.esp1     = 0x0;
 	tarea_inicial.ss1      = 0x0;
 	tarea_inicial.unused2  = 0x0;
 	tarea_inicial.esp2     = 0x0;
 	tarea_inicial.ss2      = 0x0;
 	tarea_inicial.unused3  = 0x0;
 	tarea_inicial.unused4  = 0x0;
 	tarea_inicial.unused5  = 0x0;
 	tarea_inicial.unused6  = 0x0;
 	tarea_inicial.unused7  = 0x0;
 	tarea_inicial.unused8  = 0x0;
 	tarea_inicial.unused9  = 0x0;
 	tarea_inicial.ldt      = 0x0;
 	tarea_inicial.unused10 = 0x0;
 	tarea_inicial.dtrap    = 0x0;  
 	tarea_inicial.iomap    = 0xffff;
}
