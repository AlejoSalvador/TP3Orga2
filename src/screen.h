/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__





/* Definicion de la pantalla */
#define VIDEO_FILS 25
#define VIDEO_COLS 80
#define VIDEO_ADDR 0xB8000
#define VIDEO_SCREEN 0xB8000

#define BUFFER_VIDEO_ESTADO		0x0002D000
#define BUFFER_VIDEO_MAPA		0x0002E000

#define TAMANIO_SCREEN 4000
#define TAMANIO_FILA 160


#define MODO_MAPA	0
#define MODO_ESTADO 1
#define SCAN_CODE_E		0x12
#define SCAN_CODE_M		0x32

#include "colors.h"
#include "i386.h"
#include "defines.h"
#include "mmu.h"
#include "sched.h"

typedef struct char_screen{
	unsigned char caracter;
    unsigned char modo;
} __attribute__((__packed__)) char_screen;

// void screen_pintar_pantalla();
void screen_primer_linea();
void screen_ej1();


/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr, unsigned int addr_buffer);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr, unsigned int addr_buffer);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr, unsigned int addr_buffer);

void print_mapa_clean(unsigned int addr_buffer);

void print_estado_clean(unsigned int addr_buffer);

void print_nombre_grupo();

void movmem(void* dst, void* src, unsigned int cant_bytes);

void inicializar_pantalla();

void pintar_pagina_mapa(unsigned int pagina, int tarea, unsigned int addr_buffer);

void print_relojes_default(unsigned int addr_buffer);

void print_banderas_default(unsigned int addr_buffer);

void print_reloj_actual(int tarea, unsigned int addr_buffer);

void print_reloj(int tarea);

void print_dir_ultimo_misil(unsigned int addr_buffer, unsigned int dir_misil);

void print_fondear(unsigned int dir3_old);

void print_canonear();

void print_navegar(unsigned int dir1, unsigned int dir2, unsigned int dir1_old, unsigned int dir2_old);

void despintar_pagina_mapa(unsigned int pagina, unsigned int addr_buffer);

void print_desalojo_tarea(int tarea, const char* msg);

void print_bandera_actual(int bandera, unsigned int addr_buffer);

void print_bandera(int bandera);


void print_buffer_bandera_aux(int bandera, unsigned int buffer, unsigned int addr_buffer);

void print_buffer_bandera(int bandera, unsigned int buffer);

void print_bandera_desalojo(int bandera);


void print_nombres_navios();

void print_exception(const char* msg);

void print_estado_paginas();

void print_estado_desalojo(int exception);

int eax_desalojo;
int ebx_desalojo;
int ecx_desalojo;
int edx_desalojo;
int esi_desalojo;
int edi_desalojo;
int ebp_desalojo;
int esp_desalojo;
int eip_desalojo;
int cr0_desalojo;
int cr2_desalojo;
int cr3_desalojo;
int cr4_desalojo;
int cs_desalojo;
int ds_desalojo;
int es_desalojo;
int fs_desalojo;
int gs_desalojo;
int ss_desalojo;
int eflags_desalojo;



extern int modo_pantalla;
extern unsigned int dir_ultimo_misil;
extern const char* mensajes_error[];





#endif  /* !__SCREEN_H__ */
