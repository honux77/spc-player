/*
 * SPC Bundle Creator
 * Creates a C source file containing multiple SPC binaries for linking
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SPC_RAM_SIZE 0x10000
#define SPC_DSP_SIZE 0x80
#define SPC_EXTRA_RAM_SIZE 0x40

typedef struct {
    uint16_t pc;
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t psw;
    uint8_t sp;
    char title[32];
    uint8_t ram[SPC_RAM_SIZE];
    uint8_t dsp_regs[SPC_DSP_SIZE];
    uint8_t extra_ram[SPC_EXTRA_RAM_SIZE];
} spc_binary_t;

void write_byte_array(FILE *out, const uint8_t *data, size_t size, const char *indent) {
    for (size_t i = 0; i < size; i++) {
        if (i % 16 == 0) {
            fprintf(out, "\n%s", indent);
        }
        fprintf(out, "0x%02X", data[i]);
        if (i < size - 1) {
            fprintf(out, ", ");
        }
    }
    fprintf(out, "\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("SPC Bundle Creator\n");
        printf("Usage: %s <output.c> <file1.spc.bin> <file2.spc.bin> ...\n", argv[0]);
        return 1;
    }

    const char *output_file = argv[1];
    int num_files = argc - 2;

    /* Open output file */
    FILE *out = fopen(output_file, "w");
    if (!out) {
        fprintf(stderr, "Error: Cannot create output file %s\n", output_file);
        return 1;
    }

    /* Write header */
    fprintf(out, "/*\n");
    fprintf(out, " * Auto-generated SPC data bundle\n");
    fprintf(out, " * Contains %d SPC tracks\n", num_files);
    fprintf(out, " */\n\n");
    fprintf(out, "#include \"spc_player.h\"\n\n");

    /* Process each input file */
    for (int i = 0; i < num_files; i++) {
        const char *input_file = argv[i + 2];
        
        FILE *in = fopen(input_file, "rb");
        if (!in) {
            fprintf(stderr, "Error: Cannot open input file %s\n", input_file);
            fclose(out);
            return 1;
        }

        spc_binary_t spc;
        if (fread(&spc, 1, sizeof(spc_binary_t), in) != sizeof(spc_binary_t)) {
            fprintf(stderr, "Error: Invalid SPC binary file %s\n", input_file);
            fclose(in);
            fclose(out);
            return 1;
        }
        fclose(in);

        /* Write track data */
        fprintf(out, "/* Track %d: %s */\n", i, spc.title);
        fprintf(out, "static const spc_data_t spc_track_%d = {\n", i);
        fprintf(out, "    .pc = 0x%04X,\n", spc.pc);
        fprintf(out, "    .a = 0x%02X,\n", spc.a);
        fprintf(out, "    .x = 0x%02X,\n", spc.x);
        fprintf(out, "    .y = 0x%02X,\n", spc.y);
        fprintf(out, "    .psw = 0x%02X,\n", spc.psw);
        fprintf(out, "    .sp = 0x%02X,\n", spc.sp);
        fprintf(out, "    .ram = {");
        write_byte_array(out, spc.ram, SPC_RAM_SIZE, "        ");
        fprintf(out, "    },\n");
        fprintf(out, "    .dsp_regs = {");
        write_byte_array(out, spc.dsp_regs, SPC_DSP_SIZE, "        ");
        fprintf(out, "    },\n");
        fprintf(out, "    .extra_ram = {");
        write_byte_array(out, spc.extra_ram, SPC_EXTRA_RAM_SIZE, "        ");
        fprintf(out, "    },\n");
        fprintf(out, "    .title = \"%s\"\n", spc.title);
        fprintf(out, "};\n\n");
    }

    /* Write track array */
    fprintf(out, "/* Array of all tracks */\n");
    fprintf(out, "const spc_data_t spc_tracks[] = {\n");
    for (int i = 0; i < num_files; i++) {
        fprintf(out, "    spc_track_%d%s\n", i, (i < num_files - 1) ? "," : "");
    }
    fprintf(out, "};\n\n");

    /* Write track count */
    fprintf(out, "const uint8_t spc_track_count = %d;\n", num_files);

    fclose(out);

    printf("Created bundle: %s with %d tracks\n", output_file, num_files);
    return 0;
}
