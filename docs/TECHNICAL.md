# Technical Documentation

## Architecture Overview

The SPC Player consists of three main components:

1. **Converter Tools** (Host PC)
   - `spc2bin`: Converts SPC files to binary format
   - `spcbundle`: Bundles multiple binaries into C source

2. **SNES ROM Framework** (65816 CPU)
   - Hardware initialization
   - Main loop and input handling
   - SPC700 communication

3. **SPC Player Core** (SPC700 Audio Processor)
   - Audio RAM management
   - DSP register configuration
   - Sound playback

## Memory Architecture

### SNES CPU (65816)

```
$0000-$00FF    Zero Page
$0100-$01FF    Stack
$0200-$1FFF    Working RAM
$2000-$5FFF    Hardware Registers
$6000-$7FFF    Additional RAM (optional)
$8000-$FFFF    ROM (LoROM: banks $00-$7F, $80-$FF)
```

### SPC700 Audio RAM

```
$0000-$00EF    Zero Page / Working RAM
$00F0-$00FF    I/O Registers
$0100-$FFBF    Program/Sample Data
$FFC0-$FFFF    IPL ROM area (bootloader)
```

### ROM Layout (LoROM)

```
Bank $00-$7F:
  $8000-$FFFF  Program Code (32KB per bank)

Bank $80-$FF: (Mirror of $00-$7F)
  $8000-$FFFF  Program Code

Bank $01+:
  $8000-$FFFF  SPC Data
```

## SPC700 Communication Protocol

### Communication Ports

The SNES CPU communicates with SPC700 via 4 ports:

```
$2140 (APUIO0) - Data/Status byte 0
$2141 (APUIO1) - Data byte 1
$2142 (APUIO2) - Data byte 2
$2143 (APUIO3) - Data byte 3
```

### Upload Protocol

1. **Reset SPC700**
   ```c
   APUIO0 = 0xCC;
   APUIO1 = 0x01;
   ```

2. **Wait for Ready Signal**
   ```c
   while (APUIO0 != 0xAA || APUIO1 != 0xBB);
   ```

3. **Upload IPL Loader**
   - Small bootloader to SPC RAM
   - Enables faster data transfer

4. **Transfer RAM Data**
   - 64KB audio RAM
   - Uploaded in chunks via ports

5. **Set DSP Registers**
   - 128 DSP registers
   - Configure audio parameters

6. **Start Execution**
   - Set PC to start address
   - Begin audio playback

## Data Structures

### SPC Binary Format

```c
typedef struct {
    uint16_t pc;              // Program counter
    uint8_t a;                // Accumulator
    uint8_t x;                // X register
    uint8_t y;                // Y register
    uint8_t psw;              // Status flags
    uint8_t sp;               // Stack pointer
    char title[32];           // Track title
    uint8_t ram[0x10000];     // 64KB RAM
    uint8_t dsp_regs[0x80];   // DSP registers
    uint8_t extra_ram[0x40];  // IPL area
} spc_binary_t;
```

### Player State

```c
typedef struct {
    uint8_t current_track;    // Current track index
    uint8_t num_tracks;       // Total tracks
    uint8_t playing;          // Playing flag
    uint8_t paused;           // Paused flag
} player_state_t;
```

## Build Process

### Phase 1: SPC Conversion
```
.spc files → spc2bin → .spc.bin files
```

1. Read SPC header (256 bytes)
2. Extract CPU state (PC, A, X, Y, PSW, SP)
3. Read RAM (64KB)
4. Read DSP registers (128 bytes)
5. Read extra RAM (64 bytes)
6. Write binary format

### Phase 2: Bundling
```
.spc.bin files → spcbundle → spc_data.c
```

1. Read all binary files
2. Generate C array for each track
3. Create track array
4. Set track count

### Phase 3: Compilation
```
C sources → cc65 → Assembly → ca65 → Objects
```

1. Compile C to 65816 assembly
2. Assemble to object files
3. Include hand-written assembly

### Phase 4: Linking
```
Object files → ld65 → ROM (.sfc)
```

1. Link with LoROM configuration
2. Place code in ROM banks
3. Set interrupt vectors
4. Generate final ROM

## Timing and Performance

### Frame Rate
- NTSC: 60 Hz (16.67 ms/frame)
- PAL: 50 Hz (20 ms/frame)

### SPC Upload Time
- IPL loader: <1 frame
- 64KB RAM: ~100-200 frames (2-4 seconds)
- DSP setup: <1 frame
- **Total**: ~2-4 seconds per track

### Optimization Opportunities
1. **Incremental Updates**: Only upload changed data
2. **Compression**: Reduce data size
3. **Streaming**: Load while playing previous track
4. **Caching**: Keep common samples in RAM

## DSP Register Map

Key DSP registers for audio control:

```
$0C - Master Volume Left
$1C - Master Volume Right
$2C - Echo Volume Left
$3C - Echo Volume Right
$4C - Key On (start voices)
$5C - Key Off (stop voices)
$6C - Flags (reset, mute, etc.)
$7C - Endx (voice end flags)

Voice registers (per voice 0-7):
  $x0 - Left Volume
  $x1 - Right Volume
  $x2 - Pitch Low
  $x3 - Pitch High
  $x4 - Source Number
  $x5 - ADSR1
  $x6 - ADSR2
  $x7 - Gain
```

## Hardware Register Usage

### PPU Registers
```c
INIDISP  ($2100) - Screen brightness/blanking
NMITIMEN ($4200) - NMI enable
```

### Joypad Registers
```c
JOY1L ($4218) - Joypad 1 low byte
JOY1H ($4219) - Joypad 1 high byte
```

### Button Masks
```c
#define JOY_A      0x0080
#define JOY_B      0x8000
#define JOY_START  0x1000
#define JOY_SELECT 0x2000
#define JOY_RIGHT  0x0100
#define JOY_LEFT   0x0200
```

## Interrupt Handling

### NMI (VBlank)
```assembly
nmi_handler:
    ; Save registers
    ; Read NMI status
    ; Auto-joypad read
    ; Restore registers
    rti
```

### Main Loop
```c
while (1) {
    wait_vblank();        // Wait for NMI
    read_joypad();        // Get input
    handle_input();       // Process buttons
}
```

## Error Handling

### SPC Upload Errors
- **Timeout**: SPC not responding
- **Verification**: Data mismatch
- **Invalid State**: Bad CPU registers

### Recovery Strategies
1. Retry upload
2. Reset SPC700
3. Skip corrupted track
4. Display error (future feature)

## Future Enhancements

### Planned Features
1. **Display System**
   - Track title on screen
   - Playlist visualization
   - Waveform display

2. **Advanced Playback**
   - Shuffle mode
   - Repeat modes
   - Fade in/out

3. **Compression**
   - LZ compression for SPC data
   - Shared sample banks
   - Streaming from additional banks

4. **Save System**
   - Remember position
   - Save favorites
   - SRAM for settings

### Technical Challenges

**ROM Size**
- Each SPC ~64KB
- LoROM limited to 4MB
- ~60 tracks maximum

**Upload Speed**
- 2-4 seconds per track
- Can't be easily reduced
- User must wait for track load

**Memory Constraints**
- Limited WRAM on SNES
- Can't cache multiple tracks
- Must reload each track

## Debugging

### Emulator Features
- **Memory Viewer**: Check uploaded data
- **Register Viewer**: Monitor SPC state
- **Audio Viewer**: Visualize waveforms
- **Breakpoints**: Debug upload process

### Common Issues
1. **No Audio**: Check DSP registers
2. **Distorted Audio**: Verify RAM upload
3. **Wrong Track**: Check index calculation
4. **Crash**: Verify interrupt vectors

## Code Organization

```
src/
├── init.s           # Startup & vectors
├── main.c           # Main program
├── snes.c           # Hardware functions
└── spc_player.c     # Player core

include/
├── snes.h           # Hardware definitions
└── spc_player.h     # Player interface

tools/
├── spc2bin.c        # SPC converter
└── spcbundle.c      # Bundle creator
```

## Build Configuration

### Compiler Flags
```makefile
CFLAGS = -t none -O -I $(INCLUDE_DIR)
```
- `-t none`: No target (bare metal)
- `-O`: Enable optimization
- `-I`: Include directory

### Linker Configuration
```
lorom.cfg - LoROM memory map
- ROM banks
- RAM areas
- Vector table
```

## Performance Metrics

### ROM Size
- Base ROM: ~32KB
- Per SPC: ~64KB
- 10 tracks: ~672KB
- 50 tracks: ~3.2MB

### Memory Usage
- Stack: ~512 bytes
- BSS: ~256 bytes
- Player state: ~16 bytes

### CPU Usage
- Main loop: <1% (mostly waiting)
- SPC upload: 100% (during transfer)
- Input: <0.1% per frame

## Standards Compliance

### SNES Compatibility
- Works on all SNES models
- NTSC and PAL compatible
- No special chips required

### SPC Format
- SPC700 v0.30 format
- Standard 66,048 byte files
- ID666 tag support

## References

### Documentation
- SNES Development Wiki
- SPC700 CPU Manual
- DSP Register Documentation
- cc65 Compiler Manual

### Tools
- bsnes debugger
- SPC player verification
- Hex editors for analysis
