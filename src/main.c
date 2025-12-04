/*
 * SNES SPC Player - Main Program
 * A music player for SNES that plays SPC700 sound files
 */

#include "snes.h"
#include "spc_player.h"

/* Previous joypad state for edge detection */
static uint16_t prev_joy = 0;

/* Check if button was just pressed (edge detection) */
static uint8_t button_pressed(uint16_t button, uint16_t joy) {
    return (joy & button) && !(prev_joy & button);
}

int main(void) {
    /* Initialize SNES hardware */
    snes_init();
    
    /* Initialize SPC player */
    spc_init();
    
    /* Enable NMI for VBlank */
    NMITIMEN = 0x81;
    
    /* Enable screen */
    INIDISP = 0x0F;
    
    /* Auto-play first track */
    spc_play();
    
    /* Main loop */
    while (1) {
        /* Wait for VBlank */
        wait_vblank();
        
        /* Read joypad */
        uint16_t joy = read_joypad();
        
        /* Handle button presses */
        player_state_t* state = spc_get_state();
        
        if (button_pressed(JOY_START, joy)) {
            /* Start/Pause */
            if (state->playing) {
                spc_pause();
            } else {
                spc_play();
            }
        }
        
        if (button_pressed(JOY_RIGHT, joy) || button_pressed(JOY_A, joy)) {
            /* Next track */
            spc_next_track();
        }
        
        if (button_pressed(JOY_LEFT, joy) || button_pressed(JOY_B, joy)) {
            /* Previous track */
            spc_prev_track();
        }
        
        if (button_pressed(JOY_SELECT, joy)) {
            /* Stop */
            spc_stop();
        }
        
        /* Store current joypad state */
        prev_joy = joy;
    }
    
    return 0;
}
