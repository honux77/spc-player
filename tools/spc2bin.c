/*
 * SPC to Binary Converter
 * Converts SPC700 sound files to binary format for embedding in SNES ROM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SPC_HEADER_SIZE 0x100
#define SPC_RAM_SIZE 0x10000
#define SPC_DSP_SIZE 0x80
#define SPC_EXTRA_RAM_SIZE 0x40

typedef struct {
    char header[33];
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
} spc_binary_t;

int convert_spc_file(const char* input_file, const char* output_file) {
    FILE *in = fopen(input_file, "rb");
    if (!in) {
        fprintf(stderr, "Error: Cannot open input file %s\n", input_file);
        return 1;
    }

    /* Read entire SPC header as raw bytes */
    uint8_t header[SPC_HEADER_SIZE];
    if (fread(header, 1, SPC_HEADER_SIZE, in) != SPC_HEADER_SIZE) {
        fprintf(stderr, "Error: Invalid SPC file\n");
        fclose(in);
        return 1;
    }

    /* Verify SPC header */
    if (strncmp((char*)header, "SNES-SPC700 Sound File Data", 27) != 0) {
        fprintf(stderr, "Error: Invalid SPC header\n");
        fclose(in);
        return 1;
    }

    /* Create binary structure - extract fields at correct offsets */
    spc_binary_t bin;
    bin.pc = header[0x24] | (header[0x25] << 8);
    bin.a = header[0x26];
    bin.x = header[0x27];
    bin.y = header[0x28];
    bin.psw = header[0x29];
    bin.sp = header[0x2A];
    
    /* Copy song title (offset 0x2E, 32 bytes) */
    memcpy(bin.title, &header[0x2E], 32);
    bin.title[31] = '\0';  /* Ensure null termination */

    /* Read RAM */
    if (fread(bin.ram, 1, SPC_RAM_SIZE, in) != SPC_RAM_SIZE) {
        fprintf(stderr, "Error: Cannot read RAM data\n");
        fclose(in);
        return 1;
    }

    /* Read DSP registers */
    if (fread(bin.dsp_regs, 1, SPC_DSP_SIZE, in) != SPC_DSP_SIZE) {
        fprintf(stderr, "Error: Cannot read DSP registers\n");
        fclose(in);
        return 1;
    }

    /* Read extra RAM (optional) */
    size_t extra_read = fread(bin.extra_ram, 1, SPC_EXTRA_RAM_SIZE, in);
    if (extra_read < SPC_EXTRA_RAM_SIZE) {
        /* Extra RAM not present, zero fill */
        memset(bin.extra_ram + extra_read, 0, SPC_EXTRA_RAM_SIZE - extra_read);
    }

    fclose(in);

    /* Write binary file */
    FILE *out = fopen(output_file, "wb");
    if (!out) {
        fprintf(stderr, "Error: Cannot create output file %s\n", output_file);
        return 1;
    }

    fwrite(&bin, 1, sizeof(spc_binary_t), out);
    fclose(out);

    printf("Converted: %s -> %s\n", input_file, output_file);
    printf("  Title: %s\n", bin.title);
    printf("  PC: $%04X\n", bin.pc);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("SPC to Binary Converter\n");
        printf("Usage: %s <input.spc> [output.bin]\n", argv[0]);
        printf("       %s -batch <file1.spc> <file2.spc> ...\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-batch") == 0) {
        /* Batch mode: convert multiple files */
        if (argc < 3) {
            fprintf(stderr, "Error: No input files specified\n");
            return 1;
        }

        int errors = 0;
        for (int i = 2; i < argc; i++) {
            char output_file[256];
            snprintf(output_file, sizeof(output_file), "%s.bin", argv[i]);
            
            if (convert_spc_file(argv[i], output_file) != 0) {
                errors++;
            }
        }

        printf("\nConverted %d files, %d errors\n", argc - 2, errors);
        return errors > 0 ? 1 : 0;
    } else {
        /* Single file mode */
        const char *input_file = argv[1];
        const char *output_file = argc >= 3 ? argv[2] : "output.bin";
        
        return convert_spc_file(input_file, output_file);
    }
}
