; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
%include "imprimir.mac"

global start


;; GDT
extern GDT_DESC
extern gdt_init_tss

;; IDT
extern IDT_DESC
extern idt_inicializar

;; PIC
extern resetear_pic
extern habilitar_pic
extern deshabilitar_pic

;; SCREEN
extern screen_primer_linea
extern screen_ej1
extern print
extern print_hex
extern print_int
extern print_mapa_clean
extern print_estado_clean
extern print_nombre_grupo
extern inicializar_pantalla

;; PAGINACION
extern mmu_inicializar_dir_kernel
extern mmu_mapear_pagina
extern mmu_unmapear_pagina
extern mmu_inicializar_dir_tarea
extern mmu_inicializar_dir_tareas

;; TSS
extern tss_init_idle
extern tss_init_tasks
extern tss_init_inicial

;; SCHED
extern sched_inicializar


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

msg_test db '-Mensaje de prueba-', 0
msg_test_len equ $ - msg_test

offset: dd 0x0
selector: dw 0x0

;;x
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli
    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0


    ; habilitar A20
    call habilitar_A20
    ; cargar la GDT
    lgdt [GDT_DESC]
    ; setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x90:modoprotegido; cs => code level 0 segment, index: 18
    ; pasar a modo protegido

BITS 32
    modoprotegido:
    ; acomodar los segmentos
    xor eax, eax
    mov ax, 0xA0    
    mov ds, ax      ; {index: 20 | gdt: 0 | rpl: 00} 20 = 0x14 => Agrego 3 ceros la final
    mov es, ax
    mov gs, ax
    mov ss, ax
    mov ax, 0xB0
    mov fs, ax      ; {index: 22 | gdt: 0 | rpl: 00}
    ; setear la pila
    mov esp, 0x27000
    mov ebp, 0x27000
    
    ; Mensaje bienvenida a Modo Protedigo
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 1, 0
    
    ; inicializar el manejador de memoria

    ; inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    ; inicializar memoria de tareas

    ; habilitar paginacion
    mov eax, 0x27000
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
  
    ; inicializar tarea idle
    call tss_init_inicial
    ;; inicializar todas las tsss y memoria de tareas
    call tss_init_idle
    call tss_init_tasks
    ;; inicializar entradas de la gdt de las tsss
    call gdt_init_tss     
    ; inicializar el scheduler
    call sched_inicializar
    ; inicializar pantalla
    call inicializar_pantalla

    ; inicializar la IDT
    lidt[IDT_DESC]
    call idt_inicializar


    ; configurar controlador de interrupciones
    call resetear_pic
    call habilitar_pic
    ; cargar la tarea inicial
    mov ax, 0x100
    ltr ax
    sti
    ; saltar a la primer tarea
    jmp 0xF8:0 ; TAREA IDLE


    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
