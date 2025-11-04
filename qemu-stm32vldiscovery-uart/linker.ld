/* STM32F1 (stm32vldiscovery) */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
  RAM   (rwx): ORIGIN = 0x20000000, LENGTH =  8K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector ORIGIN(FLASH) :
  {
    KEEP(*(.isr_vector))
  } > FLASH

  .text :
  {
    *(.text*) *(.rodata*) *(.glue_7) *(.glue_7t)
  } > FLASH

  .data : AT (ADDR(.text) + SIZEOF(.text))
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM

  .bss (NOLOAD) :
  {
    _sbss = .;
    *(.bss*) *(COMMON)
    _ebss = .;
  } > RAM
}
