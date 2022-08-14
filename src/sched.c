/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

int contador_tareas;
unsigned int index_navio_actual;
unsigned int index_bandera_actual;
unsigned int navios[CANT_TAREAS];
unsigned int banderas[CANT_TAREAS];
unsigned short modo;	// 0 = modo tarea   --   1 = modo bandera


void sched_inicializar() {
	contador_tareas = 0;
	index_navio_actual = 0;
	index_bandera_actual = 0;

	int i;
	for (i = 0 ; i < CANT_TAREAS ; i++){
		navios[i] = SEG_TASK(i);
	}
	int j;
	for (j = 0 ; j < CANT_TAREAS ; j++){
		banderas[j] = SEG_FLAG(j);
	}

	modo = 0; // MODO TAREA
}


unsigned short atender_isr32(){

	print_estado_paginas();

	//CHEQUEO SI LA BANDERA EXCEDIÓ EL TICK, DE SER ASÍ DEBO DESALOJAR LA TAREA
	unsigned int tarea = rtr();
	if (es_bandera(tarea)){
		desalojar_tarea(tarea, "BANDERA EXCEEDED TICK");
		return 0;
	}
	



	unsigned short res;
	if(contador_tareas == 3)
	{
		modo = 1;
		index_bandera_actual = 0;
		contador_tareas = 0;	
	}

	if (modo == 0)
	{
		contador_tareas++;
		res = sched_proximo_indice();
		if(res != 0) print_reloj((res>>3)-GDT_OFFSET_NAVIOS);
		return res;
	} else
	{
		res = sched_proxima_bandera();
		if( res == 0)
		{
			tss_reinit_banderas();
			return atender_isr32();
		}else
		{
			print_bandera((res>>3)-GDT_OFFSET_BANDERAS);
			return res;
		}

	}
	return 0;
	
}

void atender_isr66(unsigned int buffer_bandera){
	unsigned int tarea = rtr();

	if(es_bandera(tarea)){
		//es bandera
		print_buffer_bandera((tarea>>3)-GDT_OFFSET_BANDERAS, buffer_bandera);		
		return;
	} else
	{
		//UNA TAREA NO PUEDE LLAMAR INT 0x66
		desalojar_tarea(tarea, "Task called INT 0x66");
		print_estado_desalojo(21);
		return;
	}
}

unsigned short sched_proximo_indice() {

	unsigned short res = 0;
	while(index_navio_actual < 8){
		res = navios[index_navio_actual];
		index_navio_actual++;
		if(res != 0){
			return res;
		}
	}
	if(index_navio_actual >= 8 && navios_activos() > 0)
	{
		index_navio_actual = 0;
		return sched_proximo_indice();
	}
	return res;
}


int navios_activos(){
	int res = 0;
	int i;
	for (i = 0; i < CANT_TAREAS ; i++){
		res = res || (navios[i] != 0);
	}
	return res;
}



unsigned int proximo_navio(){
	while(navios[index_navio_actual] == 0){
		index_navio_actual++;
	}
	return index_navio_actual;

	return 0;
}

// DEVUELVE EL SELECTOR DE SEGMENTO DE LA BANDERA A SALTAR o 0 en caso que no haya proxima
unsigned short sched_proxima_bandera() {
	//Imprimo banderas activas
	unsigned short res = 0;
	while (index_bandera_actual <= 7){
		res = banderas[index_bandera_actual];
		index_bandera_actual++;
		if(res != 0){
			return res;
		}
	}
	if(index_bandera_actual >= 8){
		modo = 0;
	}
	
	return res;
}

// SYSCALLS
void atender_isr50(unsigned int service, unsigned int dir1, unsigned int dir2){
	//DESALOJAR TAREA SI UNA BANDERA LLAMA Ox50
	unsigned int tarea = rtr();
	if (es_bandera(tarea))
		{
			print_estado_desalojo(20);
			desalojar_tarea(tarea, "Flag called INT 0x50");
			return;
		}


	if (service == SERVICE_FONDEAR)
	{
    	unsigned short tarea = rtr();
    	tarea = (tarea >> 3) - 23;
		unsigned int dir3_old = paginas_tarea[(3*tarea) + 2];

		unsigned int res = game_fondear(dir1);
		if ( res == FALSE )
		{
		  desalojar_tarea(rtr(), "Syscall Fondear returned FALSE");
		  print_estado_desalojo(22);
		}
		else{
			print_fondear(dir3_old);
		}
		return;
			
		
	} else if ( service == SERVICE_CANONEAR)
	{
		unsigned int res = game_canonear(dir1, dir2);
		if ( res == FALSE )
		{
		 	desalojar_tarea(rtr(), "Syscall Canonear returned FALSE");
		 	print_estado_desalojo(22);
		}
		else{
			print_canonear(dir1);
		}
		return;
		
	} else if ( service == SERVICE_NAVEGAR)
	{
    	unsigned short tarea = rtr();
    	tarea = (tarea >> 3) - 23;
    	unsigned int dir1_old = paginas_tarea[3*(tarea)];
		unsigned int dir2_old = paginas_tarea[(3*tarea) + 1];
		unsigned res = game_navegar(dir1, dir2);
		if ( res == FALSE ){
			desalojar_tarea(rtr(), "Syscall Navegar returned FALSE");
			print_estado_desalojo(22);
		} else
		{
			print_navegar(dir1, dir2, dir1_old, dir2_old); 
		}
		return;
	}
}


// INTERRUPCIÓN DE TECLADO // CORREGIR ADDR
void atender_isr33(char c){

	if (c == SCAN_CODE_M)
	{
		if (modo_pantalla == MODO_ESTADO)
		{
			// TENGO QUE CAMBIAR DE ESTADO
			modo_pantalla = MODO_MAPA;
			movmem((void*) VIDEO_SCREEN, (void*) BUFFER_VIDEO_MAPA, TAMANIO_SCREEN - TAMANIO_FILA);
		}
		// ELSE: NO TENGO QUE CAMBIAR DE ESTADO, NO HAGO NADA

	} else if (c == SCAN_CODE_E)
	{
		if (modo_pantalla == MODO_MAPA)
		{
			// TENGO QUE CAMBIAR DE ESTADO
			modo_pantalla = MODO_ESTADO;
			movmem((void*) VIDEO_SCREEN, (void*) BUFFER_VIDEO_ESTADO, TAMANIO_SCREEN - TAMANIO_FILA);
		}
		// ELSE: NO CAMBIO DE ESTADO
	} else if (c == 0x02 || c == 0x03 || c == 0x04 || c == 0x05 || c == 0x06 || c == 0x07 || c == 0x08 || c == 0x09 || c == 0x0a){

		/* ESTO COMENTADO: FUE REALIZADO PARA REALIZAR EL EJERCICIO, 
		NO APORTA FUNCIONALIDAD AL KERNEL +*/
		//print_hex(c-1, 1, 5, 5, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);

	} 
}

// DEVUELVE INDICE EN LA GDT DE LA TAREA ACTUAL
unsigned int ind_gdt_tarea_actual(){
	return rtr() >> 3;
}

// DEVUELVE EL NUMERO DE LA TAREA ACTUAL (de 1 a 8). SE DEBE UTILIZAR SOLO PARA LAS TAREAS, No para las Banderas
unsigned int tarea_actual(){
	return 0;

}


void desalojar_tarea(unsigned int tarea, const char* msg){

	int i = 0;
	while (tarea != navios[i] && tarea!= banderas[i] && i < CANT_TAREAS){
		i++;
	}
	
	if ( i < CANT_TAREAS )
	{
		// Desalojar
		print_desalojo_tarea(i, msg);
		navios[i] = 0;
		banderas[i] = 0;
	} else{
		//return FALSE;
	}

}


int es_bandera(unsigned int tarea){

	int i;

	for ( i = 0 ; i < CANT_TAREAS; i++){

		if (banderas[i] == tarea) return TRUE;
	}
	return FALSE;

}


// Atiende las excepciones y desaloja las tareas
void isr_exception(const char * msg){

	unsigned int tarea = rtr();

	desalojar_tarea(tarea, msg);
		
}




