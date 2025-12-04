# Usage Guide for SNES SPC Player

This guide explains how to use the SPC Player ROM on SNES hardware or emulators.

## Quick Start

### 1. Build the ROM

```bash
# Place your SPC files in data/
cp /path/to/*.spc data/

# Build
make

# Output will be at build/spc-player.sfc
```

### 2. Load in Emulator

Open `build/spc-player.sfc` in any SNES emulator:

- **bsnes/higan**: Most accurate, best for testing
- **Snes9x**: Good balance of accuracy and performance
- **Mesen-S**: Modern emulator with good debugging features
- **RetroArch**: Multi-platform with various SNES cores

### 3. Control the Player

**Basic Controls:**
- **START**: Play/Pause current track
- **SELECT**: Stop playback
- **A** or **RIGHT**: Next track
- **B** or **LEFT**: Previous track

## Detailed Controller Mapping

### Button Functions

| Button | Function | Description |
|--------|----------|-------------|
| START | Play/Pause | Toggle playback of current track |
| SELECT | Stop | Stop playback completely |
| A | Next Track | Skip to next track in playlist |
| B | Previous Track | Go back to previous track |
| RIGHT | Next Track | Alternative to A button |
| LEFT | Previous Track | Alternative to B button |
| X | - | Not used (reserved) |
| Y | - | Not used (reserved) |
| L | - | Not used (reserved) |
| R | - | Not used (reserved) |

### Playback States

1. **Stopped**: No audio playing
   - Press START to begin playing first track

2. **Playing**: Audio is playing
   - Press START to pause
   - Press A/RIGHT to skip to next track
   - Press B/LEFT to go to previous track
   - Press SELECT to stop

3. **Paused**: Playback suspended
   - Press START to resume
   - Press SELECT to stop

## Using on Real Hardware

### Flash Cart Method

1. Copy `build/spc-player.sfc` to your flash cart's SD card
2. Insert flash cart into SNES
3. Navigate to the ROM and load it
4. Use SNES controller to control playback

**Compatible Flash Carts:**
- SD2SNES/FXPak Pro
- Super Everdrive
- Super UFO
- Other compatible carts

### ROM Burning (Advanced)

For permanent cartridges:
1. Obtain appropriate EPROM chips
2. Use EPROM programmer to burn ROM
3. Install in SNES cartridge PCB
4. Use appropriate mapper (LoROM)

**Note**: This method is for advanced users with electronics experience.

## Creating Playlists

### Automatic Ordering

Tracks are automatically ordered alphabetically by filename:

```bash
data/
├── 01-title.spc      # Track 0
├── 02-stage1.spc     # Track 1
├── 03-boss.spc       # Track 2
└── 04-ending.spc     # Track 3
```

### Custom Track Order

Rename files with prefixes to control order:

```bash
# Original files
data/boss_theme.spc
data/credits.spc
data/title_screen.spc

# Rename for desired order
mv data/title_screen.spc data/01_title_screen.spc
mv data/boss_theme.spc data/02_boss_theme.spc
mv data/credits.spc data/03_credits.spc
```

## Track Information

Currently, track information is not displayed on-screen (minimal implementation). Track titles are stored in the ROM data and can be accessed programmatically.

### Future Features
- On-screen display (OSD) showing track title
- Visual playlist
- Progress bar
- Waveform visualization

## Troubleshooting

### No Sound
- **Check**: Volume on TV/monitor
- **Check**: Emulator audio settings
- **Try**: Press START to begin playback
- **Verify**: SPC files are valid

### Track Skipping Issues
- **Check**: SPC files converted properly
- **Verify**: Files in data/ before building

### ROM Won't Load
- **Check**: ROM size compatible with your setup
- **Try**: Different emulator
- **Verify**: ROM file not corrupted (check file size)

### Controls Not Working
- **Check**: Controller properly connected
- **Try**: Different controller
- **Emulator**: Check controller configuration
- **Hardware**: Test controller in another game

## Advanced Usage

### Memory Viewer

Use emulator debugging tools to view player state:

- **Current Track**: Check player state structure
- **SPC RAM**: View uploaded audio data
- **DSP Registers**: Monitor audio chip state

### Modifying Playback

Edit `src/main.c` to customize behavior:
- Auto-advance tracks
- Shuffle mode
- Repeat modes
- Custom button mappings

Example: Auto-advance to next track
```c
// After loading track, set a timer
// In main loop, check if track finished
// Automatically call spc_next_track()
```

## Best Practices

### SPC File Selection
1. Use high-quality SPC rips
2. Verify files play correctly in SPC player first
3. Keep total ROM size reasonable (<4MB)
4. Test each SPC file before adding to ROM

### Performance
- The SNES can handle playback without issues
- Upload time per track is minimal
- Switching tracks is near-instantaneous

### Compatibility
- Works on all SNES models (NTSC/PAL)
- Compatible with Super Famicom
- Works on most SNES emulators
- Compatible with flash carts

## File Locations

After building, you'll find:

```
build/
├── spc-player.sfc      # Final ROM (load this)
├── *.o                 # Object files (build artifacts)
├── *.s                 # Assembly files (intermediate)
├── spc2bin             # Converter tool
├── spcbundle           # Bundler tool
└── *.spc.bin           # Converted SPC files
```

Only `spc-player.sfc` is needed for running the player.

## Tips and Tricks

### Quick Testing
```bash
# Build with single track for quick testing
cp test.spc data/
make clean && make
# Test ROM
rm data/test.spc
```

### Batch Processing
```bash
# Convert all SPC files from another directory
for f in ~/spc_collection/*.spc; do
    cp "$f" data/
done
make
```

### ROM Size Management
```bash
# Check ROM size
ls -lh build/spc-player.sfc

# Estimate tracks per MB
# ~64KB per track = ~16 tracks per MB
```

## Examples

### Personal Music Collection
```bash
# Copy favorite game music
cp ~/music/chrono_trigger/*.spc data/
make
```

### Game Music Pack
```bash
# Create themed ROM (e.g., all RPG battle music)
cp battle_themes/*.spc data/
make
mv build/spc-player.sfc rpg_battles.sfc
```

### Demo/Showcase ROM
```bash
# Include diverse samples
cp showcase/orchestral.spc data/01_orchestral.spc
cp showcase/chiptune.spc data/02_chiptune.spc
cp showcase/ambient.spc data/03_ambient.spc
make
```

## Additional Resources

- **SPC Music Archive**: http://snesmusic.org/
- **SNES Dev Wiki**: https://wiki.superfamicom.org/
- **Emulator Downloads**: 
  - bsnes: https://bsnes.org/
  - Snes9x: http://www.snes9x.com/
  - Mesen-S: https://www.mesen.ca/

## Getting Help

If you encounter issues:
1. Check the troubleshooting section
2. Verify your SPC files are valid
3. Review the build output for errors
4. Test in multiple emulators
5. Check the project documentation

## Contributing

Have ideas for improvements? Contributions welcome:
- Better controller visualization
- On-screen display features
- Compression support
- Additional playback modes
