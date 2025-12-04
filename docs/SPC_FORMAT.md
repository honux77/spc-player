# SPC File Format Documentation

## Overview

SPC (SPC700) files are save state dumps of the SNES audio subsystem. They capture the complete state of the SPC700 CPU and DSP at a specific moment, allowing music to be played back accurately.

## File Structure

### Total Size
Typically 66,048 bytes (0x10200):
- Header: 256 bytes (0x100)
- RAM: 65,536 bytes (0x10000)
- DSP Registers: 128 bytes (0x80)
- Extra RAM: 64 bytes (0x40) [optional]

## Header Format (256 bytes)

### Offset 0x00-0x20 (33 bytes): File Identification
```
"SNES-SPC700 Sound File Data v0.30"
```

### Offset 0x21 (1 byte): Version Minor
Usually 0x1E for version 0.30

### Offset 0x22 (1 byte): ID666 Tag Present
- 0x1A: Has ID666 tag
- 0x1B: Has extended ID666 tag

### Offset 0x23 (1 byte): Version Minor (repeat)

### Offset 0x24-0x25 (2 bytes): Program Counter (PC)
Little-endian 16-bit value

### Offset 0x26 (1 byte): Accumulator (A)

### Offset 0x27 (1 byte): X Register

### Offset 0x28 (1 byte): Y Register

### Offset 0x29 (1 byte): Processor Status Word (PSW)

### Offset 0x2A (1 byte): Stack Pointer (SP)

### Offset 0x2B-0x2D (3 bytes): Reserved

### Offset 0x2E-0x4D (32 bytes): Song Title
ASCII string, zero-padded

### Offset 0x4E-0x6D (32 bytes): Game Title
ASCII string, zero-padded

### Offset 0x6E-0x7D (16 bytes): Name of Dumper
ASCII string, zero-padded

### Offset 0x7E-0x9D (32 bytes): Comments
ASCII string, zero-padded

### Offset 0x9E-0xA8 (11 bytes): Date
Format: "MM/DD/YYYY" or "DD/MM/YYYY"

### Offset 0xA9-0xAB (3 bytes): Play Length
Little-endian 24-bit value in 1/64000 second units

### Offset 0xAC-0xAF (4 bytes): Fade Length
Little-endian 32-bit value in milliseconds

### Offset 0xB0-0xCF (32 bytes): Artist Name
ASCII string, zero-padded

### Offset 0xD0 (1 byte): Channel Disable
Bit flags for disabling channels

### Offset 0xD1 (1 byte): Emulator Used
- 0: Unknown
- 1: ZSNES
- 2: Snes9x

### Offset 0xD2-0xD3 (45 bytes): Reserved

## RAM Section (65,536 bytes)

### Offset 0x100-0x10100
Complete dump of SPC700's 64KB address space including:
- Zero page (0x00-0xFF)
- Program memory
- Sound data
- Working RAM
- I/O ports (0xF0-0xFF)

## DSP Registers (128 bytes)

### Offset 0x10100-0x10180
All 128 DSP registers (0x00-0x7F)

Important registers:
- 0x00-0x07: Voice 0 parameters
- 0x10-0x17: Voice 1 parameters
- ...
- 0x70-0x77: Voice 7 parameters
- 0x0C: Master Volume Left
- 0x1C: Master Volume Right
- 0x2C: Echo Volume Left
- 0x3C: Echo Volume Right
- 0x4C: Key On
- 0x5C: Key Off
- 0x6C: Flags

## Extra RAM (64 bytes) [Optional]

### Offset 0x10180-0x101C0
Extra 64 bytes of RAM at addresses 0xFFC0-0xFFFF
Used for IPL ROM area

## Binary Format Used by Converter

The `spc2bin` tool converts SPC to a simplified binary format:

```c
struct spc_binary {
    uint16_t pc;                    // 2 bytes
    uint8_t  a;                     // 1 byte
    uint8_t  x;                     // 1 byte
    uint8_t  y;                     // 1 byte
    uint8_t  psw;                   // 1 byte
    uint8_t  sp;                    // 1 byte
    char     title[32];             // 32 bytes
    uint8_t  ram[0x10000];          // 65,536 bytes
    uint8_t  dsp_regs[0x80];        // 128 bytes
    uint8_t  extra_ram[0x40];       // 64 bytes
};
```

Total: 65,607 bytes per track

## Usage in ROM

The converted binary format is embedded directly into the ROM and used to:

1. **Initialize SPC700 CPU**
   - Set PC, A, X, Y, PSW, SP registers

2. **Upload RAM Contents**
   - Transfer all 64KB to SPC700 memory

3. **Configure DSP**
   - Write all DSP register values

4. **Restore Extra RAM**
   - Upload IPL area if needed

## SPC700 CPU Registers

### A (Accumulator)
8-bit primary register for arithmetic operations

### X, Y (Index Registers)
8-bit registers for indexing and temporary storage

### SP (Stack Pointer)
8-bit stack pointer (grows downward from 0x1FF)

### PC (Program Counter)
16-bit instruction pointer

### PSW (Processor Status Word)
```
Bit 7: N - Negative flag
Bit 6: V - Overflow flag
Bit 5: P - Direct page flag
Bit 4: B - Break flag
Bit 3: H - Half-carry flag
Bit 2: I - Interrupt enable flag
Bit 1: Z - Zero flag
Bit 0: C - Carry flag
```

## DSP Voice Structure

Each voice (0-7) has 16 bytes of parameters:

```
+0x0: VxVOLL  - Left volume
+0x1: VxVOLR  - Right volume
+0x2: VxPITCHL - Pitch low byte
+0x3: VxPITCHH - Pitch high byte
+0x4: VxSRCN  - Source number
+0x5: VxADSR1 - ADSR1
+0x6: VxADSR2 - ADSR2
+0x7: VxGAIN  - Gain
+0x8: VxENVX  - Current envelope (read-only)
+0x9: VxOUTX  - Current output (read-only)
```

## Common Issues

### Invalid Header
- File must start with exact string
- Check with hex editor

### Wrong Size
- Minimum: 66,048 bytes
- Some files may be larger with additional metadata

### Corrupted RAM
- Verify checksum if available
- Test in SPC player first

### Missing Extra RAM
- Optional section
- May not be present in older SPC files

## Tools for Working with SPC Files

### Our Tools
- `spc2bin`: Converts SPC to binary format
- `spcbundle`: Bundles multiple SPC binaries

### External Tools
- **SNESamp**: SPC player for Windows
- **Audio Overload**: Multi-format player
- **OpenSPC**: Cross-platform player
- **SPCTool**: SPC editor and analyzer

## References

- SPC File Format v0.30: http://snesmusic.org/files/spc_file_format.txt
- SPC700 CPU: https://wiki.superfamicom.org/spc700-reference
- DSP Registers: https://wiki.superfamicom.org/dsp-registers
- SNESMUSIC.ORG: http://snesmusic.org/
