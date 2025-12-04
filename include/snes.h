#ifndef SNES_H
#define SNES_H

#include <stdint.h>

/* SNES hardware registers */
#define INIDISP  (*(volatile uint8_t*)0x2100)
#define NMITIMEN (*(volatile uint8_t*)0x4200)
#define RDNMI    (*(volatile uint8_t*)0x4210)

/* SPC700 communication ports */
#define APUIO0   (*(volatile uint8_t*)0x2140)
#define APUIO1   (*(volatile uint8_t*)0x2141)
#define APUIO2   (*(volatile uint8_t*)0x2142)
#define APUIO3   (*(volatile uint8_t*)0x2143)

/* Joypad registers */
#define JOY1L    (*(volatile uint8_t*)0x4218)
#define JOY1H    (*(volatile uint8_t*)0x4219)

/* Joypad button masks */
#define JOY_B      0x8000
#define JOY_Y      0x4000
#define JOY_SELECT 0x2000
#define JOY_START  0x1000
#define JOY_UP     0x0800
#define JOY_DOWN   0x0400
#define JOY_LEFT   0x0200
#define JOY_RIGHT  0x0100
#define JOY_A      0x0080
#define JOY_X      0x0040
#define JOY_L      0x0020
#define JOY_R      0x0010

/* Function prototypes */
void snes_init(void);
void wait_vblank(void);
uint16_t read_joypad(void);

#endif /* SNES_H */
