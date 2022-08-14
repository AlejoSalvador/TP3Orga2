/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"


static char_screen (*pantalla)[VIDEO_COLS] = (char_screen (*) [VIDEO_COLS]) VIDEO_ADDR;
//static char_screen (*modo_mapa)[VIDEO_COLS] = (char_screen (*) [VIDEO_COLS]) BUFFER_VIDEO_MAPA;
//static char_screen (*modo_estado)[VIDEO_COLS] = (char_screen (*) [VIDEO_COLS]) BUFFER_VIDEO_ESTADO;

int cuadrante_negro(int i, int j);
int cuadrante_estados(int i);

int modo_pantalla;  // 0 = mapa ---- 1 = estado

unsigned int dir_ultimo_misil;

const char* mensajes_error[23] = {"Divide by zero",
                                  "Debug",
                                  "Non-maskable Interrupt",
                                  "Breakpoint",
                                  "Overflow",
                                  "Bound Range Exceeded",
                                  "Invalid Opcode",
                                  "Device not Available",
                                  "Double Fault",
                                  "Coprocessor Segment Overrun",
                                  "Invalid TSS",
                                  "Segment not present",
                                  "Stack-Segment Fault",
                                  "General Protection Fault",
                                  "Page Fault",
                                  "Reserved",
                                  "x87 Floating-Point Exception",
                                  "Alignment Check",
                                  "Machine Check",
                                  "SIMD Floating-Point Exception",
                                  "Flag called INT 0x50",
                                  "Task called INT 0x66",
                                  "Syscall returned FALSE"};



void screen_primer_linea(){
	int i;
	for (i = 0; i < VIDEO_COLS; i++) {
		char_screen temp = {.caracter = '$', .modo = C_FG_RED | C_BG_GREEN};
		pantalla[0][i] = temp;
	}
}


void screen_ej1(){
	//int i;
	int j;
	for (j = 0; j< VIDEO_COLS; j++){
		char_screen temp = {.caracter = '-', .modo = C_FG_WHITE| C_BG_BLACK};
		pantalla[0][j] = temp;	
	}
	int i;
	for (i = 1; i < VIDEO_FILS - 1; i++){	
		for (j = 0; j < VIDEO_COLS; j++) {
			char_screen temp = {.caracter = '.', .modo = C_FG_BLACK | C_BG_BLACK};
			pantalla[i][j] = temp;
		}
	}
	
	for (j = 0; j< VIDEO_COLS; j++){
		char_screen temp = {.caracter = '-', .modo = C_FG_WHITE| C_BG_BLACK};
		pantalla[24][j] = temp;	
	}

}

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr, unsigned int addr_buffer) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr, unsigned int addr_buffer) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr, unsigned int addr_buffer) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr, addr_buffer);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}


void print_mapa_clean(unsigned int addr_buffer){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;
    int i;
    int j;
    for (i = 0; i < 25 ; i++){
        for (j = 0; j < 80 ; j++){
            if (i < 3){
                p[i][j].c = 0;
                p[i][j].a = C_FG_BLACK | C_BG_GREEN;
            } else if ( i == 3 && j < 16){
                p[i][j].c = 0;
                p[i][j].a = C_FG_BLACK | C_BG_GREEN;
            } else if ( i < 24 ) {
                p[i][j].c = 0;
                p[i][j].a = C_FG_BLACK | C_BG_CYAN;
            }
            else {
                p[i][j].c = 0;
                p[i][j].a = C_FG_WHITE | C_BG_BLACK;
            }

        }

    }

}

void print_nombre_grupo(){
    const char* nombre_grupo = "Carlos Thays - Parque Centenario";
    print(nombre_grupo, 1, 0, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
}


void print_nombres_navios(){

ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_VIDEO_ESTADO;
	int i = 2;
	int j;
	int k = 0x31; 

	for ( j = 4 ; j < 45 ; j = j + 12){
		p[i][j].c = 'N';
		p[i][j+1].c = 'a';
		p[i][j+2].c = 'v';
		p[i][j+3].c = 161;
		p[i][j+4].c = 'o';
		p[i][j+5].c = ' ';
		p[i][j+6].c = k;
		k = k + 0x01;
	}
	i = i + 7;
	for ( j = 4 ; j < 45 ; j = j + 12){
		p[i][j].c = 'N';
		p[i][j+1].c = 'a';
		p[i][j+2].c = 'v';
		p[i][j+3].c = 161;
		p[i][j+4].c = 'o';
		p[i][j+5].c = ' ';
		p[i][j+6].c = k;
		k = k + 0x01;
	}
}

void print_estado_clean(unsigned int addr_buffer){
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;
    int i;
    int j;
    for (j = 0 ; j < 80 ; j++){
        p[0][j].c = 0;
        p[0][j].a = C_FG_WHITE | C_BG_BLACK;
    }
    for (i = 1 ; i < 24; i++){
        for ( j = 0 ; j < 80 ; j++){
            if (cuadrante_negro(i,j) == 1){
                p[i][j].a = C_FG_WHITE | C_BG_BLACK;
            } else if (cuadrante_estados(i) == 1) {
                if(j == 0 || j == 79){
                    p[i][j].a = C_FG_BLACK | C_BG_BLACK;
                } else if (j > 1) {
                    p[i][j].a = C_FG_WHITE | C_BG_BLUE;
                } else {
                    p[i][j].a = C_FG_BLACK | C_BG_LIGHT_GREY;    
                }
                
            } else{
                p[i][j].a = C_FG_BLACK | C_BG_LIGHT_GREY;                  
            }
            p[i][j].c = 0;
        }
    }
    for (j = 50; j < 78 ; j++){
        p[1][j].c = ' ';
        p[1][j].a = C_FG_WHITE | C_BG_RED;
    }



    print("EAX", 51, 2,  C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("EBX", 51, 3,  C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("ECX", 51, 4,  C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("EDX", 51, 5,  C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("ESI", 51, 6,  C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("EDI", 51, 7,  C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("EBP", 51, 8,  C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("ESP", 51, 9,  C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("EIP", 51, 10, C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("CR0", 51, 11, C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("CR2", 51, 12, C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("CR3", 51, 13, C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("CR4", 51, 14, C_FG_WHITE | C_BG_BLACK, addr_buffer);

    print("CS", 66, 2, C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("DS", 66, 3, C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("ES", 66, 4, C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("FS", 66, 5, C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("GS", 66, 6, C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("SS", 66, 7, C_FG_WHITE | C_BG_BLACK, addr_buffer);
    print("EFLAGS", 66, 9, C_FG_WHITE | C_BG_BLACK, addr_buffer);





    int k = 0x31;
    for ( i = 16; i < 24; i++){
    	p[i][1].c = k;
		p[i][1].a = C_BG_LIGHT_GREY | C_FG_BLACK;
        p[i][3].c = 'P';
        p[i][4].c = '1';
        p[i][5].c = ':';
        p[i][6].c = '0';
        p[i][7].c = 'x';
    	
        p[i][18].c = 'P';
        p[i][19].c = '2';
        p[i][20].c = ':';
        p[i][21].c = '0';
        p[i][22].c = 'x';
        

        p[i][33].c = 'P';
        p[i][34].c = '3';
        p[i][35].c = ':';
        p[i][36].c = '0';
        p[i][37].c = 'x';
         
        k++;
    }





    print_nombres_navios();

}

int cuadrante_estados(int i){
    if(i < 24 && i > 15) return 1;
    return 0;
}

int cuadrante_negro(int i, int j){
    if((49 < j) && (j < 78) && (1 < i) && (i < 15)) return 1;
    return 0;
}

void movmem(void* dst, void* src, unsigned int cant_bytes){

    char* source = (char*) src;
    char* dest   = (char*) dst;
    int i;
    for (i = 0; i < cant_bytes ; i++){
        dest[i] = source[i];
    }
}



void inicializar_pantalla(){

    modo_pantalla = 0;

    print_mapa_clean(BUFFER_VIDEO_MAPA);
        
    int i;
    for (i = 0; i < CANT_TAREAS ; i++){
        pintar_pagina_mapa(paginas_tarea[3*i], i, BUFFER_VIDEO_MAPA);
        pintar_pagina_mapa(paginas_tarea[3*i + 1], i, BUFFER_VIDEO_MAPA);
        pintar_pagina_mapa(paginas_tarea[3*i + 2], i, BUFFER_VIDEO_MAPA);
    }

    print_relojes_default(BUFFER_VIDEO_MAPA);
    //print_reloj_actual(0, BUFFER_VIDEO_MAPA);

    print_banderas_default(BUFFER_VIDEO_MAPA);

    dir_ultimo_misil = 0x0;
   
    movmem((void*) VIDEO_SCREEN, (void*) BUFFER_VIDEO_MAPA, 4000);

    //CARGO BUFFER ESTADO
    print_estado_clean(BUFFER_VIDEO_ESTADO);
    print_nombre_grupo();



}

// CUANDO HAY PAGINAS SOLAPADAS SE ESCRIBE "X"
// pagina : dir 32 bits   tarea: indice{1,..,8}
void pintar_pagina_mapa(unsigned int pagina, int tarea, unsigned int addr_buffer){

    unsigned int numero_pagina =  pagina /  0x1000;

    int fil = numero_pagina / VIDEO_COLS;
    int col = numero_pagina % VIDEO_COLS;

    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;

    if ( p[fil][col].c == 0 ){
        print_int(tarea + 1, col, fil, C_FG_WHITE | C_BG_RED , addr_buffer);

    } else{
        p[fil][col].c = 'X';

    }
     
}


void print_relojes_default(unsigned int addr_buffer){
    
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;

    int i;
    for (i = 0 ; i < CANT_TAREAS ; i ++){
        if (navios[i] != 0){
        //SOLO TAREAS VIVAS
        print_int(i + 1, (i*3) + 3, 24, C_FG_BLACK | C_BG_LIGHT_GREY, addr_buffer);
        p[24][(i*3) + 4].c = 42;
        p[24][(i*3) + 4].a = C_FG_BLACK | C_BG_LIGHT_GREY;
        }
        
    }
}


void print_reloj_actual(int tarea, unsigned int addr_buffer){

    print_relojes_default(addr_buffer);
    print_int(tarea + 1, (tarea*3) + 3, 24, C_FG_BLACK | C_BG_RED, addr_buffer);

    if (tarea > 7) return;
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;
    p[24][(tarea*3) + 4].c = 0;
    p[24][(tarea*3) + 4].a = C_FG_WHITE | C_BG_RED;

}

void print_reloj(int tarea){
    //LIMPIO BANDERAS PRIMERO
        print_banderas_default(VIDEO_SCREEN);
 
        print_relojes_default(VIDEO_SCREEN);
        print_reloj_actual(tarea, VIDEO_SCREEN);
    //if (modo_pantalla == MODO_MAPA){
    //}
    
    //print_relojes_default(BUFFER_VIDEO_MAPA);
    //print_reloj_actual(tarea, BUFFER_VIDEO_MAPA);

}

void print_bandera_actual(int bandera, unsigned int addr_buffer){


    print_banderas_default(addr_buffer);

    print_int(bandera+1, (bandera*3) + 3 + 35, 24, C_FG_BLACK | C_BG_RED, addr_buffer);

    if (bandera > 7) return;

    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;
    p[24][(bandera*3) + 4 + 35].c = 0;
    p[24][(bandera*3) + 4 + 35].a = C_FG_WHITE | C_BG_RED;
}

void print_bandera(int bandera){

    // LIMPIO RELOJES
    print_relojes_default(VIDEO_SCREEN);

    print_banderas_default(VIDEO_SCREEN);
    print_bandera_actual(bandera, VIDEO_SCREEN);
//    if (modo_pantalla == MODO_MAPA){
//    }
//    
//    print_banderas_default(BUFFER_VIDEO_MAPA);
//    print_bandera_actual(bandera, BUFFER_VIDEO_MAPA);
//
//    print_banderas_default(BUFFER_VIDEO_ESTADO);
//    print_bandera_actual(bandera, BUFFER_VIDEO_ESTADO);


}


void print_banderas_default(unsigned int addr_buffer){
    
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;

    int i;
    for (i = 0 ; i < CANT_TAREAS ; i ++){
        if(navios[i] != 0 )
        {
        print_int(i + 1, (i*3) + 3 + 35, 24, C_FG_BLACK | C_BG_LIGHT_GREY, addr_buffer);
        p[24][(i*3) + 4 + 35].c = 42;
        p[24][(i*3) + 4 + 35].a = C_FG_BLACK | C_BG_LIGHT_GREY;
        }
        
    }
}




void print_desalojo_tarea(int tarea, const char* msg){

    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    // PINTO TAREAS ULTIMA FILA
    p[24][(tarea*3) + 3].c = 0;
    p[24][(tarea*3) + 3].a = C_FG_WHITE | C_BG_BLACK;

    p[24][(tarea*3) + 4].c = 0;
    p[24][(tarea*3) + 4].a = C_FG_WHITE | C_BG_BLACK;


    p[24][(tarea*3) + 3 + 35].c = 0;
    p[24][(tarea*3) + 3 + 35].a = C_FG_WHITE | C_BG_BLACK;

    p[24][(tarea*3) + 4 + 35].c = 0;
    p[24][(tarea*3) + 4 + 35].a = C_FG_WHITE | C_BG_BLACK;



    // PINTO BANDERA DE ROJO
    print_bandera_desalojo(tarea);

    // PRINT MENSAJE
    print_exception(msg);

}

    
//unsigned int paginas_tarea[CANT_TAREAS*3];
//paginas_tarea[p1(tarea1), p2(tarea1), p3(tarea1), ......, p1(tarea8), p2(tarea8), p3(tarea8)]
void print_fondear(unsigned int dir3_old){

    unsigned short tarea = rtr();

    tarea = (tarea >> 3) - 23;

    unsigned int pagina = paginas_tarea[(3*tarea) + 2];
 
    if (modo_pantalla == MODO_MAPA){
        pintar_pagina_mapa(pagina, tarea, VIDEO_SCREEN);
        despintar_pagina_mapa(dir3_old, VIDEO_SCREEN);
    } 

    pintar_pagina_mapa(pagina, tarea, BUFFER_VIDEO_MAPA);
    despintar_pagina_mapa(dir3_old, BUFFER_VIDEO_MAPA);


}

void print_canonear(unsigned int dir_misil_nuevo){

    if (modo_pantalla == MODO_MAPA) print_dir_ultimo_misil(VIDEO_SCREEN, dir_misil_nuevo);

    //print_dir_ultimo_misil(BUFFER_VIDEO_MAPA, dir_misil_nuevo);
}

void print_dir_ultimo_misil(unsigned int addr_buffer, unsigned int dir_misil){
    
    unsigned int dir_misil_anterior = dir_ultimo_misil; (void) dir_misil_anterior;

    dir_ultimo_misil = dir_misil;

    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;



    if (dir_misil_anterior != 0)
    {
    // LIMPIO PRIMERO MISIL ANTERIOR
    unsigned int numero_pagina_ =  dir_misil_anterior /  0x1000;

    int fil_ = numero_pagina_ / VIDEO_COLS;
    int col_ = numero_pagina_ % VIDEO_COLS;



    p[fil_][col_].c = 0;
    p[fil_][col_].a = C_FG_WHITE | C_BG_CYAN;    
    }

    // PINTO MISIL NUEVO
    unsigned int numero_pagina =  dir_ultimo_misil /  0x1000;

    int fil = numero_pagina / VIDEO_COLS;
    int col = numero_pagina % VIDEO_COLS;

 
    p[fil][col].c = 219; // CARACTER CUADRADO
    p[fil][col].a = C_FG_LIGHT_BROWN | C_BG_BROWN;

}

void print_navegar(unsigned int dir1,
                   unsigned int dir2,
                   unsigned int dir1_old,
                   unsigned int dir2_old)
{
    unsigned short tarea = rtr();
    tarea = (tarea >> 3) - 23;
  
    despintar_pagina_mapa(dir1_old, BUFFER_VIDEO_MAPA);
    despintar_pagina_mapa(dir2_old, BUFFER_VIDEO_MAPA);

    pintar_pagina_mapa(dir1, tarea, BUFFER_VIDEO_MAPA);
    pintar_pagina_mapa(dir2, tarea, BUFFER_VIDEO_MAPA);


    if (modo_pantalla == MODO_MAPA){
        despintar_pagina_mapa(dir1_old, VIDEO_SCREEN);
        despintar_pagina_mapa(dir2_old, VIDEO_SCREEN);
        pintar_pagina_mapa(dir1, tarea, VIDEO_SCREEN);
        pintar_pagina_mapa(dir2, tarea, VIDEO_SCREEN);
    }
  
}


void despintar_pagina_mapa(unsigned int pagina, unsigned int addr_buffer){

    unsigned int numero_pagina =  pagina /  0x1000;

    int fil = numero_pagina / VIDEO_COLS;
    int col = numero_pagina % VIDEO_COLS;

    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;

    unsigned short tarea = rtr();
    tarea = (tarea >> 3) - 23;

    int cant_mapeadas = 0;
    int i;
    int tarea_mapeada = -1;
    if (p[fil][col].c == 'X')
    {
        // CHEQUEO SI HAY MÁS DE 2: TRUE=DEJO LA X, FALSE=DEJO EL NUMBERO DE LA OTRA TAREA
        for ( i = 0 ; i < CANT_TAREAS ; i++){
            if ( (i != tarea) && (
                 (paginas_tarea[3*i + 0] == pagina) ||
                 (paginas_tarea[3*i + 1] == pagina) ||
                 (paginas_tarea[3*i + 2] == pagina) )){
                cant_mapeadas++;
                tarea_mapeada = i;
            }
        }
        if (cant_mapeadas > 2){
            p[fil][col].c = 'X';
        } else{
            p[fil][col].c = tarea_mapeada + 0x31;
        }
        return;
    }
    if ((fil < 3) || (fil == 3 && col < 16))
    {
        // ES TIERRA
        p[fil][col].a = C_FG_WHITE | C_BG_GREEN;
        p[fil][col].c = 0;
    } else
    {
        // ES MAR
        p[fil][col].a = C_FG_WHITE | C_BG_CYAN;
        p[fil][col].c = 0;
    }

}


void print_buffer_bandera_aux(int bandera, unsigned int buffer, unsigned int addr_buffer){

    int i;
    int j;

    int k = (bandera >= 4);
    i = (7 * k) + 3;
    // banderas 0, 1, 2, 3: i= 3, banderas: 4, 5, 6, 7, i = 10
    int l = bandera % 4;
    j = (12 * l) + 2;

    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) addr_buffer;

    ca (*b)[50] = (ca (*)[50]) buffer;

    int m = 0;
    for ( l = 0 ; l < 10 ; l++)
        {
        for ( k = 0 ; k < 5 ; k++){
            p[i+k][j+l] = b[0][m];
        }
            m++;
    }
}

void print_buffer_bandera(int bandera, unsigned int buffer){

    if (modo_pantalla == MODO_ESTADO){
        print_buffer_bandera_aux(bandera, buffer, VIDEO_SCREEN);
    } 
    print_buffer_bandera_aux(bandera, buffer, BUFFER_VIDEO_ESTADO);

}

void print_bandera_desalojo(int bandera){

    int i;
    int j;

    int k = (bandera >= 4);
    i = (7 * k) + 3;
    // banderas 0, 1, 2, 3: i= 3, banderas: 4, 5, 6, 7, i = 10
    int l = bandera % 4;
    j = (12 * l) + 2;

    if (modo_pantalla == MODO_ESTADO)
    {
        ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
        for ( l = 0 ; l < 10 ; l++)
            {
            for ( k = 0 ; k < 5 ; k++){
                p[i+k][j+l].a = C_FG_WHITE | C_BG_RED;
                p[i+k][j+l].c = 0;
            }
        }        
        
    }
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_VIDEO_ESTADO;
    for ( l = 0 ; l < 10 ; l++)
        {
        for ( k = 0 ; k < 5 ; k++)
        {
            p[i+k][j+l].a = C_FG_WHITE | C_BG_RED;
            p[i+k][j+l].c = 0;
        }
    }
}


void print_exception(const char* msg){
	int tarea = rtr();
	if (es_bandera(tarea)){
		tarea = (tarea >> 3) - GDT_OFFSET_BANDERAS;
	}else{
		tarea = (tarea >> 3) - GDT_OFFSET_NAVIOS;
	}
	int i = 16;
	i = i + tarea;

	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_VIDEO_ESTADO;
	ca (*p2)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
	int j;
	for(j = 2 ; j < 79 ; j++)
	{
		if(modo_pantalla == MODO_ESTADO) p2[i][j].a = C_FG_WHITE | C_BG_RED;
		p[i][j].a = C_FG_WHITE | C_BG_RED;
	}	

	if (modo_pantalla == MODO_ESTADO) print(msg, 48, i, C_BG_RED | C_FG_WHITE, VIDEO_SCREEN);
	print(msg, 48, i, C_BG_RED | C_FG_WHITE, BUFFER_VIDEO_ESTADO);

}


void print_estado_paginas(){
    int tarea;
    int i = 16;
    
    if(modo_pantalla == MODO_ESTADO)
    {
        for (tarea = 0 ; tarea < CANT_TAREAS; tarea++)
        {   
            if ((navios[tarea] != 0)){
            print_hex(paginas_tarea[(3*tarea) + 0], 8, 8 , i+tarea, C_FG_WHITE | C_BG_BLUE, VIDEO_SCREEN);
            print_hex(paginas_tarea[(3*tarea) + 1], 8, 23, i+tarea, C_FG_WHITE | C_BG_BLUE, VIDEO_SCREEN);
            print_hex(paginas_tarea[(3*tarea) + 2], 8, 37, i+tarea, C_FG_WHITE | C_BG_BLUE, VIDEO_SCREEN);
            }
            
        }    
    }
        for (tarea = 0 ; tarea < CANT_TAREAS; tarea++)
    {   
        if ((navios[tarea] != 0)){
        print_hex(paginas_tarea[(3*tarea) + 0], 8, 8 , i+tarea, C_FG_WHITE | C_BG_BLUE, BUFFER_VIDEO_ESTADO);
        print_hex(paginas_tarea[(3*tarea) + 1], 8, 23, i+tarea, C_FG_WHITE | C_BG_BLUE, BUFFER_VIDEO_ESTADO);
        print_hex(paginas_tarea[(3*tarea) + 2], 8, 37, i+tarea, C_FG_WHITE | C_BG_BLUE, BUFFER_VIDEO_ESTADO);
        }
        
    }  

}
//(gs, fs, es, ds, cr0, cr2, cr3, cr4, edi, esi, ebp, ebx, edx, ecx, eax, ss, esp, eflags, cs, eip, error_code)
// gs, fs, es, ds, cr0, cr2, cr3, cr4, edi, esi, ebp, ebx, edx, ecx, eax, ss, esp, eflags, cs, eip, exception);
void print_estado_desalojo(int exception){
    
    int j;

    if (modo_pantalla == MODO_ESTADO){
      
        ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
        for (j = 50; j < 78 ; j++){
            p[1][j].c = ' ';
            p[1][j].a = C_FG_WHITE | C_BG_RED;
        }
        print_hex(eax_desalojo, 8, 55, 2, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(ebx_desalojo, 8, 55, 3, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(ecx_desalojo, 8, 55, 4, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(edx_desalojo, 8, 55, 5, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(esi_desalojo, 8, 55, 6, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(edi_desalojo, 8, 55, 7, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(ebp_desalojo, 8, 55, 8, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(esp_desalojo, 8, 55, 9, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(eip_desalojo, 8, 55, 10, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(cr0_desalojo, 8, 55, 11, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(cr2_desalojo, 8, 55, 12, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(cr3_desalojo, 8, 55, 13, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(cr4_desalojo, 8, 55, 14, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(cs_desalojo, 8, 69, 2, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(ds_desalojo, 8, 69, 3, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(es_desalojo, 8, 69, 4, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(fs_desalojo, 8, 69, 5, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(gs_desalojo, 8, 69, 6, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(ss_desalojo, 8, 69, 7, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print_hex(eflags_desalojo, 8, 69, 10, C_FG_WHITE | C_BG_BLACK, VIDEO_SCREEN);
        print(mensajes_error[exception], 50, 1, C_FG_WHITE | C_BG_RED, VIDEO_SCREEN);
        
    }

        ca (*p2)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) BUFFER_VIDEO_ESTADO;
        for (j = 50; j < 78 ; j++){
            p2[1][j].c = ' ';
            p2[1][j].a = C_FG_WHITE | C_BG_RED;
        }
        print_hex(eax_desalojo, 8, 55, 2, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(ebx_desalojo, 8, 55, 3, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(ecx_desalojo, 8, 55, 4, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(edx_desalojo, 8, 55, 5, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(esi_desalojo, 8, 55, 6, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(edi_desalojo, 8, 55, 7, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(ebp_desalojo, 8, 55, 8, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(esp_desalojo, 8, 55, 9, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(eip_desalojo, 8, 55, 10, C_FG_WHITE | C_BG_BLACK,  BUFFER_VIDEO_ESTADO);
        print_hex(cr0_desalojo, 8, 55, 11, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(cr2_desalojo, 8, 55, 12, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(cr3_desalojo, 8, 55, 13, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(cr4_desalojo, 8, 55, 14, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(cs_desalojo, 8, 69, 2, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(ds_desalojo, 8, 69, 3, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(es_desalojo, 8, 69, 4, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(fs_desalojo, 8, 69, 5, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(gs_desalojo, 8, 69, 6, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(ss_desalojo, 8, 69, 7, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        print_hex(eflags_desalojo, 8, 69, 10, C_FG_WHITE | C_BG_BLACK, BUFFER_VIDEO_ESTADO);
        
        print(mensajes_error[exception], 50, 1, C_FG_WHITE | C_BG_RED, BUFFER_VIDEO_ESTADO);
    

        unsigned int tarea = rtr();
    
        desalojar_tarea(tarea, mensajes_error[exception]);
     


      
}