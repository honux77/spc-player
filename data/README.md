# SPC Files Directory

Place your `.spc` files in this directory to include them in the ROM.

## Getting SPC Files

You can find SPC files from various sources:

1. **SNESMUSIC.ORG**: Large collection of SNES game music
   - http://snesmusic.org/

2. **Zophar's Domain**: Video game music archive
   - https://www.zophar.net/music/nintendo-snes-spc

3. **SNESMusic**: Another SNES music archive

## File Requirements

- Files must have `.spc` extension
- Must be valid SPC700 format files
- Minimum size: 66,048 bytes (0x10200)
- Files should start with "SNES-SPC700 Sound File Data"

## Usage

1. Copy or download `.spc` files to this directory
2. Run `make` from the project root
3. All `.spc` files found here will be included in the ROM

## Example

```bash
# Download or copy your SPC files
cp ~/Downloads/music.spc .
cp ~/Downloads/boss_theme.spc .
cp ~/Downloads/credits.spc .

# Build the ROM
cd ..
make

# The ROM will contain all three tracks
```

## Notes

- Track order is alphabetical by filename
- Each SPC file is approximately 64KB
- ROM size increases with each added track
- Maximum tracks limited by ROM size (typically ~60 tracks for 4MB ROM)
