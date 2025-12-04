#ifndef SPC_PLAYER_H
#define SPC_PLAYER_H

#include <stdint.h>

/* SPC file format constants */
#define SPC_HEADER_SIZE 0x100
#define SPC_RAM_SIZE 0x10000
#define SPC_DSP_SIZE 0x80
#define SPC_EXTRA_RAM_SIZE 0x40

/* SPC file header structure */
typedef struct {
    char header[33];        /* "SNES-SPC700 Sound File Data" */
    uint8_t version_minor;
    uint8_t id_tag;
    uint8_t version_minor2;
    uint8_t pc_low;
    uint8_t pc_high;
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t psw;
    uint8_t sp;
    uint8_t reserved[2];
    char song_title[32];
    char game_title[32];
    char dumper_name[16];
    char comments[32];
    uint8_t date[11];
    uint8_t length[3];
    uint8_t fade[4];
    char artist[32];
    uint8_t channel_disable;
    uint8_t emulator;
    uint8_t reserved2[45];
} spc_header_t;

/* SPC data structure in ROM */
typedef struct {
    uint16_t pc;
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t psw;
    uint8_t sp;
    uint8_t ram[SPC_RAM_SIZE];
    uint8_t dsp_regs[SPC_DSP_SIZE];
    uint8_t extra_ram[SPC_EXTRA_RAM_SIZE];
    char title[32];
} spc_data_t;

/* Player state */
typedef struct {
    uint8_t current_track;
    uint8_t num_tracks;
    uint8_t playing;
    uint8_t paused;
} player_state_t;

/* Function prototypes */
void spc_init(void);
void spc_load(uint8_t track_num);
void spc_play(void);
void spc_stop(void);
void spc_pause(void);
void spc_next_track(void);
void spc_prev_track(void);
player_state_t* spc_get_state(void);

#endif /* SPC_PLAYER_H */
