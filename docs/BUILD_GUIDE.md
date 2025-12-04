# Build Guide for SNES SPC Player

This guide provides detailed instructions for building the SPC Player ROM.

## Prerequisites

### Required Software

1. **cc65 Toolchain** (version 2.18 or later)
   - C compiler for 65816 CPU
   - Assembler for 65816
   - Linker

2. **GCC** (or any C compiler for host)
   - Used to build converter tools
   - Most systems have this pre-installed

3. **GNU Make**
   - Build automation tool
   - Usually pre-installed on Unix-like systems

### Installing cc65

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install cc65
```

#### macOS (using Homebrew)
```bash
brew install cc65
```

#### From Source
```bash
git clone https://github.com/cc65/cc65.git
cd cc65
make
sudo make install
```

#### Windows
Download pre-built binaries from: https://sourceforge.net/projects/cc65/files/

## Build Process

### Step 1: Verify Prerequisites

```bash
# Check cc65 installation
cc65 --version
ca65 --version
ld65 --version

# Check GCC
gcc --version

# Check Make
make --version
```

### Step 2: Prepare SPC Files

Place your SPC files in the `data/` directory:

```bash
mkdir -p data
cp /path/to/your/music/*.spc data/
```

**Note**: If no SPC files are present, the build will create an empty ROM that can still be used to test the framework.

### Step 3: Build the ROM

```bash
make
```

This will:
1. Create the `build/` directory
2. Compile converter tools (`spc2bin`, `spcbundle`)
3. Convert each `.spc` file to `.spc.bin` format
4. Bundle all converted files into `spc_data.c`
5. Compile all C sources to assembly
6. Assemble all sources to object files
7. Link everything into `build/spc-player.sfc`

### Step 4: Verify the Build

```bash
ls -lh build/spc-player.sfc
```

You should see the ROM file with its size displayed.

## Build Targets

### Default Target (all)
```bash
make
# or
make all
```
Builds the complete ROM.

### Clean Build
```bash
make clean
make
```
Removes all build artifacts and rebuilds from scratch.

### Test Tools
```bash
make test-tools
```
Builds and tests the converter tools only.

### Help
```bash
make help
```
Displays available targets and usage information.

## Troubleshooting

### Error: cc65 not found

**Solution**: Install cc65 or set the `CC65_PATH` variable:
```bash
make CC65_PATH=/path/to/cc65
```

### Error: Cannot open input file

**Solution**: Ensure your SPC files are in the `data/` directory and have `.spc` extension.

### Error: Invalid SPC file

**Solution**: Verify your SPC files are valid:
- Must start with "SNES-SPC700 Sound File Data"
- Must be at least 66,048 bytes (0x10200)
- Use a hex editor or SPC player to verify

### ROM is too large

**Solution**: 
- Reduce number of SPC files
- Use smaller SPC files
- Modify `lorom.cfg` to use HiROM mapping for larger capacity

### Linker errors

**Solution**:
- Check that all object files are created
- Verify `lorom.cfg` is correct
- Ensure sufficient ROM space defined

## Advanced Building

### Custom cc65 Location

```bash
make CC65_PATH=/opt/cc65
```

### Verbose Build

```bash
make V=1
```

### Parallel Build

```bash
make -j4
```

### Cross-Platform Considerations

#### Linux/macOS
Should work out of the box with standard tools.

#### Windows
- Use MinGW, Cygwin, or WSL for Unix-like environment
- Or use native Windows builds of make and cc65
- Path separators may need adjustment in Makefile

## Customizing the Build

### Changing ROM Type

Edit `lorom.cfg` to change from LoROM to HiROM:

```
ROM0: start = $C000, size = $10000, file = %O, fill = yes;
```

### Adding Optimization

Modify `CFLAGS` in Makefile:
```makefile
CFLAGS = -t none -O -Oi -I $(INCLUDE_DIR)
```

### Debug Build

Add debug symbols:
```makefile
CFLAGS = -t none -g -I $(INCLUDE_DIR)
```

## Next Steps

After successful build:
1. Test ROM in emulator (bsnes, Snes9x)
2. Verify audio playback
3. Test controller inputs
4. Flash to physical cartridge if desired

## Additional Resources

- cc65 documentation: https://cc65.github.io/doc/
- SNES development: https://wiki.superfamicom.org/
- SPC format: http://snesmusic.org/files/spc_file_format.txt
