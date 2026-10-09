ENTRY(Reset_Handler)

MEMORY
{
    RAM(xrw)   :     ORIGIN = 0x20000000,   LENGTH = 10K
    FLASH(rx)  :     ORIGIN = 0x08000000,   LENGTH = 32K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);
_Min_Stack_Size = 0x400;
_Min_Heap_Size  = 0x200;

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.vector_table))
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
        end_of_rodata = .;
    } > FLASH

    .data :
    {
        . = ALIGN(4);
        start_of_data = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        end_of_data = .;
    } > RAM AT > FLASH
    data_in_flash = LOADADDR(.data);
    .bss :
    {
        . = ALIGN(4);
        start_of_bss = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        end_of_bss = .;
    } > RAM

    ._user_heap_stack :
    {
        . = ALIGN(8);
        _end = .;
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } > RAM
}
