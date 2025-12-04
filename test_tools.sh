#!/bin/bash
# Test script for SPC converter tools

set -e

echo "=== SPC Player Tools Test ==="
echo

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Get script directory and cd to it
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# Build tools
echo "Building tools..."
make test-tools
echo

# Test spc2bin
echo "Testing spc2bin..."
if [ -f build/spc2bin ]; then
    echo -e "${GREEN}✓${NC} spc2bin built successfully"
    
    # Check help message
    if ./build/spc2bin 2>&1 | grep -q "Usage"; then
        echo -e "${GREEN}✓${NC} spc2bin help message works"
    else
        echo -e "${RED}✗${NC} spc2bin help message failed"
        exit 1
    fi
else
    echo -e "${RED}✗${NC} spc2bin not built"
    exit 1
fi
echo

# Test spcbundle
echo "Testing spcbundle..."
if [ -f build/spcbundle ]; then
    echo -e "${GREEN}✓${NC} spcbundle built successfully"
    
    # Check help message
    if ./build/spcbundle 2>&1 | grep -q "Usage"; then
        echo -e "${GREEN}✓${NC} spcbundle help message works"
    else
        echo -e "${RED}✗${NC} spcbundle help message failed"
        exit 1
    fi
else
    echo -e "${RED}✗${NC} spcbundle not built"
    exit 1
fi
echo

# Create test SPC file (minimal valid structure)
echo "Creating test SPC file..."
mkdir -p /tmp/spc_test

# Create a minimal valid SPC file for testing
python3 << 'EOF'
import struct
import os

# Create minimal SPC file
os.makedirs('/tmp/spc_test', exist_ok=True)
with open('/tmp/spc_test/test.spc', 'wb') as f:
    # Header (256 bytes)
    header = bytearray(256)
    
    # Magic string
    magic = b"SNES-SPC700 Sound File Data v0.30\x1a\x1a"
    header[0:len(magic)] = magic
    
    # Version
    header[0x21] = 0x1E  # version minor
    header[0x22] = 0x1A  # ID666 present
    header[0x23] = 0x1E  # version minor
    
    # CPU state
    header[0x24] = 0x12  # PC low
    header[0x25] = 0x34  # PC high
    header[0x26] = 0x00  # A
    header[0x27] = 0x00  # X
    header[0x28] = 0x00  # Y
    header[0x29] = 0x00  # PSW
    header[0x2A] = 0xEF  # SP
    
    # Song title
    title = b"Test Song"
    header[0x2E:0x2E+len(title)] = title
    
    f.write(header)
    
    # RAM (64KB)
    ram = bytearray(65536)
    # Add some test data
    ram[0:4] = b'\xCD\xEF\xBD\xE8'
    f.write(ram)
    
    # DSP registers (128 bytes)
    dsp = bytearray(128)
    f.write(dsp)
    
    # Extra RAM (64 bytes) - optional
    extra = bytearray(64)
    f.write(extra)

print("Created test.spc")
EOF

if [ -f /tmp/spc_test/test.spc ]; then
    echo -e "${GREEN}✓${NC} Test SPC file created"
    ls -lh /tmp/spc_test/test.spc
else
    echo -e "${RED}✗${NC} Failed to create test SPC file"
    exit 1
fi
echo

# Test spc2bin conversion
echo "Testing SPC to binary conversion..."
if ./build/spc2bin /tmp/spc_test/test.spc /tmp/spc_test/test.spc.bin; then
    echo -e "${GREEN}✓${NC} SPC conversion successful"
    
    if [ -f /tmp/spc_test/test.spc.bin ]; then
        echo -e "${GREEN}✓${NC} Binary file created"
        ls -lh /tmp/spc_test/test.spc.bin
    else
        echo -e "${RED}✗${NC} Binary file not created"
        exit 1
    fi
else
    echo -e "${RED}✗${NC} SPC conversion failed"
    exit 1
fi
echo

# Test spcbundle
echo "Testing SPC bundling..."
if ./build/spcbundle /tmp/spc_test/bundle.c /tmp/spc_test/test.spc.bin; then
    echo -e "${GREEN}✓${NC} SPC bundling successful"
    
    if [ -f /tmp/spc_test/bundle.c ]; then
        echo -e "${GREEN}✓${NC} Bundle C file created"
        ls -lh /tmp/spc_test/bundle.c
        
        # Check if file contains expected content
        if grep -q "spc_track_0" /tmp/spc_test/bundle.c && \
           grep -q "spc_tracks" /tmp/spc_test/bundle.c && \
           grep -q "spc_track_count" /tmp/spc_test/bundle.c; then
            echo -e "${GREEN}✓${NC} Bundle contains expected structures"
        else
            echo -e "${RED}✗${NC} Bundle missing expected structures"
            exit 1
        fi
    else
        echo -e "${RED}✗${NC} Bundle file not created"
        exit 1
    fi
else
    echo -e "${RED}✗${NC} SPC bundling failed"
    exit 1
fi
echo

# Clean up
echo "Cleaning up test files..."
rm -rf /tmp/spc_test
echo

echo "=== All Tests Passed ==="
echo -e "${GREEN}✓${NC} Tools are working correctly"
echo
echo "Next steps:"
echo "  1. Add .spc files to data/ directory"
echo "  2. Install cc65 toolchain"
echo "  3. Run 'make' to build the ROM"
