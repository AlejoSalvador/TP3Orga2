	/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"


#define PAGE_DIRECTORY 0x27000
#define PAGE_TABLE_0 0x28000
//#define proxima_pagina_libre_tierra 0x30000
//#define proxima_pagina_libre_mar 0x100000
#define PDE_INDEX(virtual) (virtual >> 22) & 0x3FF
#define PTE_INDEX(virtual) (virtual >> 12) & 0x3FF //Me quedo con los primeros 10 bits
#define ALIGN(dir) (virtual & 0xFFFFF000)
#define PG_PRESENT 1
#define PG_READ_WRITE 2
//#define PG_USER 4
#define PTE_TO_INT(str_pte) (((((((((((unsigned int) (str_pte.p) | (unsigned int) (str_pte.rw << 1)) | (unsigned int) (str_pte.us << 2)) | (unsigned int) (str_pte.pwt << 3)) | (unsigned int) (str_pte.pcd << 4)) | (unsigned int) (str_pte.a << 5)) | (unsigned int) (str_pte.d << 6)) | (unsigned int) (str_pte.pat << 7)) | (unsigned int) (str_pte.g << 8)) | (unsigned int) (str_pte.disponible << 11)) | (unsigned int) (str_pte.dir_base << 12))

#define PDE_TO_INT(str_pde) (((((((((((unsigned int) (str_pde.p) | (unsigned int) (str_pde.rw << 1)) | (unsigned int) (str_pde.us << 2)) | (unsigned int) (str_pde.pwt << 3)) | (unsigned int) (str_pde.pcd << 4)) | (unsigned int) (str_pde.a << 5)) | (unsigned int) (str_pde.i << 6)) | (unsigned int) (str_pde.ps << 7)) | (unsigned int) (str_pde.g << 8)) | (unsigned int) (str_pde.disponible << 11)) | (unsigned int) (str_pde.dir_base << 12))





typedef struct str_page_directory_entry {
	unsigned char p:1;
	unsigned char rw:1;
	unsigned char us:1;
	unsigned char pwt:1;
	unsigned char pcd:1;
	unsigned char a:1;
	unsigned char i:1;		
	unsigned char ps:1;
	unsigned char g:1;
	unsigned char disponible:3;
	unsigned int dir_base:20;
} __attribute__((__packed__)) page_directory_entry;


typedef struct str_page_table_entry {
	unsigned char p:1;
	unsigned char rw:1;
	unsigned char us:1;
	unsigned char pwt:1;
	unsigned char pcd:1;
	unsigned char a:1;
	unsigned char d:1;		
	unsigned char pat:1;
	unsigned char g:1;
	unsigned char disponible:3;
	unsigned int dir_base:20;
} __attribute__((__packed__)) page_table_entry;



int proxima_pagina_libre_tierra();

int proxima_pagina_libre_mar();

void mmu_inicializar_dir_kernel();

//Devuelve cr3
unsigned int mmu_inicializar_dir_tarea(unsigned int task_number);


unsigned int mmu_inicializar_dir_tareas();

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
					   unsigned char disponible
					   );



void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3);

void mmu_identity_mapping(unsigned int cr3, unsigned int PDE_0, unsigned int PDE_1);

extern unsigned int paginas_tarea[CANT_TAREAS*3];

#endif	/* !__MMU_H__ */
