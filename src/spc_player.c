/*
 * SPC Player Core Implementation
 * Handles loading and playing SPC data on SNES hardware
 */

#include "spc_player.h"
#include "snes.h"
#include <string.h>

/* External SPC data - these will be linked from converted binary files */
extern const spc_data_t spc_tracks[];
extern const uint8_t spc_track_count;

static player_state_t player_state;

/* SPC700 IPL (Initial Program Loader) for uploading data */
static const uint8_t spc_ipl[] = {
    0xCD, 0xEF,             /* mov   x,#$ef */
    0xBD,                   /* mov   sp,x */
    0xE8, 0x00,             /* mov   a,#$00 */
    0xC4, 0xF4,             /* mov   $f4,a */
    0xC4, 0xF5,             /* mov   $f5,a */
    0xC4, 0xF6,             /* mov   $f6,a */
    0xC4, 0xF7,             /* mov   $f7,a */
    0x8F, 0x6C, 0xF2,       /* mov   $f2,#$6c */
    0x8F, 0xE0, 0xF3,       /* mov   $f3,#$e0 */
    0x8F, 0x4C, 0xF2,       /* mov   $f2,#$4c */
    0x8F, 0x00, 0xF3,       /* mov   $f3,#$00 */
    0xCD, 0xBD,             /* mov   x,#$bd */
    0xE8, 0x00,             /* mov   a,#$00 */
    0xC4, 0xF4,             /* mov   $f4,a */
    0xD8, 0xF4,             /* mov   $f4,x */
    0x7E, 0xFD,             /* cmp   y,$f4 */
    0xD0, 0xFC,             /* bne   wait1 */
    0xE4, 0xF5,             /* mov   a,$f5 */
    0xCB, 0xF4,             /* mov   $f4,y */
    0xD7, 0x00,             /* mov   ($00)+y,a */
    0xFC,                   /* inc   y */
    0xD0, 0xF3,             /* bne   wait2 */
    0xAB, 0x01,             /* inc   $01 */
    0x10, 0xEF,             /* bpl   wait1 */
    0x7E, 0xF4,             /* cmp   y,$f4 */
    0x10, 0xEB              /* bpl   wait1 */
};

void spc_init(void) {
    player_state.current_track = 0;
    player_state.num_tracks = spc_track_count;
    player_state.playing = 0;
    player_state.paused = 0;
}

/* Wait for SPC ready signal */
static void spc_wait_ready(void) {
    uint8_t counter = 0;
    while (APUIO0 != 0xAA || APUIO1 != 0xBB) {
        /* Wait with timeout */
        if (++counter == 0) break;
    }
}

/* Upload data to SPC700 RAM */
static void spc_upload_data(uint16_t addr, const uint8_t* data, uint16_t size) {
    APUIO2 = (uint8_t)(addr & 0xFF);
    APUIO3 = (uint8_t)(addr >> 8);
    
    uint8_t port0 = 0;
    for (uint16_t i = 0; i < size; i++) {
        APUIO1 = data[i];
        APUIO0 = port0;
        
        /* Wait for acknowledgment */
        while (APUIO0 != port0);
        
        port0++;
    }
}

/* Upload SPC state to SPC700 */
static void spc_upload_state(const spc_data_t* spc) {
    /* Reset SPC700 */
    APUIO0 = 0xCC;
    APUIO1 = 0x01;
    
    /* Wait for ready */
    spc_wait_ready();
    
    /* Upload IPL loader */
    spc_upload_data(0xFFC0, spc_ipl, sizeof(spc_ipl));
    
    /* Upload SPC RAM */
    spc_upload_data(0x0000, spc->ram, SPC_RAM_SIZE);
    
    /* Set DSP registers */
    for (uint8_t i = 0; i < SPC_DSP_SIZE; i++) {
        APUIO0 = i;
        APUIO1 = spc->dsp_regs[i];
        
        /* Wait a bit for DSP to process */
        for (volatile uint8_t j = 0; j < 10; j++);
    }
    
    /* Set CPU registers through communication ports */
    APUIO0 = spc->a;
    APUIO1 = spc->x;
    APUIO2 = spc->y;
    APUIO3 = spc->psw;
    
    /* Start execution */
    APUIO0 = (uint8_t)(spc->pc & 0xFF);
    APUIO1 = (uint8_t)(spc->pc >> 8);
}

void spc_load(uint8_t track_num) {
    if (track_num >= player_state.num_tracks) {
        return;
    }
    
    player_state.current_track = track_num;
    spc_upload_state(&spc_tracks[track_num]);
}

void spc_play(void) {
    if (!player_state.playing) {
        spc_load(player_state.current_track);
        player_state.playing = 1;
        player_state.paused = 0;
    }
}

void spc_stop(void) {
    player_state.playing = 0;
    player_state.paused = 0;
    
    /* Send stop command to SPC */
    APUIO0 = 0xFF;
}

void spc_pause(void) {
    player_state.paused = !player_state.paused;
}

void spc_next_track(void) {
    player_state.current_track++;
    if (player_state.current_track >= player_state.num_tracks) {
        player_state.current_track = 0;
    }
    
    if (player_state.playing) {
        spc_load(player_state.current_track);
    }
}

void spc_prev_track(void) {
    if (player_state.current_track == 0) {
        player_state.current_track = player_state.num_tracks - 1;
    } else {
        player_state.current_track--;
    }
    
    if (player_state.playing) {
        spc_load(player_state.current_track);
    }
}

player_state_t* spc_get_state(void) {
    return &player_state;
}
