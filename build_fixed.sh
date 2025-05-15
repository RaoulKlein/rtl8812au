#!/bin/bash

# Script to apply fixes and build the rtl8812au driver for kernel 6.8.0-57-generic

set -e  # Exit on error

echo "Building rtl8812au driver with UBSAN fix for kernel 6.8.0-57-generic"

# Clean previous builds
echo "Cleaning previous builds..."
make clean

# Build the driver with multiple cores
echo "Building driver with $(nproc) cores..."
make -j$(nproc) 

echo -e "\n============== DRIVER BUILD COMPLETE ==============="
echo "To verify if the UBSAN error is fixed, reinstall the driver by running:"
echo "sudo ./install.sh"
echo "After installation, run the verification script:"
echo "./verify_driver.sh"
echo -e "\nIf no UBSAN errors are shown, the fix was successful!"
