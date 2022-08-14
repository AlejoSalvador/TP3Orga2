/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_TAREAS             8

#define TAMANO_PAGINA           0x00001000

#define TASK_SIZE               2 * 4096

#define DEC_TO_HEX				

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_COD_0				18	
#define GDT_IDX_COD_3				19
#define GDT_IDX_DAT_0				20
#define GDT_IDX_DAT_3				21
#define GDT_IDX_VS					22 // Video segment

#define GDT_IDX_TASK_1				23
#define GDT_IDX_TASK_2				24
#define GDT_IDX_TASK_3				25
#define GDT_IDX_TASK_4				26				
#define GDT_IDX_TASK_5				27
#define GDT_IDX_TASK_6				28
#define GDT_IDX_TASK_7				29
#define GDT_IDX_TASK_8				30

#define GDT_IDX_TASK_IDLE			31
#define GDT_IDX_TASK_INIT			32

#define GDT_IDX_FLAG_1				33
#define GDT_IDX_FLAG_2				34
#define GDT_IDX_FLAG_3				35
#define GDT_IDX_FLAG_4				36
#define GDT_IDX_FLAG_5				37
#define GDT_IDX_FLAG_6				36
#define GDT_IDX_FLAG_7				37
#define GDT_IDX_FLAG_8				38

#define GDT_OFFSET_NAVIOS 23
#define GDT_OFFSET_BANDERAS 33

#define GDT_IDX_TASK(i)	(23 + i)
#define GDT_IDX_FLAG(i) (33 + i)


/* Selectores de segmentos */
/*----------------------------------------------------------------------------*/
#define SEG_COD_0 0x90 
#define SEG_DAT_0 0xA0
#define SEG_DAT_3 0xAB
#define SEG_COD_3 0x9B

#define SEG_TASKS_USER(index) (index << 3) | 0x03
#define SEG_FLAGS_SUSER(index) (index << 3) | 0x00

#define SEG_TASK(i) ((23 + i) << 3) | 0x03
#define SEG_FLAG(i) ((33 + i) << 3) | 0x00



/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */
#define CR3_KERNEL				0x00027000

#define BUFFER_VIDEO_ESTADO		0x0002D000
#define BUFFER_VIDEO_MAPA		0x0002E000

/* Direcciones virtuales de código, pila y datos */
/* -------------------------------------------------------------------------- */
#define TASK_CODE               0x40000000 /* direccion virtual del codigo */

#define TASK_IDLE_CODE          0x40000000 /* direccion virtual del codigo de la tarea idle */
#define TASK_IDLE_STACK         0x40001C00 /* direccion virtual de la pila de la tarea idle */
#define TASK_IDLE_STACK_RING_0  0x0002A000 /* direccion fisica de la pila de la tarea idle */

#define TASK_ANCLA              0x40002000
#define TASK_ANCLA_FIS          0x00000000

#define AREA_TIERRA_INICIO      0x00000000  /* 0.0 MB     */
#define AREA_TIERRA_FIN         0x000FFFFF  /* 1.0 MB - 1 */
#define AREA_MAR_INICIO         0x00100000  /* 1.0 MB     */
#define AREA_MAR_FIN            0x0077FFFF  /* 7.5 MB - 1 */


 /* --------- BANDERAS ------------- */
#define TASK_BANDERA_STACK_V	0x40001FFC



/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */
#define TASK_1_CODE_SRC_ADDR    0x00010000
#define TASK_2_CODE_SRC_ADDR    0x00012000
#define TASK_3_CODE_SRC_ADDR    0x00014000
#define TASK_4_CODE_SRC_ADDR    0x00016000
#define TASK_5_CODE_SRC_ADDR    0x00018000
#define TASK_6_CODE_SRC_ADDR    0x0001A000
#define TASK_7_CODE_SRC_ADDR    0x0001C000
#define TASK_8_CODE_SRC_ADDR    0x0001E000

#define TASK_SRC_CODE(i) (0x00010000 + (i * 0x0002000))	

#define TASK_1_CODE_DST_ADDR	0x00100000    
#define TASK_2_CODE_DST_ADDR	0x00150000
#define TASK_3_CODE_DST_ADDR	0x001A0000
#define TASK_4_CODE_DST_ADDR	0x001F0000
#define TASK_5_CODE_DST_ADDR	0x00240000
#define TASK_6_CODE_DST_ADDR	0x00290000
#define TASK_7_CODE_DST_ADDR	0x002E0000
#define TASK_8_CODE_DST_ADDR	0x00330000

#define TASK_DST_CODE(i) (0x00100000 + (i * 0x50000))



#define TASK_NAVIO_STACK_END		0x40001C00


#define TASK_IDLE_CODE_SRC_ADDR 0x00020000


#define TASK_PAGE_1_VIRTUAL 0x40000000
#define TASK_PAGE_2_VIRTUAL 0x40001000
#define TASK_PAGE_3_ANCLA_VIRTUAL 0x40002000



/* SYSCALLS */
/* -------------------------------------------------------------------------- */
#define SERVICE_FONDEAR 		0x923
#define SERVICE_CANONEAR		0x83A
#define SERVICE_NAVEGAR			0xAEF






#endif  /* !__DEFINES_H__ */
