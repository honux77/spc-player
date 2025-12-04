/*
 * SNES Hardware Initialization and Utility Functions
 */

#include "snes.h"

void snes_init(void) {
    /* Disable screen */
    INIDISP = 0x80;
    
    /* Disable NMI and IRQ */
    NMITIMEN = 0x00;
    
    /* Initialize joypad auto-read */
    asm volatile("stz $4016");  /* Strobe off */
}

void wait_vblank(void) {
    /* Wait for NMI flag to be set */
    while (!(RDNMI & 0x80));
    
    /* Clear NMI flag by reading */
    (void)RDNMI;
}

uint16_t read_joypad(void) {
    /* Read joypad 1 data (auto-read must be enabled) */
    uint16_t joy = JOY1L | (JOY1H << 8);
    return joy;
}
