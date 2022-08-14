/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
#include "screen.h"


int pagina_libre_tierra = 0x30000;

int proxima_pagina_libre_tierra(){
	pagina_libre_tierra += 0x1000;
	return pagina_libre_tierra;
}

int pagina_libre_mar = 0x100000;

int proxima_pagina_libre_mar(){
	pagina_libre_mar += 0x1000;
	return pagina_libre_mar;
}

unsigned int paginas_tarea[CANT_TAREAS*3];
// ESTE ARRAY CONTENDRA LAS PAGINAS FISICAS A LAS CUALES ESTAN MAPEADAS LAS VIRTUALES
// paginas_tarea[p1(tarea1), p2(tarea1), p3(tarea1), ......, p1(tarea8), p2(tarea8), p3(tarea8)]

void mmu_inicializar_dir_kernel(){
	page_directory_entry* page_directory = (page_directory_entry*) 0x27000;
	page_directory[0].dir_base = 0x28000 >> 12;
	page_directory[0].p = 1;
	page_directory[0].rw = 1;
	page_directory[0].g = 0;
	page_directory[0].ps = 0;
	page_directory[0].i = 0;
	page_directory[0].a = 0;
	page_directory[0].pcd = 0;
	page_directory[0].pwt = 0;
	page_directory[0].us = 0;

	page_directory[1].dir_base = 0x30000 >> 12;
	page_directory[1].p = 1;
	page_directory[1].rw = 1;
	page_directory[1].g = 0;
	page_directory[1].ps = 0;
	page_directory[1].i = 0;
	page_directory[1].a = 0;
	page_directory[1].pcd = 0;
	page_directory[1].pwt = 0;
	page_directory[1].us = 0;

	int i;
	for (i = 2; i < 1024 ; i++){
		page_directory[i].dir_base = 0x00000;
		page_directory[i].p = 0;
		page_directory[i].rw = 0;
		page_directory[i].g = 0;
		page_directory[i].ps = 0;
		page_directory[i].i = 0;
		page_directory[i].a = 0;
		page_directory[i].pcd = 0;
		page_directory[i].pwt = 0;
		page_directory[i].us = 0;

	}


	page_table_entry* page_table_0 = (page_table_entry*) 0x28000;
	for (i=0 ; i < 1024 ; i++){
	 	page_table_0[i].dir_base = i;
	 	page_table_0[i].p = 1;
	 	page_table_0[i].rw = 1;
	 	page_table_0[i].us = 0;
	 	page_table_0[i].pwt = 0;
	 	page_table_0[i].pcd = 0;
	 	page_table_0[i].a = 0;
	 	page_table_0[i].d = 0;
	 	page_table_0[i].pat = 0;
	 	page_table_0[i].g = 0;
	 	page_table_0[i].disponible = 0;
	 } 


	page_table_entry* page_table_1 = (page_table_entry*) 0x30000;
	for (i=0 ; i < 896 ; i++){
	 	page_table_1[i].dir_base = ((i * 0x1000) >> 12) + 0x400;
	 	page_table_1[i].p = 1;
	 	page_table_1[i].rw = 1;
	 	page_table_1[i].us = 0;
	 	page_table_1[i].pwt = 0;
	 	page_table_1[i].pcd = 0;
	 	page_table_1[i].a = 0;
	 	page_table_1[i].d = 0;
	 	page_table_1[i].pat = 0;
	 	page_table_1[i].g = 0;
	 	page_table_1[i].disponible = 0;
	 }
	 	for (i=896 ; i < 1024 ; i++){
	 	page_table_1[i].dir_base = 0x000000;
	 	page_table_1[i].p = 0;
	 	page_table_1[i].rw = 0;
	 	page_table_1[i].us = 0;
	 	page_table_1[i].pwt = 0;
	 	page_table_1[i].pcd = 0;
	 	page_table_1[i].a = 0;
	 	page_table_1[i].d = 0;
	 	page_table_1[i].pat = 0;
	 	page_table_1[i].g = 0;
	 	page_table_1[i].disponible = 0;
	 }

}

void mmu_mapear_pagina(unsigned int virtual, 
					   unsigned int cr3, 
					   unsigned int fisica,
					   unsigned char p,
					   unsigned char rw,
					   unsigned char us,
					   unsigned char pwt,
					   unsigned char pcd,
					   unsigned char a,
					   unsigned char d,
					   unsigned char pat,
					   unsigned char g,
					   unsigned char disponible){
	virtual = ALIGN(virtual);
	page_directory_entry* page_directory = (page_directory_entry*) cr3;
	unsigned int pd_index = PDE_INDEX(virtual);
	unsigned int pt_index = PTE_INDEX(virtual);
	if(page_directory[pd_index].p == 1){
		// PDE_ENTRY esta presente
		unsigned int dir_base = page_directory[pd_index].dir_base;
		//print_hex(dir_base, 8, 5, 7, 0x17);
		dir_base = dir_base << 12;
		page_table_entry* page_table = (page_table_entry*) dir_base;
		page_table[pt_index].dir_base = (fisica >> 12);
	 	page_table[pt_index].p = p;
	 	page_table[pt_index].rw = rw;
	 	page_table[pt_index].us = us;
	 	page_table[pt_index].pwt = pwt;
	 	page_table[pt_index].pcd = pcd;
	 	page_table[pt_index].a = a;
	 	page_table[pt_index].d = d;
	 	page_table[pt_index].pat = pat;
	 	page_table[pt_index].g = g;
	 	page_table[pt_index].disponible = disponible;

	} else{
	// Tengo que crear la entrada y las tablas ??? REVISAR LO DE ABAJO
		page_directory[pd_index].p = p;
		page_directory[pd_index].rw = rw;
		page_directory[pd_index].g = 0;
		page_directory[pd_index].ps = 0;
		page_directory[pd_index].i = 0;
		page_directory[pd_index].a = a;
		page_directory[pd_index].pcd = pcd;
		page_directory[pd_index].pwt = pwt;
		page_directory[pd_index].us = us;
		unsigned int NEW_PDE = proxima_pagina_libre_tierra();
		page_directory[pd_index].dir_base = NEW_PDE >> 12;
		page_table_entry* new_page_table = (page_table_entry*) NEW_PDE;
		int i;
		for (i = 0; i < 1024 ; i++){
				new_page_table[i].dir_base = 0x00000;
				new_page_table[i].p = 0;
				new_page_table[i].rw = 0;
				new_page_table[i].us = 0;
				new_page_table[i].pwt = 0;
				new_page_table[i].pcd = 0;
				new_page_table[i].a = 0;
				new_page_table[i].d = 0;
				new_page_table[i].pat = 0;
				new_page_table[i].g = 0;
				new_page_table[i].disponible = 0;
		}
		new_page_table[pt_index].dir_base = (fisica >> 12);
		new_page_table[pt_index].p = p;
	 	new_page_table[pt_index].rw = rw;
	 	new_page_table[pt_index].us = us;
	 	new_page_table[pt_index].pwt = pwt;
	 	new_page_table[pt_index].pcd = pcd;
	 	new_page_table[pt_index].a = a;
	 	new_page_table[pt_index].d = d;
	 	new_page_table[pt_index].pat = pat;
	 	new_page_table[pt_index].g = g;
	 	new_page_table[pt_index].disponible = disponible;
	}

	tlbflush();
}

void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3){
	virtual = ALIGN(virtual);
	page_directory_entry* page_directory = (page_directory_entry*) cr3;
	unsigned int pd_index = PDE_INDEX(virtual);
	unsigned int pt_index = PTE_INDEX(virtual);

	if(page_directory[pd_index].p == 1){
		unsigned int dir_base = page_directory[pd_index].dir_base;
		page_table_entry* page_table = (page_table_entry*) (dir_base << 12);
		page_table[pt_index].dir_base = 0x000000;
	 	page_table[pt_index].p = 0;
	 	page_table[pt_index].rw = 0;
	 	page_table[pt_index].us = 0;
	 	page_table[pt_index].pwt = 0;
	 	page_table[pt_index].pcd = 0;
	 	page_table[pt_index].a = 0;
	 	page_table[pt_index].d = 0;
	 	page_table[pt_index].pat = 0;
	 	page_table[pt_index].g = 0;
	 	page_table[pt_index].disponible = 0;
	} else{
		breakpoint();
	}
	tlbflush();

}

int k = 15;

unsigned int mmu_inicializar_dir_tarea(unsigned int number_task)
{

	unsigned int task_code_tierra = TASK_SRC_CODE(number_task);

	unsigned int p_directory = proxima_pagina_libre_tierra();
	unsigned int pde0 = proxima_pagina_libre_tierra();
	unsigned int pde1 = proxima_pagina_libre_tierra();
	mmu_identity_mapping(p_directory, pde0, pde1);
	
	unsigned int task_stack_ring_0 = proxima_pagina_libre_tierra();
	(void) task_stack_ring_0;
	unsigned int bandera_stack_ring_0 = proxima_pagina_libre_tierra();
	(void) bandera_stack_ring_0;

	unsigned int task_code_p1_mar  = TASK_DST_CODE(number_task);
	unsigned int task_code_p2_mar  = task_code_p1_mar + 0x1000;
	

	//Ahora tengo que copiar el código de la tarea al mar
	char* src = (char*) task_code_tierra;
	char* dst = (char*) task_code_p1_mar;

	int i;
	for (i = 0; i < 0x2000; i++){
		dst[i] = src[i];
	}


	mmu_mapear_pagina(TASK_PAGE_1_VIRTUAL, p_directory, task_code_p1_mar, 1,1,1,0,0,0,0,0,0,0);
	mmu_mapear_pagina(TASK_PAGE_2_VIRTUAL, p_directory, task_code_p2_mar, 1,1,1,0,0,0,0,0,0,0); //modificar funcion para el nivel
	mmu_mapear_pagina(TASK_PAGE_3_ANCLA_VIRTUAL, p_directory, 0x0, 1,0,1,0,0,0,0,0,0,0);

	paginas_tarea[3*(number_task)] = task_code_p1_mar;
	paginas_tarea[(3*number_task) + 1] = task_code_p2_mar;
	paginas_tarea[(3*number_task) + 2] = 0x0;


	return p_directory;

}

unsigned int mmu_inicializar_dir_tareas(){

		unsigned int p_directory_tasks[8];

		p_directory_tasks[0] = mmu_inicializar_dir_tarea(TASK_1_CODE_SRC_ADDR);

		p_directory_tasks[1] = mmu_inicializar_dir_tarea(TASK_2_CODE_SRC_ADDR);

		p_directory_tasks[2] = mmu_inicializar_dir_tarea(TASK_3_CODE_SRC_ADDR);

		p_directory_tasks[3] = mmu_inicializar_dir_tarea(TASK_4_CODE_SRC_ADDR);

		p_directory_tasks[4] = mmu_inicializar_dir_tarea(TASK_5_CODE_SRC_ADDR);

		p_directory_tasks[5] = mmu_inicializar_dir_tarea(TASK_6_CODE_SRC_ADDR);

		p_directory_tasks[6] = mmu_inicializar_dir_tarea(TASK_7_CODE_SRC_ADDR);

		p_directory_tasks[7] = mmu_inicializar_dir_tarea(TASK_8_CODE_SRC_ADDR);

		//(void) p_directory_tasks;
		return p_directory_tasks[0];
}


void mmu_identity_mapping(unsigned int cr3, unsigned int PDE_0, unsigned int PDE_1){
	page_directory_entry* page_directory = (page_directory_entry*) cr3;
	page_directory[0].dir_base = PDE_0 >> 12;
	page_directory[0].p = 1;
	page_directory[0].rw = 1;
	page_directory[0].g = 0;
	page_directory[0].ps = 0;
	page_directory[0].i = 0;
	page_directory[0].a = 0;
	page_directory[0].pcd = 0;
	page_directory[0].pwt = 0;
	page_directory[0].us = 0;

	page_directory[1].dir_base = PDE_1 >> 12;
	page_directory[1].p = 1;
	page_directory[1].rw = 1;
	page_directory[1].g = 0;
	page_directory[1].ps = 0;
	page_directory[1].i = 0;
	page_directory[1].a = 0;
	page_directory[1].pcd = 0;
	page_directory[1].pwt = 0;
	page_directory[1].us = 0;

	int i;
	for (i = 2; i < 1024 ; i++){
		page_directory[i].dir_base = 0x00000;
		page_directory[i].p = 0;
		page_directory[i].rw = 0;
		page_directory[i].g = 0;
		page_directory[i].ps = 0;
		page_directory[i].i = 0;
		page_directory[i].a = 0;
		page_directory[i].pcd = 0;
		page_directory[i].pwt = 0;
		page_directory[i].us = 0;

	}


	page_table_entry* page_table_0 = (page_table_entry*) PDE_0;
	for (i=0 ; i < 1024 ; i++){
	 	page_table_0[i].dir_base = i;
	 	page_table_0[i].p = 1;
	 	page_table_0[i].rw = 1;
	 	page_table_0[i].us = 0;
	 	page_table_0[i].pwt = 0;
	 	page_table_0[i].pcd = 0;
	 	page_table_0[i].a = 0;
	 	page_table_0[i].d = 0;
	 	page_table_0[i].pat = 0;
	 	page_table_0[i].g = 0;
	 	page_table_0[i].disponible = 0;
	 } 


	page_table_entry* page_table_1 = (page_table_entry*) PDE_1;
	for (i=0 ; i < 896 ; i++){
	 	page_table_1[i].dir_base = ((i * 0x1000) >> 12) + 0x400;
	 	page_table_1[i].p = 1;
	 	page_table_1[i].rw = 1;
	 	page_table_1[i].us = 0;
	 	page_table_1[i].pwt = 0;
	 	page_table_1[i].pcd = 0;
	 	page_table_1[i].a = 0;
	 	page_table_1[i].d = 0;
	 	page_table_1[i].pat = 0;
	 	page_table_1[i].g = 0;
	 	page_table_1[i].disponible = 0;
	 }
	 	for (i=896 ; i < 1024 ; i++){
	 	page_table_1[i].dir_base = 0x000000;
	 	page_table_1[i].p = 0;
	 	page_table_1[i].rw = 0;
	 	page_table_1[i].us = 0;
	 	page_table_1[i].pwt = 0;
	 	page_table_1[i].pcd = 0;
	 	page_table_1[i].a = 0;
	 	page_table_1[i].d = 0;
	 	page_table_1[i].pat = 0;
	 	page_table_1[i].g = 0;
	 	page_table_1[i].disponible = 0;
	 }

}
