# SNES SPC Player

A music player ROM for Super Nintendo Entertainment System (SNES) that plays SPC700 sound files.

## Features

- **C Language Based**: Written in C for maintainability and portability
- **SPC File Conversion**: Utility to convert SPC files to binary format for ROM embedding
- **Multiple Track Support**: Bundle multiple SPC files into a single ROM
- **Playback Control**: Play, pause, stop, next/previous track controls via SNES controller

## Project Structure

```
spc-player/
├── src/              # Source code
│   ├── main.c        # Main program and controller handling
│   ├── snes.c        # SNES hardware initialization
│   ├── spc_player.c  # SPC player core functionality
│   ├── init.s        # Assembly initialization code
│   └── snes_regs.inc # Hardware register definitions
├── include/          # Header files
│   ├── snes.h        # SNES hardware definitions
│   └── spc_player.h  # SPC player interface
├── tools/            # Converter utilities
│   ├── spc2bin.c     # SPC to binary converter
│   └── spcbundle.c   # Multiple SPC bundler
├── data/             # Place your .spc files here
├── lorom.cfg         # Linker configuration for LoROM
└── Makefile          # Build system
```

## Requirements

- **cc65 toolchain**: C compiler and assembler for 65816 CPU
  - Download from: https://cc65.github.io/
  - Or install via package manager (e.g., `apt install cc65`)
- **GCC**: For building converter tools
- **Make**: For build automation

## Building

1. **Install cc65 toolchain**:
   ```bash
   # On Ubuntu/Debian
   sudo apt install cc65
   
   # Or install from source
   # See: https://github.com/cc65/cc65
   ```

2. **Add your SPC files**:
   ```bash
   # Copy your .spc files to the data/ directory
   cp /path/to/your/music.spc data/
   ```

3. **Build the ROM**:
   ```bash
   make
   ```

4. **The ROM will be created at**: `build/spc-player.sfc`

## Usage

### Controller Controls

- **START**: Play/Pause
- **SELECT**: Stop
- **RIGHT / A**: Next track
- **LEFT / B**: Previous track

### Adding SPC Files

1. Place `.spc` files in the `data/` directory
2. Run `make` to rebuild the ROM
3. The ROM will automatically include all SPC files found

### Running the ROM

Load `build/spc-player.sfc` in your favorite SNES emulator:
- **bsnes**: Most accurate
- **Snes9x**: Good balance of accuracy and speed
- **ZSNES**: Older but still works

Or flash to a physical cartridge using a flash cart device.

## SPC File Format

SPC (SPC700) files are save states of the SNES audio processor. They contain:
- CPU register state (A, X, Y, PSW, SP, PC)
- 64KB of audio RAM
- DSP register values
- Metadata (song title, game title, artist, etc.)

### Converting SPC Files Manually

```bash
# Build converter tool
make build/spc2bin

# Convert single file
./build/spc2bin input.spc output.bin

# Batch convert multiple files
./build/spc2bin -batch file1.spc file2.spc file3.spc
```

### Bundling SPC Files

```bash
# Build bundler tool
make build/spcbundle

# Create C source bundle
./build/spcbundle output.c file1.spc.bin file2.spc.bin
```

## Technical Details

### Memory Map (LoROM)

- **$8000-$FFFF**: ROM Bank 0 (32KB) - Program code
- **$018000-$01FFFF**: ROM Bank 1 (32KB) - SPC data

### SPC700 Communication

The player uploads SPC data to the audio processor using:
1. Initial Program Loader (IPL) upload
2. RAM transfer via communication ports ($2140-$2143)
3. DSP register initialization
4. CPU state restoration

### Build Process

1. Convert `.spc` files to `.bin` format (removes header, keeps raw data)
2. Bundle all `.bin` files into a C source file
3. Compile C sources to assembly
4. Assemble all sources to object files
5. Link with LoROM configuration to create `.sfc` ROM

## Customization

### Changing Memory Map

Edit `lorom.cfg` to adjust ROM size or memory layout.

### Adding Features

- **Display**: Modify `src/main.c` to add text display using SNES PPU
- **More Controls**: Add additional button mappings
- **Playlist**: Implement track ordering or shuffle

### Optimizing Size

Large SPC files can make the ROM very big. To optimize:
- Use compressed SPC data
- Implement streaming from additional ROM banks
- Share common samples between tracks

## Limitations

- ROM size limited by cartridge type (LoROM supports up to 4MB)
- Each SPC file contains full 64KB RAM dump
- No compression implemented (yet)
- Basic playback only (no seeking within track)

## License

This project is provided as-is for educational and personal use.

## Resources

- **SPC File Format**: http://snesmusic.org/files/spc_file_format.txt
- **SNES Hardware**: https://wiki.superfamicom.org/
- **cc65 Documentation**: https://cc65.github.io/doc/
- **SPC700 Documentation**: https://wiki.superfamicom.org/spc700-reference

## Contributing

Contributions are welcome! Areas for improvement:
- Compression support
- Better error handling
- On-screen display (OSD) for track info
- Save state support
- Advanced playback features

## Acknowledgments

- SPC file format by various SNES music archivists
- cc65 team for the excellent toolchain
- SNES homebrew community for documentation and tools 
