#!/bin/bash
# Build script for rtl8812au with Kernel 6.1.21+ support

# Clear any previous build artifacts
make clean

# Build the module
make

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "Build successful! Installing the driver..."
    sudo make install
    echo "If you want to load the driver now, run: sudo modprobe 8812au"
    echo "For DKMS installation, run: sudo make dkms_install"
else
    echo "Build failed. Please check the output for errors."
fi
