MEMORY
{
    DDR_MEM(rwx): ORIGIN = @WOLFBOOT_LOAD_BASE@, LENGTH = 0x3c000000
}
ENTRY(_vector_table);

SECTIONS
{
    .text :
    {
        _start_text = .;
        KEEP(*(.boot*))
        *(.text*)
        *(.rodata*)
        *(.note.*)
        . = ALIGN(4);
        _end_text = .;
    } > DDR_MEM
    .edidx :
    {
        . = ALIGN(4);
        *(.ARM.exidx*)
    } > DDR_MEM

    .edidx :
    {
        . = ALIGN(4);
        *(.ARM.exidx*)
    } > DDR_MEM

    .data :
    {
        _start_data = .;
        KEEP(*(.data*))
        . = ALIGN(4);
        KEEP(*(.ramcode))
        . = ALIGN(4);
        _end_data = .;
    } > DDR_MEM

    .bss (NOLOAD) :
    {
        _start_bss = .;
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _end_bss = .;
        __bss_end__ = .;
        _end = .;
    } > DDR_MEM

    /* Reserved stack, used only in BL33 mode (NXP_IMX8_BL33: sp = END_STACK,
     * see src/boot_aarch64_start.S). When started from U-Boot proper the
     * simple startup keeps sp at the image base (_vector_table) as before.
     * NOLOAD, after _end, so the U-Boot "go" build is unchanged. */
    .stack (NOLOAD) :
    {
        . = ALIGN(16);
        . += 0x40000;      /* 256 KB */
        . = ALIGN(16);
        END_STACK = .;
    } > DDR_MEM
    . = ALIGN(4);
}

kernel_addr  = 0x40540000;
update_addr  = 0x46000000;
dts_addr     = 0x43000000;
kernel_load_addr = 0x44000000;
dts_load_addr = 0x45000000;

_wolfboot_partition_boot_address = kernel_addr;
_wolfboot_partition_update_address = update_addr;
