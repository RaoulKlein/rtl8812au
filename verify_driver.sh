#!/bin/bash

# Script to verify rtl8812au driver for Netgear AC6100 USB WiFi dongle

echo "==================== DRIVER VERIFICATION ===================="

# Check if driver is loaded
echo "Checking if the 8812au driver is loaded..."
if lsmod | grep -q "8812au\|rtl8812au"; then
    echo "✓ Driver is loaded!"
else
    echo "✗ Driver is NOT loaded!"
    echo "  Try running 'sudo modprobe 8812au' or 'sudo modprobe rtl8812au'"
fi

# Check for WiFi interfaces
echo -e "\nChecking for wireless interfaces..."
WIFI_INTERFACES=$(iw dev | grep Interface | wc -l)
if [ "$WIFI_INTERFACES" -gt 0 ]; then
    echo "✓ Found $WIFI_INTERFACES wireless interface(s):"
    iw dev | grep -A 1 Interface
else
    echo "✗ No wireless interfaces found!"
fi

# Check USB devices
echo -e "\nChecking for USB WiFi device..."
if lsusb | grep -q "Realtek\|RTL8812\|Netgear AC6100"; then
    echo "✓ USB WiFi device found!"
    lsusb | grep -E "Realtek|RTL8812|Netgear"
else
    echo "✗ USB WiFi device NOT found!"
    echo "  Make sure your Netgear AC6100 USB WiFi dongle is properly connected"
fi

# Check kernel messages for driver
echo -e "\nChecking kernel messages for driver information..."
dmesg | grep -i "8812\|rtl\|wifi\|netgear" | tail -n 10

# Check for any UBSAN errors related to rtl8812au
echo -e "\nChecking for UBSAN errors related to the driver..."
if dmesg | grep -i "UBSAN.*rtl8812au\|UBSAN.*8812" > /dev/null; then
    echo "✗ UBSAN errors found:"
    dmesg | grep -i "UBSAN.*rtl8812au\|UBSAN.*8812" | tail -n 5
else
    echo "✓ No UBSAN errors found!"
fi

echo -e "\n==================== NETWORK STATUS ===================="

# Show iwconfig info
echo "Wireless configuration:"
iwconfig 2>/dev/null || echo "iwconfig not available"

# Show IP configuration
echo -e "\nIP configuration:"
ip a | grep -A 2 "wl\|wlan"

echo -e "\nVerification complete!"
