/*
    GNU linker script for SAME70
*/

/* Specify the memory areas */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00004000, LENGTH = 2048K - 16K
    RAM (xrw)   : ORIGIN = 0x20000000, LENGTH = 384K
}

/* Top end of the stack, with room for double-tap variable */
_estack = ORIGIN(RAM) + LENGTH(RAM) - 8;
_sstack = _estack - 16K;

_sheap = _ebss;
_eheap = _sstack;
