## Changes
2025-05-15: Updated to compile against kernel 6.1.21+
2019-07-11: Updated to compile against kernel 5.2

## Kernel 6.1.21+ Compatibility

This driver has been updated to work with Linux kernel version 6.1.21 and newer. The following changes were made:
- Added compatibility layer for memory barriers on newer AMD processors
- Updated network driver select_queue API for 6.1+ kernels
- Fixed IEEE80211_BAND to NL80211_BAND mapping for 6.1+ kernels
- Added support for other kernel API changes

### Simplified Build for Kernel 6.1+ 

```sh
$ ./build.sh
$ sudo modprobe 8812au
```

## Realtek 802.11ac (rtl8812au)

This is a fork of the Realtek 802.11ac (rtl8812au) v4.2.2 (7502.20130507)
driver altered to build on Linux kernel version >= 3.10.

### Purpose

My D-Link DWA-171 wireless dual-band USB adapter needs the Realtek 8812au
driver to work under Linux.

The current rtl8812au version (per nov. 20th 2013) doesn't compile on Linux
kernels >= 3.10 due to a change in the proc entry API, specifically the
deprecation of the `create_proc_entry()` and `create_proc_read_entry()`
functions in favor of the new `proc_create()` function.

### Building

The Makefile is preconfigured to handle most x86/PC versions.  If you are compiling for something other than an intel x86 architecture, you need to first select the platform, e.g. for the Raspberry Pi, you need to set the I386 to n and the ARM_RPI to y:
```sh
...
CONFIG_PLATFORM_I386_PC = n
...
CONFIG_PLATFORM_ARM_RPI = y
```

There are many other platforms supported and some other advanced options, e.g. PCI instead of USB, but most won't be needed.

The driver is built by running `make`, and can be tested by loading the
built module using `insmod`:

```sh
$ make
$ sudo insmod 8812au.ko
```

After loading the module, a wireless network interface named __Realtek 802.11n WLAN Adapter__ should be available.

### Installing

Installing the driver is simply a matter of copying the built module
into the correct location and updating module dependencies using `depmod`:

```sh
$ sudo cp 8812au.ko /lib/modules/$(uname -r)/kernel/drivers/net/wireless
$ sudo depmod
```

The driver module should now be loaded automatically.

### DKMS

Automatically rebuilds and installs on kernel updates. DKMS is in official sources of Ubuntu, for installation do:

```sh
$ sudo apt-get install build-essential dkms 
```

Install the driver to DKMS with:
```sh
sudo make dkms_install
```

Automatically load at boot:
```sh
$ echo 8812au | sudo tee -a /etc/modules
```

Eventually remove from DKMS with:
```sh
$ sudo make dkms_remove
```

### References

- D-Link DWA-171
  - [D-Link page](http://www.dlink.com/no/nb/home-solutions/connect/adapters/dwa-171-wireless-ac-dual-band-usb-adapter)
  - [wikidevi page](http://wikidevi.com/wiki/D-Link_DWA-171_rev_A1)
