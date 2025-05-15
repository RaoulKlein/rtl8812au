#!/bin/bash

# Script to build and install rtl8812au driver for kernel 6.8.0-57-generic
# For Netgear AC6100 USB WiFi dongle

set -e  # Exit on error

echo "Building and installing rtl8812au driver for kernel 6.8.0-57-generic"

# Check that we're running as root
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

# Install dependencies if needed
if ! dpkg -s linux-headers-$(uname -r) &> /dev/null; then
    echo "Installing linux headers..."
    apt-get update
    apt-get install -y linux-headers-$(uname -r) build-essential
fi

# Setup directory paths
DRIV_DIR=/lib/modules/$(uname -r)/kernel/drivers/net/wireless/realtek/rtlwifi/rtl8812au
ALT_DIR=/lib/modules/$(uname -r)/kernel/drivers/net/wireless

# Clean previous builds
echo "Cleaning previous builds..."
make clean

# Build the driver with multiple cores
echo "Building driver with $(nproc) cores..."
make -j$(nproc) 

# Install the driver
echo "Installing driver module..."
mkdir -p $DRIV_DIR
xz -c 8812au.ko > $DRIV_DIR/rtl8812au.ko.xz
mkdir -p $ALT_DIR
cp 8812au.ko $ALT_DIR/
depmod -a

echo "
                       ***Success***
***Module will be activated automatically at next reboot***
"

# Prompt to load now or at next boot
while true; do
    read -p "Do you wish to activate the module now? (y/n)" yn
    case $yn in
        [Yy]* ) 
            echo "Unloading any existing rtl8812au module..."
            rmmod 8812au 2>/dev/null || true
            echo "Loading new module..."
            modprobe 8812au || modprobe rtl8812au
            
            # Verify driver loaded successfully
            if lsmod | grep -q "8812au\|rtl8812au"; then
                echo "***Module activated successfully***"
                echo "You may need to reconnect your Netgear AC6100 USB WiFi dongle."
                echo "Use 'ip link' or 'iwconfig' to verify the WiFi interface is available."
            else
                echo "Warning: Failed to load the module. It will be loaded at next reboot."
            fi
            break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
    esac
done
