.cpu cortex-m0
.thumb

.section .text
.global reset_handler
.global default_handler
.global main
.global isr_vector

.section .isr_vector, "a", %progbits
.type isr_vector, %object
isr_vector:
    .word 0x20000000 + 6 * 1024 @ Stack pointer initialization address.
    .word reset_handler
    .word default_handler
    .word default_handler
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word default_handler
    .word 0x00000000
    .word 0x00000000
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler

.section .text
.type default_handler, %function
default_handler:
    b reset_handler

.type copy_data, %function
copy_data:
    ldr R0, =_etext     @ R0 stores the address of the current byte in flash.
    ldr R1, =_sdata     @ R1 stores the address of the current byte in SRAM.

    ldr R2, =_edata
    ldr R3, =_sdata
    sub R2, R2, R3      @ R2 stores the size of data.

    ldr R3, =0          @ R3 stores the current index.
copy_data_loop:
    cmp R3, R2
    bge copy_data_loop_end

    mov R4, #0
    ldrb R4, [R0]        @ R4 contains the current byte.
    strb R4, [R1]

    add R0, R0, #1
    add R1, R1, #1
    add R3, R3, #1
    b copy_data_loop
copy_data_loop_end:
    bx LR

.type zero_out_bss, %function
zero_out_bss:
    ldr R0, =_sbss      @ R0 stores the current bss byte.
    ldr R1, =_ebss      @ R1 stores the end of the bss section.
zero_out_bss_loop:
    cmp R0, R1
    bge zero_out_bss_loop_end

    mov R2, #0
    strb R2, [R0]

    add R0, R0, #1
    b zero_out_bss_loop
zero_out_bss_loop_end:
    bx LR

.type reset_handler, %function
reset_handler:
    bl copy_data
    bl zero_out_bss
    bl main
