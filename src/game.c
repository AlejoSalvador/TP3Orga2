/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "screen.h"

unsigned int game_fondear(unsigned int ancla_dir_fisica) {

	if(ancla_dir_fisica > AREA_TIERRA_FIN ) return FALSE;

	unsigned int cr3 = rcr3();
 	
 	mmu_unmapear_pagina(TASK_PAGE_3_ANCLA_VIRTUAL, cr3);
 	
 	mmu_mapear_pagina(TASK_PAGE_3_ANCLA_VIRTUAL, cr3, ancla_dir_fisica, 1,0,1,0,0,0,0,0,0,0);

 	unsigned short tarea = (rtr() >> 3) -23;
	paginas_tarea[(3*tarea) + 2] = ancla_dir_fisica;

    return TRUE;
}

unsigned int game_canonear(unsigned int dir_misil_fisica, unsigned int dir_buffer_absoluta) {
  
	if (dir_misil_fisica < AREA_MAR_INICIO || dir_misil_fisica > AREA_MAR_FIN ) return FALSE;

	char* src = (char*) dir_buffer_absoluta;
	char* dst = (char*) dir_misil_fisica;

	
	int limite;
	if (97 > AREA_MAR_FIN - dir_misil_fisica){
		limite = AREA_MAR_FIN - dir_misil_fisica;
	}else{
		limite = 97;
	}
	
	int i;
	for (i = 0; i < limite; i++){
		dst[i] = src[i];
	}

    return TRUE;
}

unsigned int game_navegar(unsigned int dir_primera_pag_fisica, unsigned int dir_segunda_pag_fisica) {
    
    if (dir_primera_pag_fisica < AREA_MAR_INICIO || dir_primera_pag_fisica > AREA_MAR_FIN ) return FALSE;
    if (dir_segunda_pag_fisica < AREA_MAR_INICIO || dir_segunda_pag_fisica > AREA_MAR_FIN ) return FALSE;

    unsigned int cr3 = rcr3();
    unsigned int tarea_actual = rtr() >> 3;
    tarea_actual = tarea_actual - GDT_OFFSET_NAVIOS; // SOLO NAVIOS PUEDEN ACCEDER A ESTE SERVICIO

    

	unsigned int p1 = paginas_tarea[3*(tarea_actual)];     
	unsigned int p2 = paginas_tarea[(3*tarea_actual) + 1];
		

	char* src = (char*) p1;
	char* dst = (char*) dir_primera_pag_fisica;

	int i;
	for (i = 0; i < 0x1000; i++){
		dst[i] = src[i];
	}

	char* src2 = (char*) p2;
	char* dst2 = (char*) dir_segunda_pag_fisica;
	int j;
	for (j = 0; j < 0x1000; j++){
		dst2[j] = src2[j];
	}
	
	mmu_unmapear_pagina(TASK_PAGE_1_VIRTUAL, cr3);
 	mmu_unmapear_pagina(TASK_PAGE_2_VIRTUAL, cr3);
 	mmu_mapear_pagina(TASK_PAGE_1_VIRTUAL, cr3, dir_primera_pag_fisica, 1,1,1,0,0,0,0,0,0,0);
	mmu_mapear_pagina(TASK_PAGE_2_VIRTUAL, cr3, dir_segunda_pag_fisica, 1,1,1,0,0,0,0,0,0,0); //modificar funcion para el nivel
	

	paginas_tarea[3*(tarea_actual)] = dir_primera_pag_fisica;   
	paginas_tarea[(3*tarea_actual) + 1] = dir_segunda_pag_fisica;





    return TRUE;
}
