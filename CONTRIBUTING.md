# Contributing to SNES SPC Player

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## Getting Started

1. Fork the repository
2. Clone your fork
3. Create a feature branch
4. Make your changes
5. Test thoroughly
6. Submit a pull request

## Development Setup

### Prerequisites
- GCC or compatible C compiler
- cc65 toolchain (for SNES compilation)
- Make
- Git
- SNES emulator (for testing)

### Building
```bash
# Clone repository
git clone https://github.com/honux77/spc-player.git
cd spc-player

# Build converter tools
make test-tools

# Build ROM (requires cc65)
make
```

## Code Style

### C Code
- Use 4 spaces for indentation
- K&R brace style
- Descriptive variable names
- Comment complex logic
- Maximum line length: 100 characters

Example:
```c
void spc_load(uint8_t track_num) {
    if (track_num >= player_state.num_tracks) {
        return;
    }
    
    player_state.current_track = track_num;
    spc_upload_state(&spc_tracks[track_num]);
}
```

### Assembly Code
- Use lowercase mnemonics
- Indent instructions with tabs
- Comment register usage
- Document side effects

Example:
```assembly
reset_handler:
    sei                     ; Disable interrupts
    clc
    xce                     ; Switch to native mode
```

## Project Structure

```
spc-player/
├── src/           # SNES source code
├── include/       # Header files
├── tools/         # Converter utilities
├── data/          # SPC files (not in repo)
├── docs/          # Documentation
└── build/         # Build output (not in repo)
```

## Making Changes

### Adding Features

1. **Plan your changes**
   - Consider impact on ROM size
   - Ensure SNES hardware compatibility
   - Check for existing similar features

2. **Implement incrementally**
   - Make small, focused commits
   - Test after each change
   - Update documentation

3. **Test thoroughly**
   - Test on multiple emulators
   - Verify with different SPC files
   - Check edge cases

### Bug Fixes

1. **Reproduce the bug**
   - Create minimal test case
   - Document steps to reproduce
   - Identify root cause

2. **Fix and test**
   - Make minimal changes
   - Add test if applicable
   - Verify fix works

3. **Document**
   - Explain the bug in commit message
   - Update relevant documentation

## Testing

### Converter Tools
```bash
# Test tools build
make test-tools

# Test conversion
./build/spc2bin test.spc test.bin

# Test bundling
./build/spcbundle output.c test.bin
```

### ROM Testing
- Test in bsnes (most accurate)
- Test in Snes9x (compatibility)
- Test basic playback
- Test all controller functions
- Test with various SPC files
- Test edge cases (0 tracks, many tracks)

## Documentation

### Code Comments
- Explain **why**, not **what**
- Document assumptions
- Note SNES-specific quirks
- Reference documentation sources

### Documentation Files
- Keep README.md up to date
- Update BUILD_GUIDE.md for build changes
- Update USAGE.md for feature changes
- Update TECHNICAL.md for architecture changes

## Pull Request Process

### Before Submitting

1. **Test your changes**
   - All existing functionality works
   - New features work as expected
   - No compiler warnings (if possible)

2. **Update documentation**
   - README if user-facing changes
   - Technical docs if architecture changes
   - Code comments for complex logic

3. **Clean commit history**
   - Meaningful commit messages
   - Squash work-in-progress commits
   - Logical grouping of changes

### PR Description

Include:
- **What**: What does this PR do?
- **Why**: Why is this change needed?
- **How**: How does it work?
- **Testing**: How was it tested?
- **Screenshots**: If UI changes (emulator screenshots)

Example:
```markdown
## Add Shuffle Mode

### What
Implements random track playback order

### Why
Users requested shuffle functionality for variety

### How
- Adds LFSR-based random number generator
- Shuffles track list on player init
- New button combo to toggle shuffle

### Testing
- Tested in bsnes with 10 tracks
- Verified randomness distribution
- Tested toggle functionality
```

## Areas for Contribution

### High Priority
- [ ] On-screen display (track info)
- [ ] Compression support
- [ ] Better error handling
- [ ] SRAM save support
- [ ] Fade in/out effects

### Medium Priority
- [ ] Shuffle mode
- [ ] Repeat modes
- [ ] Visual feedback
- [ ] Track seeking
- [ ] Playlist management

### Low Priority
- [ ] Waveform visualization
- [ ] Equalizer
- [ ] Custom themes
- [ ] Advanced DSP effects

## Coding Guidelines

### Memory Management
- Be careful with stack usage
- Minimize global variables
- Consider ROM/RAM tradeoffs
- Profile memory usage

### Performance
- SNES CPU is slow (3.58 MHz)
- Optimize hot paths
- Minimize VBlank work
- Use efficient algorithms

### Compatibility
- Test on NTSC and PAL
- Avoid timing dependencies
- Support all SNES models
- Work with flash carts

## Resources

### SNES Development
- [SNES Dev Wiki](https://wiki.superfamicom.org/)
- [cc65 Documentation](https://cc65.github.io/doc/)
- [bsnes Forum](https://board.byuu.org/)

### SPC Format
- [SPC File Format](http://snesmusic.org/files/spc_file_format.txt)
- [SPC700 Reference](https://wiki.superfamicom.org/spc700-reference)

### Tools
- [bsnes](https://bsnes.org/) - Best emulator
- [Mesen-S](https://www.mesen.ca/) - Great debugger
- [SPC Players](http://snesmusic.org/) - Test SPC files

## Getting Help

- **Issues**: Open an issue for bugs or questions
- **Discussions**: Use GitHub Discussions for ideas
- **Discord**: Join SNES dev community
- **IRC**: #snesdev on EFnet

## Code of Conduct

### Be Respectful
- Treat all contributors with respect
- Welcome newcomers
- Provide constructive feedback
- Help others learn

### Be Collaborative
- Share knowledge
- Credit others' work
- Work together on solutions
- Review PRs thoughtfully

## License

By contributing, you agree that your contributions will be licensed under the same license as the project.

## Recognition

Contributors will be acknowledged in:
- CONTRIBUTORS.md file
- Release notes
- Project README

Thank you for contributing to SNES SPC Player!
