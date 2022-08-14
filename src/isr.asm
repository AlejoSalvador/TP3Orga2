; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32


;; PIC
extern fin_intr_pic1
extern atender_isr32
extern atender_isr33
extern atender_isr50
extern sched_proximo_indice
extern isr_exception
extern atender_isr66
extern print_estado_desalojo

extern eax_desalojo
extern ebx_desalojo
extern ecx_desalojo
extern edx_desalojo
extern esi_desalojo
extern edi_desalojo
extern ebp_desalojo
extern esp_desalojo
extern eip_desalojo
extern cr0_desalojo
extern cr2_desalojo
extern cr3_desalojo
extern cr4_desalojo
extern cs_desalojo
extern ds_desalojo
extern es_desalojo
extern fs_desalojo
extern gs_desalojo
extern ss_desalojo 
extern eflags_desalojo

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:
    ; Cuando el procesador salta a la excepción, el CS, EIP y el ESP se pushea en la pila del kernel, lo busco ahí
    ; Primero salvo el registro EAX
    push %1                 ; error code = [ESP]
    pushear_registros_desalojo
    call print_estado_desalojo
    jmp 0xF8:0 ; TAREA IDLE
    jmp $
    ; jamas se retorna
%endmacro

%macro pushear_registros_desalojo 0
    mov [eax_desalojo], eax
    mov [ebx_desalojo], ebx
    mov [ecx_desalojo], ecx
    mov [edx_desalojo], edx
    mov [esi_desalojo], esi    

    mov eax, [esp + 4]      ; eip
    mov ebx, [esp + 8]      ; cs
    mov ecx, [esp + 12]     ; flags
    mov edx, [esp + 16]     ; esp
    mov esi, [esp + 20]     ; ss

    mov [eip_desalojo], eax
    mov [cs_desalojo], ebx
    mov [eflags_desalojo], ecx
    mov [esp_desalojo], edx
    mov [ss_desalojo], esi
    ; Hasta acá parámetros en C: ( ss, esp, flags, cs, eip)
    mov [edi_desalojo], edi
    mov eax, cr4
    mov [cr4_desalojo], eax
    mov eax, cr3
    mov [cr3_desalojo], eax
    mov eax, cr2
    mov [cr2_desalojo], eax
    mov eax, cr0
    mov [cr0_desalojo], eax
    mov ax, ds
    mov [ds_desalojo], ax

    mov ax, es
    mov [es_desalojo], ax
    
    mov ax, fs
    mov [fs_desalojo], ax
    
    mov ax, gs
    mov [gs_desalojo], ax
    
    ; Hasta acá : (gs, fs, es, ds, cr0, cr2, cr3, cr4, edi, esi, ebp, ebx, edx, ecx, eax, ss, esp, flags, cs, eip, error_code)   
    
%endmacro

;;
;; Datoq
;; -------------------------------------------------------------------------- ;;
; Scheduler
reloj_numero:           dd 0x00000000
reloj:                  db '|/-\'

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2 
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
;ISR 13
;ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19


; Separo estas interrupciones porque tienen Error Code pusheado, preguntar como solucionar
global _isr13
_isr13:
    add esp, 4
    push 13
    pushear_registros_desalojo
    call print_estado_desalojo
    jmp 0xF8:0
    jmp $


global _isr14
_isr14:
    add esp, 4
    push 14
    pushear_registros_desalojo
    call print_estado_desalojo
    jmp 0xF8:0
    jmp $

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
offset: dd 0
selector: dw 0
_isr32:
    pushad

    call proximo_reloj

    xor eax, eax
    
    call atender_isr32
    cmp ax, 0
    je .nojump
        mov [selector], ax
        call fin_intr_pic1
        jmp far [offset] 
        jmp .end
    
    .nojump:
    call fin_intr_pic1
    
    .end:
    popad
iret
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33
_isr33:
pushad
call fin_intr_pic1
xor eax, eax
in al, 0x60
; SCAN CODES: m = 0x32, e = 0x12
push eax
call atender_isr33
pop eax
popad
iret
;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr0x50
_isr0x50:
pushad
add esp, 28
    push ecx
    push ebx
    push eax
    add esp, 12
    pushear_registros_desalojo
    sub esp, 12
    call atender_isr50
    pop eax
    pop ebx
    pop ecx
    jmp 0xF8:0 ; TAREA IDLE

sub esp, 28
popad
iret

global _isr0x66
_isr0x66:
pushad
add esp, 28
    push eax
    add esp, 4
    pushear_registros_desalojo
    sub esp, 4
    call fin_intr_pic1 
    call atender_isr66
    pop eax
    jmp 0xF8:0 ; TAREA DLE
    
popad
iret

;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
    pushad

    inc DWORD [reloj_numero]
    mov ebx, [reloj_numero]
    cmp ebx, 0x4
    jl .ok
        mov DWORD [reloj_numero], 0x0
        mov ebx, 0
    .ok:
        add ebx, reloj
        imprimir_texto_mp ebx, 1, 0x0f, 24, 79

    popad
    ret


