/*This script is specifically designed for the STM32F042K6T6 MCU.*/

ENTRY(reset_handler)

MIN_HEAP_SIZE = 0x200;
MIN_STACK_SIZE = 0x400;

/*Source: https://www.st.com/resource/en/reference_manual/rm0091-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf*/
MEMORY
{
    FLASH (rx): ORIGIN = 0x08000000, LENGTH = 32K
    SRAM (rwx): ORIGIN = 0x20000000, LENGTH = 6K
}

/*Source: https://www.st.com/resource/en/programming_manual/pm0215-stm32f0-series-cortexm0-programming-manual-stmicroelectronics.pdf*/
SECTIONS {
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } > FLASH

    .text :
    {
        . = ALIGN(4);

        *(.text)
        *(.rodata)

        . = ALIGN(4);
        _etext = .;
    } > FLASH

    .data :
    {
        . = ALIGN(4);
        _sdata = .;

        *(.data)

        . = ALIGN(4);
        _edata = .;
    } > SRAM AT > FLASH

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;

        *(.bss)
        
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } > SRAM
}