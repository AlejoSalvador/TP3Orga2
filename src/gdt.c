/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    
    [GDT_IDX_COD_0] = (gdt_entry){
        (unsigned short)    0xFFFF,         /* limit[0:15]  */ // 1,75 Gb =  1835008/4 Kb = 458752 Kb => Offset = 458751 = 0x6FFFF
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */ 
        (unsigned char)     0x01,           /* s            */ // En este caso es 'code'
        (unsigned char)     0x00,           /* dpl          */ // level 0
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x06,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */ 	
        (unsigned char)     0x01,           /* g            */ 
        (unsigned char)     0x00,           /* base[31:24]  */   	
    },

    [GDT_IDX_COD_3] = (gdt_entry){
        (unsigned short)    0xFFFF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */ //?
        (unsigned char)     0x01,           /* s            */ // En este caso es 'code'
        (unsigned char)     0x03,           /* dpl          */ // level 3    2b = 10?
        (unsigned char)     0x01,           /* p            */ //?
        (unsigned char)     0x06,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */ 	
        (unsigned char)     0x01,           /* g            */ 
        (unsigned char)     0x00,           /* base[31:24]  */   	
    },

    [GDT_IDX_DAT_0] = (gdt_entry){
        (unsigned short)    0xFFFF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */ 
        (unsigned char)     0x01,           /* s            */ // En este caso es 'dat'
        (unsigned char)     0x00,           /* dpl          */ // level 0
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x06,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */ 	
        (unsigned char)     0x01,           /* g            */ 
        (unsigned char)     0x00,           /* base[31:24]  */   	
    },

    [GDT_IDX_DAT_3] = (gdt_entry){
        (unsigned short)    0xFFFF,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */ 
        (unsigned char)     0x01,           /* s            */ // En este caso es 'dat'
        (unsigned char)     0x03,           /* dpl          */ 
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x06,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */ 	
        (unsigned char)     0x01,           /* g            */ 
        (unsigned char)     0x00,           /* base[31:24]  */   	
    },

    [GDT_IDX_VS] = (gdt_entry){
        (unsigned short)    0x000,          /* limit[0:15]  */ // 80 x 25 x 2 = 4000 B. 
        (unsigned short)    0x8000,         /* base[0:15]   */
        (unsigned char)     0x0B,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */ 
        (unsigned char)     0x01,           /* s            */ // En este caso es 'dat'
        (unsigned char)     0x00,           /* dpl          */ 
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00, 			/* db           */ 
        (unsigned char)     0x01,           /* g            */ // g = 1 Límite expresado en 4kb
        (unsigned char)     0x00,           /* base[31:24]  */   	
    }



};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};

void gdt_init_tss(){
    int i;
    // ENTRADAS EN LA GDT DE LAS TAREAS
    for (i = 0; i < 8; i++){
        gdt[i + GDT_OFFSET_NAVIOS] = (gdt_entry){
        (unsigned short)    0x0103,         /* limit[0:15]  */ 
        (unsigned short)    ((unsigned int) &tss_navios[i]) & 0x0000ffff,         /* base[0:15]   */
        (unsigned char)     (((unsigned int) &tss_navios[i]) & 0x00ff0000) >> 16,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */ //TYPE = 1 0 0 1 b
        (unsigned char)     0x00,           /* s            */ 
        (unsigned char)     0x03,           /* dpl          */ 
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */   
        (unsigned char)     0x00,           /* g            */ 
        (unsigned char)     (((unsigned int) &tss_navios[i]) & 0xff000000) >> 24           /* base[31:24]  */     
        };
    }

    // ENTRADAS EN LA GDT DE LAS BANDERAS
    for (i = 0; i < 8; i++){
        gdt[i + GDT_OFFSET_BANDERAS] = (gdt_entry){
        (unsigned short)    0x0103,         /* limit[0:15]  */   
        (unsigned short)    ((unsigned int) &tss_banderas[i]) & 0x0000ffff,         /* base[0:15]   */
        (unsigned char)     (((unsigned int) &tss_banderas[i]) & 0x00ff0000) >> 16,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */ //TYPE = 1 0 0 1 b
        (unsigned char)     0x00,           /* s            */ 
        (unsigned char)     0x03,           /* dpl          */ 
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */  
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (((unsigned int) &tss_navios[i]) & 0xff000000) >> 24           /* base[31:24]  */     
        };
    }

    // ENTRADA EN LA GDT DE LA TAREA IDLE
        gdt[GDT_IDX_TASK_IDLE] = (gdt_entry){
        (unsigned short)    0x0103,         /* limit[0:15]  */ 
        (unsigned short)    ((unsigned int) &tarea_idle) & 0x0000ffff,         /* base[0:15]   */
        (unsigned char)     (((unsigned int) &tarea_idle) & 0x00ff0000) >> 16,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */ //TYPE = 1 0 0 1 b
        (unsigned char)     0x00,           /* s            */ 
        (unsigned char)     0x00,           /* dpl          */ 
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */ 
        (unsigned char)     0x00,           /* g            */ 
        (unsigned char)     (((unsigned int) &tarea_idle) & 0xff000000) >> 24           /* base[31:24]  */  
        };

    // ENTRADA EN LA GDT DE LA TAREA INICIAL
        gdt[GDT_IDX_TASK_INIT] = (gdt_entry){
        (unsigned short)    0x0103,         /* limit[0:15]  */  
        (unsigned short)    ((unsigned int) &tarea_inicial) & 0x0000ffff,         /* base[0:15]   */
        (unsigned char)     (((unsigned int) &tarea_inicial) & 0x00ff0000) >> 16,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */ //TYPE = 1 0 0 1 b
        (unsigned char)     0x00,           /* s            */ 
        (unsigned char)     0x00,           /* dpl          */ 
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */   
        (unsigned char)     0x00,           /* g            */ 
        (unsigned char)     (((unsigned int) &tarea_inicial) & 0xff000000) >> 24           /* base[31:24]  */  
        };


}

