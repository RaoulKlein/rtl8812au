/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#define _HCI_INTF_C_

#include <drv_types.h>

#ifndef CONFIG_USB_HCI

#error "CONFIG_USB_HCI shall be on!\n"

#endif

#if defined (PLATFORM_LINUX) && defined (PLATFORM_WINDOWS)

#error "Shall be Linux or Windows, but not both!\n"

#endif

#ifdef CONFIG_80211N_HT
extern int rtw_ht_enable;
extern int rtw_bw_mode;
extern int rtw_ampdu_enable;//for enable tx_ampdu
#endif

#ifdef CONFIG_GLOBAL_UI_PID
int ui_pid[3] = {0, 0, 0};
#endif


extern int pm_netdev_open(struct net_device *pnetdev,u8 bnormal);
static int rtw_suspend(struct usb_interface *intf, pm_message_t message);
static int rtw_resume(struct usb_interface *intf);
int rtw_resume_process(_adapter *padapter);


static int rtw_drv_init(struct usb_interface *pusb_intf,const struct usb_device_id *pdid);
static void rtw_dev_remove(struct usb_interface *pusb_intf);

#if (LINUX_VERSION_CODE<=KERNEL_VERSION(2,6,23))
/* Some useful macros to use to create struct usb_device_id */
 #define USB_DEVICE_ID_MATCH_VENDOR 			 0x0001
 #define USB_DEVICE_ID_MATCH_PRODUCT			 0x0002
 #define USB_DEVICE_ID_MATCH_DEV_LO 			 0x0004
 #define USB_DEVICE_ID_MATCH_DEV_HI 			 0x0008
 #define USB_DEVICE_ID_MATCH_DEV_CLASS			 0x0010
 #define USB_DEVICE_ID_MATCH_DEV_SUBCLASS		 0x0020
 #define USB_DEVICE_ID_MATCH_DEV_PROTOCOL		 0x0040
 #define USB_DEVICE_ID_MATCH_INT_CLASS			 0x0080
 #define USB_DEVICE_ID_MATCH_INT_SUBCLASS		 0x0100
 #define USB_DEVICE_ID_MATCH_INT_PROTOCOL		 0x0200
 #define USB_DEVICE_ID_MATCH_INT_NUMBER 		 0x0400


#define USB_DEVICE_ID_MATCH_INT_INFO \
				 (USB_DEVICE_ID_MATCH_INT_CLASS | \
				 USB_DEVICE_ID_MATCH_INT_SUBCLASS | \
				 USB_DEVICE_ID_MATCH_INT_PROTOCOL)


#define USB_DEVICE_AND_INTERFACE_INFO(vend, prod, cl, sc, pr) \
		 .match_flags = USB_DEVICE_ID_MATCH_INT_INFO \
				 | USB_DEVICE_ID_MATCH_DEVICE, \
		 .idVendor = (vend), \
		 .idProduct = (prod), \
		 .bInterfaceClass = (cl), \
		 .bInterfaceSubClass = (sc), \
		 .bInterfaceProtocol = (pr)

 /**
  * USB_VENDOR_AND_INTERFACE_INFO - describe a specific usb vendor with a class of usb interfaces
  * @vend: the 16 bit USB Vendor ID
  * @cl: bInterfaceClass value
  * @sc: bInterfaceSubClass value
  * @pr: bInterfaceProtocol value
  *
  * This macro is used to create a struct usb_device_id that matches a
  * specific vendor with a specific class of interfaces.
  *
  * This is especially useful when explicitly matching devices that have
  * vendor specific bDeviceClass values, but standards-compliant interfaces.
  */
#define USB_VENDOR_AND_INTERFACE_INFO(vend, cl, sc, pr) \
		 .match_flags = USB_DEVICE_ID_MATCH_INT_INFO \
				 | USB_DEVICE_ID_MATCH_VENDOR, \
		 .idVendor = (vend), \
		 .bInterfaceClass = (cl), \
		 .bInterfaceSubClass = (sc), \
		 .bInterfaceProtocol = (pr)

/* ----------------------------------------------------------------------- */
#endif


#define USB_VENDER_ID_REALTEK		0x0BDA


/* DID_USB_v916_20130116 */
static struct usb_device_id rtw_usb_id_tbl[] ={

#ifdef CONFIG_RTL8192C
	/*=== Realtek demoboard ===*/
	/****** 8188CUS ********/
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8176),.driver_info = RTL8188C_8192C},/* 8188cu 1*1 dongole */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8170),.driver_info = RTL8188C_8192C},/* 8188CE-VAU USB minCard */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x817E),.driver_info = RTL8188C_8192C},/* 8188CE-VAU USB minCard */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x817A),.driver_info = RTL8188C_8192C},/* 8188cu Slim Solo */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x817B),.driver_info = RTL8188C_8192C},/* 8188cu Slim Combo */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x817D),.driver_info = RTL8188C_8192C},/* 8188RU High-power USB Dongle */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8754),.driver_info = RTL8188C_8192C},/* 8188 Combo for BC4 */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x817F),.driver_info = RTL8188C_8192C},/* 8188RU */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x818A),.driver_info = RTL8188C_8192C},/* RTL8188CUS-VL */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x018A),.driver_info = RTL8188C_8192C},/* RTL8188CTV */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x17C0),.driver_info = RTL8188C_8192C},/* RTK demoboard - USB-N10E */
	/****** 8192CUS ********/
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8177),.driver_info = RTL8188C_8192C},/* 8191cu 1*2 */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8178),.driver_info = RTL8188C_8192C},/* 8192cu 2*2 */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x817C),.driver_info = RTL8188C_8192C},/* 8192CE-VAU USB minCard */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8191),.driver_info = RTL8188C_8192C},/* 8192CU 2*2 */
	{USB_DEVICE(0x1058, 0x0631),.driver_info = RTL8188C_8192C},/* Alpha, 8192CU */
	/*=== Customer ID ===*/
	/****** 8188CUS Dongle ********/
	{USB_DEVICE(0x2019, 0xED17),.driver_info = RTL8188C_8192C},/* PCI - Edimax */
	{USB_DEVICE(0x0DF6, 0x0052),.driver_info = RTL8188C_8192C},/* Sitecom - Edimax */
	{USB_DEVICE(0x7392, 0x7811),.driver_info = RTL8188C_8192C},/* Edimax - Edimax */
	{USB_DEVICE(0x07B8, 0x8189),.driver_info = RTL8188C_8192C},/* Abocom - Abocom */
	{USB_DEVICE(0x0EB0, 0x9071),.driver_info = RTL8188C_8192C},/* NO Brand - Etop */
	{USB_DEVICE(0x06F8, 0xE033),.driver_info = RTL8188C_8192C},/* Hercules - Edimax */
	{USB_DEVICE(0x103C, 0x1629),.driver_info = RTL8188C_8192C},/* HP - Lite-On ,8188CUS Slim Combo */
	{USB_DEVICE(0x2001, 0x3308),.driver_info = RTL8188C_8192C},/* D-Link - Alpha */
	{USB_DEVICE(0x050D, 0x1102),.driver_info = RTL8188C_8192C},/* Belkin - Edimax */
	{USB_DEVICE(0x2019, 0xAB2A),.driver_info = RTL8188C_8192C},/* Planex - Abocom */
	{USB_DEVICE(0x20F4, 0x648B),.driver_info = RTL8188C_8192C},/* TRENDnet - Cameo */
	{USB_DEVICE(0x4855, 0x0090),.driver_info = RTL8188C_8192C},/*  - Feixun */
	{USB_DEVICE(0x13D3, 0x3357),.driver_info = RTL8188C_8192C},/*  - AzureWave */
	{USB_DEVICE(0x0DF6, 0x005C),.driver_info = RTL8188C_8192C},/* Sitecom - Edimax */
	{USB_DEVICE(0x0BDA, 0x5088),.driver_info = RTL8188C_8192C},/* Thinkware - CC&C */
	{USB_DEVICE(0x4856, 0x0091),.driver_info = RTL8188C_8192C},/* NetweeN - Feixun */
	{USB_DEVICE(0x0846, 0x9041),.driver_info = RTL8188C_8192C}, /* Netgear - Cameo */
	{USB_DEVICE(0x2019, 0x4902),.driver_info = RTL8188C_8192C},/* Planex - Etop */
	{USB_DEVICE(0x2019, 0xAB2E),.driver_info = RTL8188C_8192C},/* SW-WF02-AD15 -Abocom */
	{USB_DEVICE(0x2001, 0x330B),.driver_info = RTL8188C_8192C}, /* D-LINK - T&W */
	{USB_DEVICE(0xCDAB, 0x8010),.driver_info = RTL8188C_8192C}, /* - - compare */
	{USB_DEVICE(0x0B05, 0x17BA),.driver_info = RTL8188C_8192C}, /* ASUS - Edimax */
	{USB_DEVICE(0x0BDA, 0x1E1E),.driver_info = RTL8188C_8192C}, /* Intel - - */
	{USB_DEVICE(0x04BB, 0x094c),.driver_info = RTL8188C_8192C}, /* I-O DATA - Edimax */
	/****** 8188CTV ********/
	{USB_DEVICE(0xCDAB, 0x8011),.driver_info = RTL8188C_8192C}, /* - - compare */
	{USB_DEVICE(0x0BDA, 0x0A8A),.driver_info = RTL8188C_8192C}, /* Sony - Foxconn */
	/****** 8188 RU ********/
	{USB_DEVICE(0x0BDA, 0x317F),.driver_info = RTL8188C_8192C},/* Netcore,Netcore */
	/****** 8188CE-VAU ********/
	{USB_DEVICE(0x13D3, 0x3359),.driver_info = RTL8188C_8192C},/*  - Azwave */
	{USB_DEVICE(0x13D3, 0x3358),.driver_info = RTL8188C_8192C},/*  - Azwave */
	/****** 8188CUS Slim Solo********/
	{USB_DEVICE(0x04F2, 0xAFF7),.driver_info = RTL8188C_8192C},/* XAVI - XAVI */
	{USB_DEVICE(0x04F2, 0xAFF9),.driver_info = RTL8188C_8192C},/* XAVI - XAVI */
	{USB_DEVICE(0x04F2, 0xAFFA),.driver_info = RTL8188C_8192C},/* XAVI - XAVI */
	/****** 8188CUS Slim Combo ********/
	{USB_DEVICE(0x04F2, 0xAFF8),.driver_info = RTL8188C_8192C},/* XAVI - XAVI */
	{USB_DEVICE(0x04F2, 0xAFFB),.driver_info = RTL8188C_8192C},/* XAVI - XAVI */
	{USB_DEVICE(0x04F2, 0xAFFC),.driver_info = RTL8188C_8192C},/* XAVI - XAVI */
	{USB_DEVICE(0x2019, 0x1201),.driver_info = RTL8188C_8192C},/* Planex - Vencer */
	/****** 8192CUS Dongle ********/
	{USB_DEVICE(0x2001, 0x3307),.driver_info = RTL8188C_8192C},/* D-Link - Cameo */
	{USB_DEVICE(0x2001, 0x330A),.driver_info = RTL8188C_8192C},/* D-Link - Alpha */
	{USB_DEVICE(0x2001, 0x3309),.driver_info = RTL8188C_8192C},/* D-Link - Alpha */
	{USB_DEVICE(0x0586, 0x341F),.driver_info = RTL8188C_8192C},/* Zyxel - Abocom */
	{USB_DEVICE(0x7392, 0x7822),.driver_info = RTL8188C_8192C},/* Edimax - Edimax */
	{USB_DEVICE(0x2019, 0xAB2B),.driver_info = RTL8188C_8192C},/* Planex - Abocom */
	{USB_DEVICE(0x07B8, 0x8178),.driver_info = RTL8188C_8192C},/* Abocom - Abocom */
	{USB_DEVICE(0x07AA, 0x0056),.driver_info = RTL8188C_8192C},/* ATKK - Gemtek */
	{USB_DEVICE(0x4855, 0x0091),.driver_info = RTL8188C_8192C},/*  - Feixun */
	{USB_DEVICE(0x050D, 0x2102),.driver_info = RTL8188C_8192C},/* Belkin - Sercomm */
	{USB_DEVICE(0x050D, 0x2103),.driver_info = RTL8188C_8192C},/* Belkin - Edimax */
	{USB_DEVICE(0x20F4, 0x624D),.driver_info = RTL8188C_8192C},/* TRENDnet */
	{USB_DEVICE(0x0DF6, 0x0061),.driver_info = RTL8188C_8192C},/* Sitecom - Edimax */
	{USB_DEVICE(0x0B05, 0x17AB),.driver_info = RTL8188C_8192C},/* ASUS - Edimax */
	{USB_DEVICE(0x0846, 0x9021),.driver_info = RTL8188C_8192C},/* Netgear - Sercomm */
	{USB_DEVICE(0x0846, 0xF001),.driver_info = RTL8188C_8192C}, /* Netgear - Sercomm */
	{USB_DEVICE(0x0E66, 0x0019),.driver_info = RTL8188C_8192C},/* Hawking,Edimax */
	{USB_DEVICE(0x0E66, 0x0020),.driver_info = RTL8188C_8192C}, /* Hawking  - Edimax */
	{USB_DEVICE(0x050D, 0x1004),.driver_info = RTL8188C_8192C}, /* Belkin - Edimax */
	{USB_DEVICE(0x0BDA, 0x2E2E),.driver_info = RTL8188C_8192C}, /* Intel - - */
	{USB_DEVICE(0x2357, 0x0100),.driver_info = RTL8188C_8192C}, /* TP-Link - TP-Link */
	{USB_DEVICE(0x06F8, 0xE035),.driver_info = RTL8188C_8192C}, /* Hercules - Edimax */
	{USB_DEVICE(0x04BB, 0x0950),.driver_info = RTL8188C_8192C}, /* IO-DATA - Edimax */
	{USB_DEVICE(0x0DF6, 0x0070),.driver_info = RTL8188C_8192C}, /* Sitecom - Edimax */
	{USB_DEVICE(0x0789, 0x016D),.driver_info = RTL8188C_8192C}, /* LOGITEC - Edimax */
	/****** 8192CE-VAU  ********/
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8186),.driver_info = RTL8188C_8192C},/* Intel-Xavi( Azwave) */
#endif

#ifdef CONFIG_RTL8192D
	/*=== Realtek demoboard ===*/
	/****** 8192DU ********/
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8193),.driver_info = RTL8192D},/* 8192DU-VC */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8194),.driver_info = RTL8192D},/* 8192DU-VS */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8111),.driver_info = RTL8192D},/* Realtek 5G dongle for WiFi Display */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x0193),.driver_info = RTL8192D},/* 8192DE-VAU */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8171),.driver_info = RTL8192D},/* 8192DU-VC */
	/*=== Customer ID ===*/
	/****** 8192DU-VC ********/
	{USB_DEVICE(0x2019, 0xAB2C),.driver_info = RTL8192D},/* PCI - Abocm */
	{USB_DEVICE(0x2019, 0x4903),.driver_info = RTL8192D},/* PCI - ETOP */
	{USB_DEVICE(0x2019, 0x4904),.driver_info = RTL8192D},/* PCI - ETOP */
	{USB_DEVICE(0x07B8, 0x8193),.driver_info = RTL8192D},/* Abocom - Abocom */
	/****** 8192DU-VS ********/
	{USB_DEVICE(0x20F4, 0x664B),.driver_info = RTL8192D},/* TRENDnet */
	{USB_DEVICE(0x04DD, 0x954F),.driver_info = RTL8192D},  /* Sharp */
	{USB_DEVICE(0x04DD, 0x96A6),.driver_info = RTL8192D},  /* Sharp */
	{USB_DEVICE(0x050D, 0x110A),.driver_info = RTL8192D}, /* Belkin - Edimax */
	{USB_DEVICE(0x050D, 0x1105),.driver_info = RTL8192D}, /* Belkin - Edimax */
	{USB_DEVICE(0x050D, 0x120A),.driver_info = RTL8192D}, /* Belkin - Edimax */
	{USB_DEVICE(0x1668, 0x8102),.driver_info = RTL8192D}, /*  -  */
	{USB_DEVICE(0x0BDA, 0xE194),.driver_info = RTL8192D}, /*  - Edimax */
	/****** 8192DU-WiFi Display Dongle ********/
	{USB_DEVICE(0x2019, 0xAB2D),.driver_info = RTL8192D},/* Planex - Abocom ,5G dongle for WiFi Display */
#endif

#ifdef CONFIG_RTL8723A
	{USB_DEVICE_AND_INTERFACE_INFO(USB_VENDER_ID_REALTEK, 0x8724,0xff,0xff,0xff),.driver_info = RTL8723A}, /* 8723AU 1*1 */
	{USB_DEVICE_AND_INTERFACE_INFO(USB_VENDER_ID_REALTEK, 0x1724,0xff,0xff,0xff),.driver_info = RTL8723A}, /* 8723AU 1*1 */
	{USB_DEVICE_AND_INTERFACE_INFO(USB_VENDER_ID_REALTEK, 0x0724,0xff,0xff,0xff),.driver_info = RTL8723A}, /* 8723AU 1*1 */
#endif

#ifdef CONFIG_RTL8188E
	/*=== Realtek demoboard ===*/
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8179),.driver_info = RTL8188E}, /* 8188EUS */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x0179),.driver_info = RTL8188E}, /* 8188ETV */
	/*=== Customer ID ===*/
	/****** 8188EUS ********/
	{USB_DEVICE(0x07B8, 0x8179),.driver_info = RTL8188E}, /* Abocom - Abocom */
#endif

#ifdef CONFIG_RTL8812A
	/*=== Realtek demoboard ===*/
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8812),.driver_info = RTL8812},/* Default ID */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x881A),.driver_info = RTL8812},/* Default ID */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x881B),.driver_info = RTL8812},/* Default ID */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x881C),.driver_info = RTL8812},/* Default ID */
	/*=== Customer ID ===*/
	{USB_DEVICE(0x050D, 0x1106),.driver_info = RTL8812}, /* Belkin - sercomm */
	{USB_DEVICE(0x2604, 0x0012),.driver_info = RTL8812}, /* Tenda - U12 */
	{USB_DEVICE(0x050D, 0x1109),.driver_info = RTL8812}, /* Belkin F9L1109 - SerComm */
	{USB_DEVICE(0x2001, 0x330E),.driver_info = RTL8812}, /* D-Link - ALPHA */
	{USB_DEVICE(0x7392, 0xA822),.driver_info = RTL8812}, /* Edimax - Edimax */
	{USB_DEVICE(0x0DF6, 0x0074),.driver_info = RTL8812}, /* Sitecom - Edimax */
	{USB_DEVICE(0x04BB, 0x0952),.driver_info = RTL8812}, /* I-O DATA - Edimax */
	{USB_DEVICE(0x0789, 0x016E),.driver_info = RTL8812}, /* Logitec - Edimax */
	{USB_DEVICE(0x0409, 0x0408),.driver_info = RTL8812}, /* NEC - */
	{USB_DEVICE(0x0B05, 0x17D2),.driver_info = RTL8812}, /* ASUS - Edimax */
	{USB_DEVICE(0x0E66, 0x0022),.driver_info = RTL8812}, /* HAWKING - Edimax */
	{USB_DEVICE(0x0586, 0x3426),.driver_info = RTL8812}, /* ZyXEL - */
	{USB_DEVICE(0x2001, 0x3313),.driver_info = RTL8812}, /* D-Link - ALPHA */
	{USB_DEVICE(0x1058, 0x0632),.driver_info = RTL8812}, /* WD - Cybertan*/
	{USB_DEVICE(0x13b1, 0x003f),.driver_info = RTL8812}, /* Linksys WUSB6300 */
	{USB_DEVICE(0x1740, 0x0100),.driver_info = RTL8812}, /* EnGenius - EnGenius */
	{USB_DEVICE(0x2019, 0xAB30),.driver_info = RTL8812}, /* Planex - Abocom */
	{USB_DEVICE(0x07B8, 0x8812),.driver_info = RTL8812}, /* Abocom - Abocom */
	{USB_DEVICE(0x2001, 0x3315),.driver_info = RTL8812}, /* D-Link - Cameo */
	{USB_DEVICE(0x2001, 0x3316),.driver_info = RTL8812}, /* D-Link - Cameo */
	{USB_DEVICE(0x20f4, 0x805b),.driver_info = RTL8812}, /* TRENDnet - */
	{USB_DEVICE(0x2357, 0x0101),.driver_info = RTL8812}, /* TP-Link - Archer T4U, AC1200 */
	{USB_DEVICE(0x2357, 0x0103),.driver_info = RTL8812}, /* TP-Link - Archer T4UH(US), AC1200 */
	{USB_DEVICE(0x2357, 0x010d),.driver_info = RTL8812}, /* TP-Link - Archer T4U, AC1300 */
	{USB_DEVICE(0x2357, 0x010e),.driver_info = RTL8812}, /* TP-Link - Archer T4UH, AC1300 */
	{USB_DEVICE(0x2357, 0x0122),.driver_info = RTL8812}, /* TP-Link - Archer T4UHP, AC1300 */
	{USB_DEVICE(0x0411, 0x025d),.driver_info = RTL8812}, /* Buffalo - WI-U3-866D */
	{USB_DEVICE(0x0bda, 0x8812),.driver_info = RTL8812}, /* Netis WF2190 */
	{USB_DEVICE(0x2604, 0x0012),.driver_info = RTL8812}, /* Tenda U12 */
#endif

#ifdef CONFIG_RTL8821A
        /*=== Realtek demoboard ===*/
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x0811),.driver_info = RTL8821},/* Default ID */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x0821),.driver_info = RTL8821},/* Default ID */
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8822),.driver_info = RTL8821},/* Default ID */
	/*=== Customer ID ===*/
	{USB_DEVICE(0x7392, 0xA811),.driver_info = RTL8821}, /* Edimax - Edimax */
	{USB_DEVICE(0x7392, 0xA812),.driver_info = RTL8821}, /* Edimax - EW-7811UTC */
	{USB_DEVICE(0x7392, 0xA813),.driver_info = RTL8821}, /* Edimax - EW-7811UAC */
	{USB_DEVICE(0x2001, 0x3314),.driver_info = RTL8821}, /* D-Link - Cameo */
	{USB_DEVICE(0x0846, 0x9052),.driver_info = RTL8821}, /* Netgear - A6100 */
	{USB_DEVICE(0x2019, 0xAB32),.driver_info = RTL8821}, /* Planex - GW-450S */
	{USB_DEVICE(0x0411, 0x0242),.driver_info = RTL8821}, /* BUFFALO - Edimax */
	{USB_DEVICE(0x0411, 0x029B),.driver_info = RTL8821}, /* BUFFALO - WI-U2-433DHP */
	{USB_DEVICE(0x056E, 0x4007),.driver_info = RTL8821}, /* ELECOM - WDC-433DU2H */
	{USB_DEVICE(0x04BB, 0x0953),.driver_info = RTL8821}, /* I-O DATA - WN-AC433UA */
	{USB_DEVICE(0x04BB, 0x0959),.driver_info = RTL8821}, /* I-O DATA - WN-AC433UMK */
	{USB_DEVICE(0x0BDA, 0xA811),.driver_info = RTL8821}, /* GMYLE - AC450 */
	{USB_DEVICE(0x2001, 0x3318),.driver_info = RTL8821}, /* D-Link - DWA-172 */
	{USB_DEVICE(0x3823, 0x6249),.driver_info = RTL8821}, /* Obihai - OBiWiFi */
	{USB_DEVICE(0x0bda, 0x0823),.driver_info = RTL8821}, /* I-O DATA - WNPU583B */
	{USB_DEVICE(0x2357, 0x011e),.driver_info = RTL8821}, /* TP-Link - Archer T2U Nano, AC600 */
	{USB_DEVICE(0x2357, 0x0120),.driver_info = RTL8821}, /* TP-Link - Archer T2U Plus, AC600 */
#endif

#ifdef CONFIG_RTL8192E
	/*=== Realtek demoboard ===*/
	{USB_DEVICE(USB_VENDER_ID_REALTEK, 0x818B),.driver_info = RTL8192E},/* Default ID */
#endif
	{}	/* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, rtw_usb_id_tbl);

int const rtw_usb_id_len = sizeof(rtw_usb_id_tbl) / sizeof(struct usb_device_id);

static struct specific_device_id specific_device_id_tbl[] = {
	{.idVendor=USB_VENDER_ID_REALTEK, .idProduct=0x8177, .flags=SPEC_DEV_ID_DISABLE_HT},//8188cu 1*1 dongole, (b/g mode only)
	{.idVendor=USB_VENDER_ID_REALTEK, .idProduct=0x817E, .flags=SPEC_DEV_ID_DISABLE_HT},//8188CE-VAU USB minCard (b/g mode only)
	{.idVendor=0x0b05, .idProduct=0x1791, .flags=SPEC_DEV_ID_DISABLE_HT},
	{.idVendor=0x13D3, .idProduct=0x3311, .flags=SPEC_DEV_ID_DISABLE_HT},
	{.idVendor=0x13D3, .idProduct=0x3359, .flags=SPEC_DEV_ID_DISABLE_HT},//Russian customer -Azwave (8188CE-VAU  g mode)
#ifdef RTK_DMP_PLATFORM
	{.idVendor=USB_VENDER_ID_REALTEK, .idProduct=0x8111, .flags=SPEC_DEV_ID_ASSIGN_IFNAME}, // Realtek 5G dongle for WiFi Display
	{.idVendor=0x2019, .idProduct=0xAB2D, .flags=SPEC_DEV_ID_ASSIGN_IFNAME}, // PCI-Abocom 5G dongle for WiFi Display
#endif /* RTK_DMP_PLATFORM */
	{}
};

struct rtw_usb_drv {
	struct usb_driver usbdrv;
	int drv_registered;
	u8 hw_type;
};

struct rtw_usb_drv usb_drv = {
	.usbdrv.name =(char*)DRV_NAME,
	.usbdrv.probe = rtw_drv_init,
	.usbdrv.disconnect = rtw_dev_remove,
	.usbdrv.id_table = rtw_usb_id_tbl,
	.usbdrv.suspend =  rtw_suspend,
	.usbdrv.resume = rtw_resume,
	#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 22))
  	.usbdrv.reset_resume   = rtw_resume,
	#endif
	#ifdef CONFIG_AUTOSUSPEND
	.usbdrv.supports_autosuspend = 1,
	#endif
};

static inline int RT_usb_endpoint_dir_in(const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN);
}

static inline int RT_usb_endpoint_dir_out(const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT);
}

static inline int RT_usb_endpoint_xfer_int(const struct usb_endpoint_descriptor *epd)
{
	return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT);
}

static inline int RT_usb_endpoint_xfer_bulk(const struct usb_endpoint_descriptor *epd)
{
 	return ((epd->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK);
}

static inline int RT_usb_endpoint_is_bulk_in(const struct usb_endpoint_descriptor *epd)
{
	return (RT_usb_endpoint_xfer_bulk(epd) && RT_usb_endpoint_dir_in(epd));
}

static inline int RT_usb_endpoint_is_bulk_out(const struct usb_endpoint_descriptor *epd)
{
	return (RT_usb_endpoint_xfer_bulk(epd) && RT_usb_endpoint_dir_out(epd));
}

static inline int RT_usb_endpoint_is_int_in(const struct usb_endpoint_descriptor *epd)
{
	return (RT_usb_endpoint_xfer_int(epd) && RT_usb_endpoint_dir_in(epd));
}

static inline int RT_usb_endpoint_num(const struct usb_endpoint_descriptor *epd)
{
	return epd->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
}

static u8 rtw_init_intf_priv(struct dvobj_priv *dvobj)
{
	u8 rst = _SUCCESS;

	#ifdef CONFIG_USB_VENDOR_REQ_MUTEX
	_rtw_mutex_init(&dvobj->usb_vendor_req_mutex);
	#endif


	#ifdef CONFIG_USB_VENDOR_REQ_BUFFER_PREALLOC
	dvobj->usb_alloc_vendor_req_buf = rtw_zmalloc(MAX_USB_IO_CTL_SIZE);
	if (dvobj->usb_alloc_vendor_req_buf == NULL) {
		DBG_871X("alloc usb_vendor_req_buf failed... /n");
		rst = _FAIL;
		goto exit;
	}
	dvobj->usb_vendor_req_buf  =
		(u8 *)N_BYTE_ALIGMENT((SIZE_PTR)(dvobj->usb_alloc_vendor_req_buf ), ALIGNMENT_UNIT);
exit:
	#endif

	return rst;

}

static u8 rtw_deinit_intf_priv(struct dvobj_priv *dvobj)
{
	u8 rst = _SUCCESS;

	#ifdef CONFIG_USB_VENDOR_REQ_BUFFER_PREALLOC
	if(dvobj->usb_vendor_req_buf)
		rtw_mfree(dvobj->usb_alloc_vendor_req_buf, MAX_USB_IO_CTL_SIZE);
	#endif

	#ifdef CONFIG_USB_VENDOR_REQ_MUTEX
	_rtw_mutex_free(&dvobj->usb_vendor_req_mutex);
	#endif

	return rst;
}

static struct dvobj_priv *usb_dvobj_init(struct usb_interface *usb_intf)
{
	int	i;
	u8	val8;
	int	status = _FAIL;
	struct dvobj_priv *pdvobjpriv;
	struct usb_device_descriptor 	*pdev_desc;
	struct usb_host_config			*phost_conf;
	struct usb_config_descriptor		*pconf_desc;
	struct usb_host_interface		*phost_iface;
	struct usb_interface_descriptor	*piface_desc;
	struct usb_host_endpoint		*phost_endp;
	struct usb_endpoint_descriptor	*pendp_desc;
	struct usb_device				*pusbd;

_func_enter_;

	if ((pdvobjpriv = (struct dvobj_priv*)rtw_zmalloc(sizeof(*pdvobjpriv))) == NULL) {
		goto exit;
	}

	_rtw_mutex_init(&pdvobjpriv->hw_init_mutex);
	_rtw_mutex_init(&pdvobjpriv->h2c_fwcmd_mutex);
	_rtw_mutex_init(&pdvobjpriv->setch_mutex);
	_rtw_mutex_init(&pdvobjpriv->setbw_mutex);

	_rtw_spinlock_init(&pdvobjpriv->lock);

	pdvobjpriv->macid[1] = _TRUE; //macid=1 for bc/mc stainfo


	pdvobjpriv->pusbintf = usb_intf ;
	pusbd = pdvobjpriv->pusbdev = interface_to_usbdev(usb_intf);
	usb_set_intfdata(usb_intf, pdvobjpriv);

	pdvobjpriv->RtNumInPipes = 0;
	pdvobjpriv->RtNumOutPipes = 0;

	//padapter->EepromAddressSize = 6;
	//pdvobjpriv->nr_endpoint = 6;

	pdev_desc = &pusbd->descriptor;

#if 0
	DBG_871X("\n8712_usb_device_descriptor:\n");
	DBG_871X("bLength=%x\n", pdev_desc->bLength);
	DBG_871X("bDescriptorType=%x\n", pdev_desc->bDescriptorType);
	DBG_871X("bcdUSB=%x\n", pdev_desc->bcdUSB);
	DBG_871X("bDeviceClass=%x\n", pdev_desc->bDeviceClass);
	DBG_871X("bDeviceSubClass=%x\n", pdev_desc->bDeviceSubClass);
	DBG_871X("bDeviceProtocol=%x\n", pdev_desc->bDeviceProtocol);
	DBG_871X("bMaxPacketSize0=%x\n", pdev_desc->bMaxPacketSize0);
	DBG_871X("idVendor=%x\n", pdev_desc->idVendor);
	DBG_871X("idProduct=%x\n", pdev_desc->idProduct);
	DBG_871X("bcdDevice=%x\n", pdev_desc->bcdDevice);
	DBG_871X("iManufacturer=%x\n", pdev_desc->iManufacturer);
	DBG_871X("iProduct=%x\n", pdev_desc->iProduct);
	DBG_871X("iSerialNumber=%x\n", pdev_desc->iSerialNumber);
	DBG_871X("bNumConfigurations=%x\n", pdev_desc->bNumConfigurations);
#endif

	phost_conf = pusbd->actconfig;
	pconf_desc = &phost_conf->desc;

#if 0
	DBG_871X("\n8712_usb_configuration_descriptor:\n");
	DBG_871X("bLength=%x\n", pconf_desc->bLength);
	DBG_871X("bDescriptorType=%x\n", pconf_desc->bDescriptorType);
	DBG_871X("wTotalLength=%x\n", pconf_desc->wTotalLength);
	DBG_871X("bNumInterfaces=%x\n", pconf_desc->bNumInterfaces);
	DBG_871X("bConfigurationValue=%x\n", pconf_desc->bConfigurationValue);
	DBG_871X("iConfiguration=%x\n", pconf_desc->iConfiguration);
	DBG_871X("bmAttributes=%x\n", pconf_desc->bmAttributes);
	DBG_871X("bMaxPower=%x\n", pconf_desc->bMaxPower);
#endif

	//DBG_871X("\n/****** num of altsetting = (%d) ******/\n", pusb_interface->num_altsetting);

	phost_iface = &usb_intf->altsetting[0];
	piface_desc = &phost_iface->desc;

#if 0
	DBG_871X("\n8712_usb_interface_descriptor:\n");
	DBG_871X("bLength=%x\n", piface_desc->bLength);
	DBG_871X("bDescriptorType=%x\n", piface_desc->bDescriptorType);
	DBG_871X("bInterfaceNumber=%x\n", piface_desc->bInterfaceNumber);
	DBG_871X("bAlternateSetting=%x\n", piface_desc->bAlternateSetting);
	DBG_871X("bNumEndpoints=%x\n", piface_desc->bNumEndpoints);
	DBG_871X("bInterfaceClass=%x\n", piface_desc->bInterfaceClass);
	DBG_871X("bInterfaceSubClass=%x\n", piface_desc->bInterfaceSubClass);
	DBG_871X("bInterfaceProtocol=%x\n", piface_desc->bInterfaceProtocol);
	DBG_871X("iInterface=%x\n", piface_desc->iInterface);
#endif

	pdvobjpriv->NumInterfaces = pconf_desc->bNumInterfaces;
	pdvobjpriv->InterfaceNumber = piface_desc->bInterfaceNumber;
	pdvobjpriv->nr_endpoint = piface_desc->bNumEndpoints;

	//DBG_871X("\ndump usb_endpoint_descriptor:\n");

	for (i = 0; i < pdvobjpriv->nr_endpoint; i++)
	{
		phost_endp = phost_iface->endpoint + i;
		if (phost_endp)
		{
			pendp_desc = &phost_endp->desc;

			DBG_871X("\nusb_endpoint_descriptor(%d):\n", i);
			DBG_871X("bLength=%x\n",pendp_desc->bLength);
			DBG_871X("bDescriptorType=%x\n",pendp_desc->bDescriptorType);
			DBG_871X("bEndpointAddress=%x\n",pendp_desc->bEndpointAddress);
			//DBG_871X("bmAttributes=%x\n",pendp_desc->bmAttributes);
			DBG_871X("wMaxPacketSize=%d\n",le16_to_cpu(pendp_desc->wMaxPacketSize));
			DBG_871X("bInterval=%x\n",pendp_desc->bInterval);
			//DBG_871X("bRefresh=%x\n",pendp_desc->bRefresh);
			//DBG_871X("bSynchAddress=%x\n",pendp_desc->bSynchAddress);

			if (RT_usb_endpoint_is_bulk_in(pendp_desc))
			{
				DBG_871X("RT_usb_endpoint_is_bulk_in = %x\n", RT_usb_endpoint_num(pendp_desc));
				pdvobjpriv->RtInPipe[pdvobjpriv->RtNumInPipes] = RT_usb_endpoint_num(pendp_desc);
				pdvobjpriv->RtNumInPipes++;
			}
			else if (RT_usb_endpoint_is_int_in(pendp_desc))
			{
				DBG_871X("RT_usb_endpoint_is_int_in = %x, Interval = %x\n", RT_usb_endpoint_num(pendp_desc),pendp_desc->bInterval);
				pdvobjpriv->RtInPipe[pdvobjpriv->RtNumInPipes] = RT_usb_endpoint_num(pendp_desc);
				pdvobjpriv->RtNumInPipes++;
			}
			else if (RT_usb_endpoint_is_bulk_out(pendp_desc))
			{
				DBG_871X("RT_usb_endpoint_is_bulk_out = %x\n", RT_usb_endpoint_num(pendp_desc));
				pdvobjpriv->RtOutPipe[pdvobjpriv->RtNumOutPipes] = RT_usb_endpoint_num(pendp_desc);
				pdvobjpriv->RtNumOutPipes++;
			}
			// Prevent array index out of bounds (ep_num has 6 elements)
			if (i < 6) {
				pdvobjpriv->ep_num[i] = RT_usb_endpoint_num(pendp_desc);
			} else {
				DBG_871X("Warning: ep_num array index %d out of bounds (max 5)\n", i);
			}
		}
	}

	DBG_871X("nr_endpoint=%d, in_num=%d, out_num=%d\n\n", pdvobjpriv->nr_endpoint, pdvobjpriv->RtNumInPipes, pdvobjpriv->RtNumOutPipes);

	switch(pusbd->speed) {
		case USB_SPEED_LOW:
			DBG_871X("USB_SPEED_LOW\n");
			pdvobjpriv->usb_speed = RTW_USB_SPEED_1_1;
			break;
		case USB_SPEED_FULL:
			DBG_871X("USB_SPEED_FULL\n");
			pdvobjpriv->usb_speed = RTW_USB_SPEED_1_1;
			break;
		case USB_SPEED_HIGH:
			DBG_871X("USB_SPEED_HIGH\n");
			pdvobjpriv->usb_speed = RTW_USB_SPEED_2;
			break;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31))
		case USB_SPEED_SUPER:
			DBG_871X("USB_SPEED_SUPER\n");
			pdvobjpriv->usb_speed = RTW_USB_SPEED_3;
			break;
#endif
		default:
			DBG_871X("USB_SPEED_UNKNOWN(%x)\n",pusbd->speed);
			pdvobjpriv->usb_speed = RTW_USB_SPEED_UNKNOWN;
			break;
	}

	if (pdvobjpriv->usb_speed == RTW_USB_SPEED_UNKNOWN) {
		DBG_871X("UNKNOWN USB SPEED MODE, ERROR !!!\n");
		goto free_dvobj;
	}

	if (rtw_init_intf_priv(pdvobjpriv) == _FAIL) {
		RT_TRACE(_module_os_intfs_c_,_drv_err_,("\n Can't INIT rtw_init_intf_priv\n"));
		goto free_dvobj;
	}

	//.3 misc
	_rtw_init_sema(&(pdvobjpriv->usb_suspend_sema), 0);
	rtw_reset_continual_urb_error(pdvobjpriv);

	usb_get_dev(pusbd);

	status = _SUCCESS;

free_dvobj:
	if (status != _SUCCESS && pdvobjpriv) {
		usb_set_intfdata(usb_intf, NULL);
		_rtw_spinlock_free(&pdvobjpriv->lock);
		_rtw_mutex_free(&pdvobjpriv->hw_init_mutex);
		_rtw_mutex_free(&pdvobjpriv->h2c_fwcmd_mutex);
		_rtw_mutex_free(&pdvobjpriv->setch_mutex);
		_rtw_mutex_free(&pdvobjpriv->setbw_mutex);
		rtw_mfree((u8*)pdvobjpriv, sizeof(*pdvobjpriv));
		pdvobjpriv = NULL;
	}
exit:
_func_exit_;
	return pdvobjpriv;
}

static void usb_dvobj_deinit(struct usb_interface *usb_intf)
{
	struct dvobj_priv *dvobj = usb_get_intfdata(usb_intf);

_func_enter_;

	usb_set_intfdata(usb_intf, NULL);
	if (dvobj) {
		//Modify condition for 92DU DMDP 2010.11.18, by Thomas
		if ((dvobj->NumInterfaces != 2 && dvobj->NumInterfaces != 3)
			|| (dvobj->InterfaceNumber == 1)) {
			if (interface_to_usbdev(usb_intf)->state != USB_STATE_NOTATTACHED) {
				//If we didn't unplug usb dongle and remove/insert modlue, driver fails on sitesurvey for the first time when device is up .
				//Reset usb port for sitesurvey fail issue. 2009.8.13, by Thomas
				DBG_871X("usb attached..., try to reset usb device\n");
				usb_reset_device(interface_to_usbdev(usb_intf));
			}
		}
		rtw_deinit_intf_priv(dvobj);
		_rtw_spinlock_free(&dvobj->lock);
		_rtw_mutex_free(&dvobj->hw_init_mutex);
		_rtw_mutex_free(&dvobj->h2c_fwcmd_mutex);
		_rtw_mutex_free(&dvobj->setch_mutex);
		_rtw_mutex_free(&dvobj->setbw_mutex);
		rtw_mfree((u8*)dvobj, sizeof(*dvobj));
	}

	//DBG_871X("%s %d\n", __func__, ATOMIC_READ(&usb_intf->dev.kobj.kref.refcount));
	usb_put_dev(interface_to_usbdev(usb_intf));

_func_exit_;
}

static void rtw_decide_chip_type_by_usb_info(_adapter *padapter, const struct usb_device_id *pdid)
{
	padapter->chip_type = pdid->driver_info;

	#ifdef CONFIG_RTL8192C
	if(padapter->chip_type == RTL8188C_8192C)
		rtl8192cu_set_hw_type(padapter);
	#endif

	#ifdef CONFIG_RTL8192D
	if(padapter->chip_type == RTL8192D)
		rtl8192du_set_hw_type(padapter);
	#endif

	#ifdef CONFIG_RTL8723A
	if(padapter->chip_type == RTL8723A)
		rtl8723au_set_hw_type(padapter);
	#endif

	#ifdef CONFIG_RTL8188E
	if(padapter->chip_type == RTL8188E)
		rtl8188eu_set_hw_type(padapter);
	#endif

	#if defined(CONFIG_RTL8812A) || defined(CONFIG_RTL8821A)
	if(padapter->chip_type == RTL8812 || padapter->chip_type == RTL8821)
		rtl8812au_set_hw_type(padapter);
	#endif

	#ifdef CONFIG_RTL8192E
	if(padapter->chip_type == RTL8192E)
		rtl8192eu_set_hw_type(padapter);
	#endif

}
void rtw_set_hal_ops(_adapter *padapter)
{
	#ifdef CONFIG_RTL8192C
	if(padapter->chip_type == RTL8188C_8192C)
		rtl8192cu_set_hal_ops(padapter);
	#endif

	#ifdef CONFIG_RTL8192D
	if(padapter->chip_type == RTL8192D)
		rtl8192du_set_hal_ops(padapter);
	#endif

	#ifdef CONFIG_RTL8723A
	if(padapter->chip_type == RTL8723A)
		rtl8723au_set_hal_ops(padapter);
	#endif

	#ifdef CONFIG_RTL8188E
	if(padapter->chip_type == RTL8188E)
		rtl8188eu_set_hal_ops(padapter);
	#endif

	#if defined(CONFIG_RTL8812A) || defined(CONFIG_RTL8821A)
	if(padapter->chip_type == RTL8812 || padapter->chip_type == RTL8821)
		rtl8812au_set_hal_ops(padapter);
	#endif

	#ifdef CONFIG_RTL8192E
	if(padapter->chip_type == RTL8192E)
		rtl8192eu_set_hal_ops(padapter);
	#endif
}

void usb_set_intf_ops(_adapter *padapter,struct _io_ops *pops)
{
	#ifdef CONFIG_RTL8192C
	if(padapter->chip_type == RTL8188C_8192C)
		rtl8192cu_set_intf_ops(pops);
	#endif

	#ifdef CONFIG_RTL8192D
	if(padapter->chip_type == RTL8192D)
		rtl8192du_set_intf_ops(pops);
	#endif

	#ifdef CONFIG_RTL8723A
	if(padapter->chip_type == RTL8723A)
		rtl8723au_set_intf_ops(pops);
	#endif

	#ifdef CONFIG_RTL8188E
	if(padapter->chip_type == RTL8188E)
		rtl8188eu_set_intf_ops(pops);
	#endif

	#if defined(CONFIG_RTL8812A) || defined(CONFIG_RTL8821A)
	if(padapter->chip_type == RTL8812 || padapter->chip_type == RTL8821)
		rtl8812au_set_intf_ops(pops);
	#endif

	#ifdef CONFIG_RTL8192E
	if(padapter->chip_type == RTL8192E)
		rtl8192eu_set_intf_ops(pops);
	#endif

}


static void usb_intf_start(_adapter *padapter)
{

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+usb_intf_start\n"));

	rtw_hal_inirp_init(padapter);

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-usb_intf_start\n"));

}

static void usb_intf_stop(_adapter *padapter)
{

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+usb_intf_stop\n"));

	//disabel_hw_interrupt
	if(padapter->bSurpriseRemoved == _FALSE)
	{
		//device still exists, so driver can do i/o operation
		//TODO:
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("SurpriseRemoved==_FALSE\n"));
	}

	//cancel in irp
	rtw_hal_inirp_deinit(padapter);

	//cancel out irp
	rtw_write_port_cancel(padapter);

	//todo:cancel other irps

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-usb_intf_stop\n"));

}

static void rtw_dev_unload(_adapter *padapter)
{
	struct net_device *pnetdev= (struct net_device*)padapter->pnetdev;
	u8 val8;
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+rtw_dev_unload\n"));

	if(padapter->bup == _TRUE)
	{
		DBG_871X("===> rtw_dev_unload\n");

		padapter->bDriverStopped = _TRUE;
		#ifdef CONFIG_XMIT_ACK
		if (padapter->xmitpriv.ack_tx)
			rtw_ack_tx_done(&padapter->xmitpriv, RTW_SCTX_DONE_DRV_STOP);
		#endif

		//s3.
		if(padapter->intf_stop)
		{
			padapter->intf_stop(padapter);
		}

		//s4.
		if(!padapter->pwrctrlpriv.bInternalAutoSuspend )
		rtw_stop_drv_threads(padapter);


		//s5.
		if(padapter->bSurpriseRemoved == _FALSE)
		{
			//DBG_871X("r871x_dev_unload()->rtl871x_hal_deinit()\n");
#ifdef CONFIG_WOWLAN
			if((padapter->pwrctrlpriv.bSupportRemoteWakeup==_TRUE)&&(padapter->pwrctrlpriv.wowlan_mode==_TRUE)){
				DBG_871X("%s bSupportWakeOnWlan==_TRUE  do not run rtw_hal_deinit()\n",__FUNCTION__);
			}
			else
#endif //CONFIG_WOWLAN
			{
				rtw_hal_deinit(padapter);
			}
			padapter->bSurpriseRemoved = _TRUE;
		}

		padapter->bup = _FALSE;
#ifdef CONFIG_WOWLAN
		padapter->hw_init_completed=_FALSE;
#endif //CONFIG_WOWLAN
	}
	else
	{
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("r871x_dev_unload():padapter->bup == _FALSE\n" ));
	}

	DBG_871X("<=== rtw_dev_unload\n");

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-rtw_dev_unload\n"));

}

static void process_spec_devid(const struct usb_device_id *pdid)
{
	u16 vid, pid;
	u32 flags;
	int i;
	int num = sizeof(specific_device_id_tbl)/sizeof(struct specific_device_id);

	for(i=0; i<num; i++)
	{
		vid = specific_device_id_tbl[i].idVendor;
		pid = specific_device_id_tbl[i].idProduct;
		flags = specific_device_id_tbl[i].flags;

#ifdef CONFIG_80211N_HT
		if((pdid->idVendor==vid) && (pdid->idProduct==pid) && (flags&SPEC_DEV_ID_DISABLE_HT))
		{
			 rtw_ht_enable = 0;
			 rtw_bw_mode = 0;
			 rtw_ampdu_enable = 0;
		}
#endif

#ifdef RTK_DMP_PLATFORM
		// Change the ifname to wlan10 when PC side WFD dongle plugin on DMP platform.
		// It is used to distinguish between normal and PC-side wifi dongle/module.
		if((pdid->idVendor==vid) && (pdid->idProduct==pid) && (flags&SPEC_DEV_ID_ASSIGN_IFNAME))
		{
			extern char* ifname;
			strncpy(ifname, "wlan10", 6);
			//DBG_871X("%s()-%d: ifname=%s, vid=%04X, pid=%04X\n", __FUNCTION__, __LINE__, ifname, vid, pid);
		}
#endif /* RTK_DMP_PLATFORM */

	}
}

#ifdef SUPPORT_HW_RFOFF_DETECTED
int rtw_hw_suspend(_adapter *padapter )
{
	struct pwrctrl_priv *pwrpriv = &padapter->pwrctrlpriv;
	struct usb_interface *pusb_intf = adapter_to_dvobj(padapter)->pusbintf;
	struct net_device *pnetdev = padapter->pnetdev;

	_func_enter_;

	if((!padapter->bup) || (padapter->bDriverStopped)||(padapter->bSurpriseRemoved))
	{
		DBG_871X("padapter->bup=%d bDriverStopped=%d bSurpriseRemoved = %d\n",
			padapter->bup, padapter->bDriverStopped,padapter->bSurpriseRemoved);
		goto error_exit;
	}

	if(padapter)//system suspend
	{
		LeaveAllPowerSaveMode(padapter);

		DBG_871X("==> rtw_hw_suspend\n");
		_enter_pwrlock(&pwrpriv->lock);
		pwrpriv->bips_processing = _TRUE;
		//padapter->net_closed = _TRUE;
		//s1.
		if(pnetdev)
		{
			netif_carrier_off(pnetdev);
			rtw_netif_stop_queue(pnetdev);
		}

		//s2.
		rtw_disassoc_cmd(padapter, 500, _FALSE);

		//s2-2.  indicate disconnect to os
		//rtw_indicate_disconnect(padapter);
		{
			struct	mlme_priv *pmlmepriv = &padapter->mlmepriv;

			if(check_fwstate(pmlmepriv, _FW_LINKED))
			{
				_clr_fwstate_(pmlmepriv, _FW_LINKED);

				rtw_led_control(padapter, LED_CTL_NO_LINK);

				rtw_os_indicate_disconnect(padapter);

				#ifdef CONFIG_LPS
				//donnot enqueue cmd
				rtw_lps_ctrl_wk_cmd(padapter, LPS_CTRL_DISCONNECT, 0);
				#endif
			}

		}
		//s2-3.
		rtw_free_assoc_resources(padapter, 1);

		//s2-4.
		rtw_free_network_queue(padapter,_TRUE);
		#ifdef CONFIG_IPS
		rtw_ips_dev_unload(padapter);
		#endif
		pwrpriv->rf_pwrstate = rf_off;
		pwrpriv->bips_processing = _FALSE;

		_exit_pwrlock(&pwrpriv->lock);
	}
	else
		goto error_exit;

	_func_exit_;
	return 0;

error_exit:
	DBG_871X("%s, failed \n",__FUNCTION__);
	return (-1);

}

int rtw_hw_resume(_adapter *padapter)
{
	struct pwrctrl_priv *pwrpriv = &padapter->pwrctrlpriv;
	struct usb_interface *pusb_intf = adapter_to_dvobj(padapter)->pusbintf;
	struct net_device *pnetdev = padapter->pnetdev;

	_func_enter_;

	if(padapter)//system resume
	{
		DBG_871X("==> rtw_hw_resume\n");
		_enter_pwrlock(&pwrpriv->lock);
		pwrpriv->bips_processing = _TRUE;
		rtw_reset_drv_sw(padapter);

		if(pm_netdev_open(pnetdev,_FALSE) != 0)
		{
			_exit_pwrlock(&pwrpriv->lock);
			goto error_exit;
		}

		netif_device_attach(pnetdev);
		netif_carrier_on(pnetdev);

		if(!rtw_netif_queue_stopped(pnetdev))
      			rtw_netif_start_queue(pnetdev);
		else
			rtw_netif_wake_queue(pnetdev);

		pwrpriv->bkeepfwalive = _FALSE;
		pwrpriv->brfoffbyhw = _FALSE;

		pwrpriv->rf_pwrstate = rf_on;
		pwrpriv->bips_processing = _FALSE;

		_exit_pwrlock(&pwrpriv->lock);
	}
	else
	{
		goto error_exit;
	}

	_func_exit_;

	return 0;
error_exit:
	DBG_871X("%s, Open net dev failed \n",__FUNCTION__);
	return (-1);
}
#endif

static int rtw_suspend(struct usb_interface *pusb_intf, pm_message_t message)
{
	struct dvobj_priv *dvobj = usb_get_intfdata(pusb_intf);
	_adapter *padapter = dvobj->if1;
	struct net_device *pnetdev = padapter->pnetdev;
	struct mlme_priv *pmlmepriv = &padapter->mlmepriv;
	struct pwrctrl_priv *pwrpriv = &padapter->pwrctrlpriv;
	struct usb_device *usb_dev = interface_to_usbdev(pusb_intf);
#ifdef CONFIG_WOWLAN
	struct wowlan_ioctl_param poidparam;
#endif // CONFIG_WOWLAN

	int ret = 0;
	u32 start_time = rtw_get_current_time();

	_func_enter_;

	DBG_871X("==> %s (%s:%d)\n",__FUNCTION__, current->comm, current->pid);

#ifdef CONFIG_WOWLAN
	if (check_fwstate(pmlmepriv, _FW_LINKED))
		padapter->pwrctrlpriv.wowlan_mode = _TRUE;
	else
		padapter->pwrctrlpriv.wowlan_mode = _FALSE;
#endif

	if((!padapter->bup) || (padapter->bDriverStopped)||(padapter->bSurpriseRemoved))
	{
		DBG_871X("padapter->bup=%d bDriverStopped=%d bSurpriseRemoved = %d\n",
			padapter->bup, padapter->bDriverStopped,padapter->bSurpriseRemoved);
		goto exit;
	}

	if(pwrpriv->bInternalAutoSuspend )
	{
	#ifdef CONFIG_AUTOSUSPEND
	#ifdef SUPPORT_HW_RFOFF_DETECTED
		// The FW command register update must after MAC and FW init ready.
		if((padapter->bFWReady) && ( padapter->pwrctrlpriv.bHWPwrPindetect ) && (padapter->registrypriv.usbss_enable ))
		{
			u8 bOpen = _TRUE;
			rtw_interface_ps_func(padapter,HAL_USB_SELECT_SUSPEND,&bOpen);
			//rtl8192c_set_FwSelectSuspend_cmd(padapter,_TRUE ,500);//note fw to support hw power down ping detect
		}
	#endif
	#endif
	}
	pwrpriv->bInSuspend = _TRUE;
	rtw_cancel_all_timer(padapter);
	LeaveAllPowerSaveMode(padapter);

	_enter_pwrlock(&pwrpriv->lock);
	//padapter->net_closed = _TRUE;
	//s1.
	if(pnetdev)
	{
		netif_carrier_off(pnetdev);
		rtw_netif_stop_queue(pnetdev);
	}

#ifdef CONFIG_WOWLAN
	if(padapter->pwrctrlpriv.bSupportRemoteWakeup==_TRUE&&padapter->pwrctrlpriv.wowlan_mode==_TRUE){
		//set H2C command
		poidparam.subcode=WOWLAN_ENABLE;
		padapter->HalFunc.SetHwRegHandler(padapter,HW_VAR_WOWLAN,(u8 *)&poidparam);
	}
	else
#else
	{
	//s2.
	rtw_disassoc_cmd(padapter, 0, _FALSE);
	}
#endif //CONFIG_WOWLAN

#ifdef CONFIG_LAYER2_ROAMING_RESUME
	if(check_fwstate(pmlmepriv, WIFI_STATION_STATE) && check_fwstate(pmlmepriv, _FW_LINKED) )
	{
		//DBG_871X("%s:%d assoc_ssid:%s\n", __FUNCTION__, __LINE__, pmlmepriv->assoc_ssid.Ssid);
		DBG_871X("%s:%d %s(" MAC_FMT "), length:%d assoc_ssid.length:%d\n",__FUNCTION__, __LINE__,
				pmlmepriv->cur_network.network.Ssid.Ssid,
				MAC_ARG(pmlmepriv->cur_network.network.MacAddress),
				pmlmepriv->cur_network.network.Ssid.SsidLength,
				pmlmepriv->assoc_ssid.SsidLength);

		rtw_set_roaming(padapter, 1);
	}
#endif
	//s2-2.  indicate disconnect to os
	rtw_indicate_disconnect(padapter);
	//s2-3.
	rtw_free_assoc_resources(padapter, 1);
#ifdef CONFIG_AUTOSUSPEND
	if(!pwrpriv->bInternalAutoSuspend )
#endif
	//s2-4.
	rtw_free_network_queue(padapter, _TRUE);

	rtw_dev_unload(padapter);
#ifdef CONFIG_AUTOSUSPEND
	pwrpriv->rf_pwrstate = rf_off;
	pwrpriv->bips_processing = _FALSE;
#endif
	_exit_pwrlock(&pwrpriv->lock);

	if(check_fwstate(pmlmepriv, _FW_UNDER_SURVEY))
		rtw_indicate_scan_done(padapter, 1);

	if(check_fwstate(pmlmepriv, _FW_UNDER_LINKING))
		rtw_indicate_disconnect(padapter);

exit:
	DBG_871X("<===  %s return %d.............. in %dms\n", __FUNCTION__
		, ret, rtw_get_passing_time_ms(start_time));

	_func_exit_;
	return ret;
}

static int rtw_resume(struct usb_interface *pusb_intf)
{
	struct dvobj_priv *dvobj = usb_get_intfdata(pusb_intf);
	_adapter *padapter = dvobj->if1;
	struct net_device *pnetdev = padapter->pnetdev;
	struct pwrctrl_priv *pwrpriv = &padapter->pwrctrlpriv;
	 int ret = 0;

	if(pwrpriv->bInternalAutoSuspend ){
 		ret = rtw_resume_process(padapter);
	} else {
#ifdef CONFIG_RESUME_IN_WORKQUEUE
		rtw_resume_in_workqueue(pwrpriv);
#else
		if (rtw_is_earlysuspend_registered(pwrpriv)
			#ifdef CONFIG_WOWLAN
			&& !padapter->pwrctrlpriv.wowlan_mode
			#endif /* CONFIG_WOWLAN */
		) {
			/* jeff: bypass resume here, do in late_resume */
			rtw_set_do_late_resume(pwrpriv, _TRUE);
		} else {
			ret = rtw_resume_process(padapter);
		}
#endif /* CONFIG_RESUME_IN_WORKQUEUE */
	}

	return ret;

}

int rtw_resume_process(_adapter *padapter)
{
	struct net_device *pnetdev;
	struct pwrctrl_priv *pwrpriv;
	int ret = -1;
	u32 start_time = rtw_get_current_time();
#ifdef CONFIG_BT_COEXIST
	u8 pm_cnt;
#endif	//#ifdef CONFIG_BT_COEXIST
	_func_enter_;

	DBG_871X("==> %s (%s:%d)\n",__FUNCTION__, current->comm, current->pid);

	if(padapter) {
		pnetdev= padapter->pnetdev;
		pwrpriv = &padapter->pwrctrlpriv;
	} else {
		goto exit;
	}

	_enter_pwrlock(&pwrpriv->lock);
#ifdef CONFIG_BT_COEXIST
#ifdef CONFIG_AUTOSUSPEND
	#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,32))
	DBG_871X("%s...pm_usage_cnt(%d)  pwrpriv->bAutoResume=%x.  ....\n",__func__,atomic_read(&(adapter_to_dvobj(padapter)->pusbintf->pm_usage_cnt)),pwrpriv->bAutoResume);
	pm_cnt=atomic_read(&(adapter_to_dvobj(padapter)->pusbintf->pm_usage_cnt));
	#else
	DBG_871X("...pm_usage_cnt(%d).....\n", adapter_to_dvobj(padapter)->pusbintf->pm_usage_cnt);
	pm_cnt = adapter_to_dvobj(padapter)->pusbintf->pm_usage_cnt;
	#endif

	DBG_871X("pwrpriv->bAutoResume (%x)\n",pwrpriv->bAutoResume );
	if( _TRUE == pwrpriv->bAutoResume ){
		pwrpriv->bInternalAutoSuspend = _FALSE;
		pwrpriv->bAutoResume=_FALSE;
		DBG_871X("pwrpriv->bAutoResume (%x)  pwrpriv->bInternalAutoSuspend(%x)\n",pwrpriv->bAutoResume,pwrpriv->bInternalAutoSuspend );

	}
#endif //#ifdef CONFIG_AUTOSUSPEND
#endif //#ifdef CONFIG_BT_COEXIST
	rtw_reset_drv_sw(padapter);
	pwrpriv->bkeepfwalive = _FALSE;

	DBG_871X("bkeepfwalive(%x)\n",pwrpriv->bkeepfwalive);
	if(pm_netdev_open(pnetdev,_TRUE) != 0){
		_exit_pwrlock(&pwrpriv->lock);
		goto exit;
	}
	
	netif_device_attach(pnetdev);
	netif_carrier_on(pnetdev);

#ifdef CONFIG_AUTOSUSPEND
	if(pwrpriv->bInternalAutoSuspend )
	{
		#ifdef CONFIG_AUTOSUSPEND
		#ifdef SUPPORT_HW_RFOFF_DETECTED
			// The FW command register update must after MAC and FW init ready.
		if((padapter->bFWReady) && ( padapter->pwrctrlpriv.bHWPwrPindetect ) && (padapter->registrypriv.usbss_enable ))
		{
			//rtl8192c_set_FwSelectSuspend_cmd(padapter,_FALSE ,500);//note fw to support hw power down ping detect
			u8 bOpen = _FALSE;
			rtw_interface_ps_func(padapter,HAL_USB_SELECT_SUSPEND,&bOpen);
		}
		#endif
		#endif
#ifdef CONFIG_BT_COEXIST
		DBG_871X("pwrpriv->bAutoResume (%x)\n",pwrpriv->bAutoResume );
		if( _TRUE == pwrpriv->bAutoResume ){
		pwrpriv->bInternalAutoSuspend = _FALSE;
			pwrpriv->bAutoResume=_FALSE;
			DBG_871X("pwrpriv->bAutoResume (%x)  pwrpriv->bInternalAutoSuspend(%x)\n",pwrpriv->bAutoResume,pwrpriv->bInternalAutoSuspend );
		}

#else	//#ifdef CONFIG_BT_COEXIST
		pwrpriv->bInternalAutoSuspend = _FALSE;
#endif	//#ifdef CONFIG_BT_COEXIST
		pwrpriv->brfoffbyhw = _FALSE;
		{
			DBG_871X("enc_algorithm(%x),wepkeymask(%x)\n",
				padapter->securitypriv.dot11PrivacyAlgrthm,pwrpriv->wepkeymask);
			if(	(_WEP40_ == padapter->securitypriv.dot11PrivacyAlgrthm) ||
				(_WEP104_ == padapter->securitypriv.dot11PrivacyAlgrthm))
			{
				sint keyid;

				for(keyid=0;keyid<4;keyid++){
					if(pwrpriv->wepkeymask & BIT(keyid)) {
						if(keyid == padapter->securitypriv.dot11PrivacyKeyIndex)
							rtw_set_key(padapter,&padapter->securitypriv, keyid, 1);
						else
							rtw_set_key(padapter,&padapter->securitypriv, keyid, 0);
					}
				}
			}
		}
	}
#endif
	_exit_pwrlock(&pwrpriv->lock);

	if( padapter->pid[1]!=0) {
		DBG_871X("pid[1]:%d\n",padapter->pid[1]);
		rtw_signal_process(padapter->pid[1], SIGUSR2);
	}

	#ifdef CONFIG_LAYER2_ROAMING_RESUME
	rtw_roaming(padapter, NULL);
	#endif

	ret = 0;
exit:
	#ifdef CONFIG_RESUME_IN_WORKQUEUE
	rtw_unlock_suspend();
	#endif //CONFIG_RESUME_IN_WORKQUEUE

	pwrpriv->bInSuspend = _FALSE;
	DBG_871X("<===  %s return %d.............. in %dms\n", __FUNCTION__
		, ret, rtw_get_passing_time_ms(start_time));

	_func_exit_;

	return ret;
}

#ifdef CONFIG_AUTOSUSPEND
void autosuspend_enter(_adapter* padapter)
{
	struct pwrctrl_priv *pwrpriv = &padapter->pwrctrlpriv;
	struct dvobj_priv *dvobj = adapter_to_dvobj(padapter);

	DBG_871X("==>autosuspend_enter...........\n");

	pwrpriv->bInternalAutoSuspend = _TRUE;
	pwrpriv->bips_processing = _TRUE;

	if(rf_off == pwrpriv->change_rfpwrstate )
	{
#ifndef	CONFIG_BT_COEXIST
		#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,35))
		usb_enable_autosuspend(dvobj->pusbdev);
		#else
		dvobj->pusbdev->autosuspend_disabled = 0;//autosuspend disabled by the user
		#endif

		#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,33))
			usb_autopm_put_interface(dvobj->pusbintf);
		#elif (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,20))
			usb_autopm_enable(dvobj->pusbintf);
		#else
			usb_autosuspend_device(dvobj->pusbdev, 1);
		#endif
#else	//#ifndef	CONFIG_BT_COEXIST
		if(1==pwrpriv->autopm_cnt){
		#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,35))
		usb_enable_autosuspend(dvobj->pusbdev);
		#else
		dvobj->pusbdev->autosuspend_disabled = 0;//autosuspend disabled by the user
		#endif

		#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,33))
			usb_autopm_put_interface(dvobj->pusbintf);
		#elif (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,20))
			usb_autopm_enable(dvobj->pusbintf);
		#else
			usb_autosuspend_device(dvobj->pusbdev, 1);
		#endif
			pwrpriv->autopm_cnt --;
		}
		else
		DBG_871X("0!=pwrpriv->autopm_cnt[%d]   didn't usb_autopm_put_interface\n", pwrpriv->autopm_cnt);

#endif	//#ifndef	CONFIG_BT_COEXIST
	}
	#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,32))
	DBG_871X("...pm_usage_cnt(%d).....\n", atomic_read(&(dvobj->pusbintf->pm_usage_cnt)));
	#else
	DBG_871X("...pm_usage_cnt(%d).....\n", dvobj->pusbintf->pm_usage_cnt);
	#endif

}
int autoresume_enter(_adapter* padapter)
{
	int result = _SUCCESS;
	struct pwrctrl_priv *pwrpriv = &padapter->pwrctrlpriv;
	struct security_priv* psecuritypriv=&(padapter->securitypriv);
	struct mlme_ext_priv	*pmlmeext = &padapter->mlmeextpriv;
	struct mlme_ext_info	*pmlmeinfo = &(pmlmeext->mlmext_info);
	struct dvobj_priv *dvobj = adapter_to_dvobj(padapter);

	DBG_871X("====> autoresume_enter \n");

	if(rf_off == pwrpriv->rf_pwrstate )
	{
		pwrpriv->ps_flag = _FALSE;
#ifndef	CONFIG_BT_COEXIST
		#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,33))
			if (usb_autopm_get_interface(dvobj->pusbintf) < 0)
			{
				DBG_871X( "can't get autopm: %d\n", result);
				result = _FAIL;
				goto error_exit;
			}
		#elif (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,20))
			usb_autopm_disable(dvobj->pusbintf);
		#else
			usb_autoresume_device(dvobj->pusbdev, 1);
		#endif

		#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,32))
		DBG_871X("...pm_usage_cnt(%d).....\n", atomic_read(&(dvobj->pusbintf->pm_usage_cnt)));
		#else
		DBG_871X("...pm_usage_cnt(%d).....\n", dvobj->pusbintf->pm_usage_cnt);
		#endif
#else	//#ifndef	CONFIG_BT_COEXIST
		pwrpriv->bAutoResume=_TRUE;
		if(0==pwrpriv->autopm_cnt){
		#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,33))
			if (usb_autopm_get_interface(dvobj->pusbintf) < 0)
			{
				DBG_871X( "can't get autopm: %d\n", result);
				result = _FAIL;
				goto error_exit;
			}
		#elif (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,20))
			usb_autopm_disable(dvobj->pusbintf);
		#else
			usb_autoresume_device(dvobj->pusbdev, 1);
		#endif
		#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,32))
			DBG_871X("...pm_usage_cnt(%d).....\n", atomic_read(&(dvobj->pusbintf->pm_usage_cnt)));
		#else
			DBG_871X("...pm_usage_cnt(%d).....\n", dvobj->pusbintf->pm_usage_cnt);
		#endif
			pwrpriv->autopm_cnt++;
		}
		else
			DBG_871X("0!=pwrpriv->autopm_cnt[%d]   didn't usb_autopm_get_interface\n",pwrpriv->autopm_cnt);
#endif //#ifndef	CONFIG_BT_COEXIST
	}
	DBG_871X("<==== autoresume_enter \n");
error_exit:

	return result;
}
#endif

#ifdef CONFIG_PLATFORM_RTD2880B
extern void rtd2885_wlan_netlink_sendMsg(char *action_string, char *name);
#endif

#ifdef CONFIG_PLATFORM_ARM_SUNxI
#include <mach/sys_config.h>
extern int sw_usb_disable_hcd(__u32 usbc_no);
extern int sw_usb_enable_hcd(__u32 usbc_no);
static int usb_wifi_host = 2;
#endif

#ifdef CONFIG_PLATFORM_ARM_SUN6I
#include <mach/sys_config.h>
extern int sw_usb_disable_hcd(__u32 usbc_no);
extern int sw_usb_enable_hcd(__u32 usbc_no);
extern void wifi_pm_power(int on);
static script_item_u item;
#endif

/*
 * drv_init() - a device potentially for us
 *
 * notes: drv_init() is called when the bus driver has located a card for us to support.
 *        We accept the new device by returning 0.
*/

_adapter  *rtw_sw_export = NULL;

_adapter *rtw_usb_if1_init(struct dvobj_priv *dvobj,
	struct usb_interface *pusb_intf, const struct usb_device_id *pdid)
{
	_adapter *padapter = NULL;
	struct net_device *pnetdev = NULL;
	int status = _FAIL;

	if ((padapter = (_adapter *)rtw_zvmalloc(sizeof(*padapter))) == NULL) {
		goto exit;
	}
	padapter->dvobj = dvobj;
	dvobj->if1 = padapter;

	padapter->bDriverStopped=_TRUE;

	dvobj->padapters[dvobj->iface_nums++] = padapter;
	padapter->iface_id = IFACE_ID0;

#if defined(CONFIG_CONCURRENT_MODE) || defined(CONFIG_DUALMAC_CONCURRENT)
	//set adapter_type/iface type for primary padapter
	padapter->isprimary = _TRUE;
	padapter->adapter_type = PRIMARY_ADAPTER;
	#ifndef CONFIG_HWPORT_SWAP
	padapter->iface_type = IFACE_PORT0;
	#else
	padapter->iface_type = IFACE_PORT1;
	#endif	
#endif

	//step 1-1., decide the chip_type via driver_info
	padapter->interface_type = RTW_USB;
	rtw_decide_chip_type_by_usb_info(padapter, pdid);

	if (rtw_handle_dualmac(padapter, 1) != _SUCCESS)
		goto free_adapter;

	if((pnetdev = rtw_init_netdev(padapter)) == NULL) {
		goto handle_dualmac;
	}
	SET_NETDEV_DEV(pnetdev, dvobj_to_dev(dvobj));
	padapter = rtw_netdev_priv(pnetdev);

#ifdef CONFIG_IOCTL_CFG80211
	if(rtw_wdev_alloc(padapter, dvobj_to_dev(dvobj)) != 0) {
		goto handle_dualmac;
	}
#endif

	//step 2. hook HalFunc, allocate HalData
	//hal_set_hal_ops(padapter);
	rtw_set_hal_ops(padapter);

	padapter->intf_start=&usb_intf_start;
	padapter->intf_stop=&usb_intf_stop;

	//step init_io_priv
	rtw_init_io_priv(padapter,usb_set_intf_ops);

	//step read_chip_version
	rtw_hal_read_chip_version(padapter);

	//step usb endpoint mapping
	rtw_hal_chip_configure(padapter);

	//step read efuse/eeprom data and get mac_addr
	rtw_hal_read_chip_info(padapter);

	//step 5.
	if(rtw_init_drv_sw(padapter) ==_FAIL) {
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("Initialize driver software resource Failed!\n"));
		goto free_hal_data;
	}

#ifdef CONFIG_PM
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,18))
	if(padapter->pwrctrlpriv.bSupportRemoteWakeup)
	{
		dvobj->pusbdev->do_remote_wakeup=1;
		pusb_intf->needs_remote_wakeup = 1;
		device_init_wakeup(&pusb_intf->dev, 1);
		DBG_871X("\n  padapter->pwrctrlpriv.bSupportRemoteWakeup~~~~~~\n");
		DBG_871X("\n  padapter->pwrctrlpriv.bSupportRemoteWakeup~~~[%d]~~~\n",device_may_wakeup(&pusb_intf->dev));
	}
#endif
#endif

#ifdef CONFIG_AUTOSUSPEND
	if( padapter->registrypriv.power_mgnt != PS_MODE_ACTIVE )
	{
		if(padapter->registrypriv.usbss_enable ){ 	/* autosuspend (2s delay) */
			#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,38))
			dvobj->pusbdev->dev.power.autosuspend_delay = 0 * HZ;//15 * HZ; idle-delay time
			#else
			dvobj->pusbdev->autosuspend_delay = 0 * HZ;//15 * HZ; idle-delay time
			#endif

			#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,35))
			usb_enable_autosuspend(dvobj->pusbdev);
			#elif  (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,22) && LINUX_VERSION_CODE<=KERNEL_VERSION(2,6,34))
			padapter->bDisableAutosuspend = dvobj->pusbdev->autosuspend_disabled ;
			dvobj->pusbdev->autosuspend_disabled = 0;//autosuspend disabled by the user
			#endif

			//usb_autopm_get_interface(adapter_to_dvobj(padapter)->pusbintf );//init pm_usage_cnt ,let it start from 1

			#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,32))
			DBG_871X("%s...pm_usage_cnt(%d).....\n",__FUNCTION__,atomic_read(&(dvobj->pusbintf ->pm_usage_cnt)));
			#else
			DBG_871X("%s...pm_usage_cnt(%d).....\n",__FUNCTION__,dvobj->pusbintf ->pm_usage_cnt);
			#endif
		}
	}
#endif
	//2012-07-11 Move here to prevent the 8723AS-VAU BT auto suspend influence
	#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,33))
			if (usb_autopm_get_interface(pusb_intf) < 0)
				{
					DBG_871X( "can't get autopm: \n");
				}
	#endif
#ifdef	CONFIG_BT_COEXIST
	padapter->pwrctrlpriv.autopm_cnt=1;
#endif

	// set mac addr
	rtw_macaddr_cfg(padapter->eeprompriv.mac_addr);
	rtw_init_wifidirect_addrs(padapter, padapter->eeprompriv.mac_addr, padapter->eeprompriv.mac_addr);

	DBG_871X("bDriverStopped:%d, bSurpriseRemoved:%d, bup:%d, hw_init_completed:%d\n"
		, padapter->bDriverStopped
		, padapter->bSurpriseRemoved
		, padapter->bup
		, padapter->hw_init_completed
	);

	status = _SUCCESS;

free_hal_data:
	if(status != _SUCCESS && padapter->HalData)
		rtw_mfree(padapter->HalData, sizeof(*(padapter->HalData)));
free_wdev:
	if(status != _SUCCESS) {
		#ifdef CONFIG_IOCTL_CFG80211
		rtw_wdev_unregister(padapter->rtw_wdev);
		rtw_wdev_free(padapter->rtw_wdev);
		#endif
	}
handle_dualmac:
	if (status != _SUCCESS)
		rtw_handle_dualmac(padapter, 0);
free_adapter:
	if (status != _SUCCESS) {
		if (pnetdev)
			rtw_free_netdev(pnetdev);
		else if (padapter)
			rtw_vmfree((u8*)padapter, sizeof(*padapter));
		padapter = NULL;
	}
exit:
	return padapter;
}

static void rtw_usb_if1_deinit(_adapter *if1)
{
	struct net_device *pnetdev = if1->pnetdev;
	struct mlme_priv *pmlmepriv= &if1->mlmepriv;

	if(check_fwstate(pmlmepriv, _FW_LINKED))
		rtw_disassoc_cmd(if1, 0, _FALSE);


#ifdef CONFIG_AP_MODE
	free_mlme_ap_info(if1);
	#ifdef CONFIG_HOSTAPD_MLME
	hostapd_mode_unload(if1);
	#endif
#endif

	if(if1->DriverState != DRIVER_DISAPPEAR) {
		if(pnetdev) {
			unregister_netdev(pnetdev); //will call netdev_close()
			rtw_proc_remove_one(pnetdev);
		}
	}

	rtw_cancel_all_timer(if1);

#ifdef CONFIG_WOWLAN
	if1->pwrctrlpriv.wowlan_mode=_FALSE;
#endif //CONFIG_WOWLAN

	rtw_dev_unload(if1);

	DBG_871X("+r871xu_dev_remove, hw_init_completed=%d\n", if1->hw_init_completed);

	rtw_handle_dualmac(if1, 0);

#ifdef CONFIG_IOCTL_CFG80211
	if(if1->rtw_wdev)
	{
		rtw_wdev_unregister(if1->rtw_wdev);
		rtw_wdev_free(if1->rtw_wdev);
	}
#endif

#ifdef CONFIG_BT_COEXIST
	if(1 == if1->pwrctrlpriv.autopm_cnt){
		#if (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,33))
			usb_autopm_put_interface(adapter_to_dvobj(if1)->pusbintf);
		#elif (LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,20))
			usb_autopm_enable(adapter_to_dvobj(if1)->pusbintf);
		#else
			usb_autosuspend_device(adapter_to_dvobj(if1)->pusbdev, 1);
		#endif
		if1->pwrctrlpriv.autopm_cnt --;
	}
#endif

	rtw_free_drv_sw(if1);

	if(pnetdev)
		rtw_free_netdev(pnetdev);

#ifdef CONFIG_PLATFORM_RTD2880B
	DBG_871X("wlan link down\n");
	rtd2885_wlan_netlink_sendMsg("linkdown", "8712");
#endif

}

static void dump_usb_interface(struct usb_interface *usb_intf)
{
	int	i;
	u8	val8;

	struct usb_device				*udev = interface_to_usbdev(usb_intf);
	struct usb_device_descriptor 	*dev_desc = &udev->descriptor;

	struct usb_host_config			*act_conf = udev->actconfig;
	struct usb_config_descriptor	*act_conf_desc = &act_conf->desc;

	struct usb_host_interface		*host_iface;
	struct usb_interface_descriptor	*iface_desc;
	struct usb_host_endpoint		*host_endp;
	struct usb_endpoint_descriptor	*endp_desc;

#if 1 /* The usb device this usb interface belongs to */
	DBG_871X("usb_interface:%p, usb_device:%p(num:%d, path:%s), usb_device_descriptor:%p\n", usb_intf, udev, udev->devnum, udev->devpath, dev_desc);
	DBG_871X("bLength:%u\n", dev_desc->bLength);
	DBG_871X("bDescriptorType:0x%02x\n", dev_desc->bDescriptorType);
	DBG_871X("bcdUSB:0x%04x\n", le16_to_cpu(dev_desc->bcdUSB));
	DBG_871X("bDeviceClass:0x%02x\n", dev_desc->bDeviceClass);
	DBG_871X("bDeviceSubClass:0x%02x\n", dev_desc->bDeviceSubClass);
	DBG_871X("bDeviceProtocol:0x%02x\n", dev_desc->bDeviceProtocol);
	DBG_871X("bMaxPacketSize0:%u\n", dev_desc->bMaxPacketSize0);
	DBG_871X("idVendor:0x%04x\n", le16_to_cpu(dev_desc->idVendor));
	DBG_871X("idProduct:0x%04x\n", le16_to_cpu(dev_desc->idProduct));
	DBG_871X("bcdDevice:0x%04x\n", le16_to_cpu(dev_desc->bcdDevice));
	DBG_871X("iManufacturer:0x02%x\n", dev_desc->iManufacturer);
	DBG_871X("iProduct:0x%02x\n", dev_desc->iProduct);
	DBG_871X("iSerialNumber:0x%02x\n", dev_desc->iSerialNumber);
	DBG_871X("bNumConfigurations:%u\n", dev_desc->bNumConfigurations);
#endif


#if 1 /* The acting usb_config_descriptor */
	DBG_871X("\nact_conf_desc:%p\n", act_conf_desc);
	DBG_871X("bLength:%u\n", act_conf_desc->bLength);
	DBG_871X("bDescriptorType:0x%02x\n", act_conf_desc->bDescriptorType);
	DBG_871X("wTotalLength:%u\n", le16_to_cpu(act_conf_desc->wTotalLength));
	DBG_871X("bNumInterfaces:%u\n", act_conf_desc->bNumInterfaces);
	DBG_871X("bConfigurationValue:0x%02x\n", act_conf_desc->bConfigurationValue);
	DBG_871X("iConfiguration:0x%02x\n", act_conf_desc->iConfiguration);
	DBG_871X("bmAttributes:0x%02x\n", act_conf_desc->bmAttributes);
	DBG_871X("bMaxPower=%u\n", act_conf_desc->bMaxPower);
#endif


	DBG_871X("****** num of altsetting = (%d) ******/\n", usb_intf->num_altsetting);
	/* Get he host side alternate setting (the current alternate setting) for this interface*/
	host_iface = usb_intf->cur_altsetting;
	iface_desc = &host_iface->desc;

#if 1 /* The current alternate setting*/
	DBG_871X("\nusb_interface_descriptor:%p:\n", iface_desc);
	DBG_871X("bLength:%u\n", iface_desc->bLength);
	DBG_871X("bDescriptorType:0x%02x\n", iface_desc->bDescriptorType);
	DBG_871X("bInterfaceNumber:0x%02x\n", iface_desc->bInterfaceNumber);
	DBG_871X("bAlternateSetting=%x\n", iface_desc->bAlternateSetting);
	DBG_871X("bNumEndpoints=%x\n", iface_desc->bNumEndpoints);
	DBG_871X("bInterfaceClass=%x\n", iface_desc->bInterfaceClass);
	DBG_871X("bInterfaceSubClass=%x\n", iface_desc->bInterfaceSubClass);
	DBG_871X("bInterfaceProtocol=%x\n", iface_desc->bInterfaceProtocol);
	DBG_871X("iInterface=%x\n", iface_desc->iInterface);
#endif


#if 1
	//DBG_871X("\ndump usb_endpoint_descriptor:\n");

	for (i = 0; i < iface_desc->bNumEndpoints; i++)
	{
		host_endp = host_iface->endpoint + i;
		if (host_endp)
		{
			endp_desc = &host_endp->desc;

			DBG_871X("\nusb_endpoint_descriptor(%d):\n", i);
			DBG_871X("bLength=%x\n",endp_desc->bLength);
			DBG_871X("bDescriptorType=%x\n",endp_desc->bDescriptorType);
			DBG_871X("bEndpointAddress=%x\n",endp_desc->bEndpointAddress);
			DBG_871X("bmAttributes=%x\n",endp_desc->bmAttributes);
			DBG_871X("wMaxPacketSize=%x\n",endp_desc->wMaxPacketSize);
			DBG_871X("wMaxPacketSize=%x\n",le16_to_cpu(endp_desc->wMaxPacketSize));
			DBG_871X("bInterval=%x\n",endp_desc->bInterval);
			//DBG_871X("bRefresh=%x\n",pendp_desc->bRefresh);
			//DBG_871X("bSynchAddress=%x\n",pendp_desc->bSynchAddress);

			if (RT_usb_endpoint_is_bulk_in(endp_desc))
			{
				DBG_871X("RT_usb_endpoint_is_bulk_in = %x\n", RT_usb_endpoint_num(endp_desc));
				//pdvobjpriv->RtNumInPipes++;
			}
			else if (RT_usb_endpoint_is_int_in(endp_desc))
			{
				DBG_871X("RT_usb_endpoint_is_int_in = %x, Interval = %x\n", RT_usb_endpoint_num(endp_desc),endp_desc->bInterval);
				//pdvobjpriv->RtNumInPipes++;
			}
			else if (RT_usb_endpoint_is_bulk_out(endp_desc))
			{
				DBG_871X("RT_usb_endpoint_is_bulk_out = %x\n", RT_usb_endpoint_num(endp_desc));
				//pdvobjpriv->RtNumOutPipes++;
			}
			//pdvobjpriv->ep_num[i] = RT_usb_endpoint_num(pendp_desc);
		}
	}

	//DBG_871X("nr_endpoint=%d, in_num=%d, out_num=%d\n\n", pdvobjpriv->nr_endpoint, pdvobjpriv->RtNumInPipes, pdvobjpriv->RtNumOutPipes);
#endif

	if (udev->speed == USB_SPEED_HIGH)
		DBG_871X("USB_SPEED_HIGH\n");
	else
		DBG_871X("NON USB_SPEED_HIGH\n");

}


static int rtw_drv_init(struct usb_interface *pusb_intf, const struct usb_device_id *pdid)
{
	_adapter *if1 = NULL, *if2 = NULL;
	int status;
	struct dvobj_priv *dvobj;
#ifdef CONFIG_MULTI_VIR_IFACES
	int i;
#endif //CONFIG_MULTI_VIR_IFACES

	RT_TRACE(_module_hci_intfs_c_, _drv_err_, ("+rtw_drv_init\n"));
	//DBG_871X("+rtw_drv_init\n");

	//step 0.
	process_spec_devid(pdid);

	/* Initialize dvobj_priv */
	if ((dvobj = usb_dvobj_init(pusb_intf)) == NULL) {
		RT_TRACE(_module_hci_intfs_c_, _drv_err_, ("initialize device object priv Failed!\n"));
		goto exit;
	}

	if ((if1 = rtw_usb_if1_init(dvobj, pusb_intf, pdid)) == NULL) {
		DBG_871X("rtw_usb_if1_init Failed!\n");
		goto free_dvobj;
	}

#ifdef CONFIG_CONCURRENT_MODE
	if((if2 = rtw_drv_if2_init(if1, usb_set_intf_ops)) == NULL) {
		goto free_if1;
	}
#ifdef CONFIG_MULTI_VIR_IFACES
	for(i=0; i<if1->registrypriv.ext_iface_num;i++)
	{
		if(rtw_drv_add_vir_if(if1, usb_set_intf_ops) == NULL)
		{
			DBG_871X("rtw_drv_add_iface failed! (%d)\n", i);
			goto free_if2;
		}
	}
#endif //CONFIG_MULTI_VIR_IFACES
#endif

#ifdef CONFIG_INTEL_PROXIM
	rtw_sw_export=if1;
#endif

#ifdef CONFIG_GLOBAL_UI_PID
	if(ui_pid[1]!=0) {
		DBG_871X("ui_pid[1]:%d\n",ui_pid[1]);
		rtw_signal_process(ui_pid[1], SIGUSR2);
	}
#endif

	//dev_alloc_name && register_netdev
	if((status = rtw_drv_register_netdev(if1)) != _SUCCESS) {
		goto free_if2;
	}

#ifdef CONFIG_HOSTAPD_MLME
	hostapd_mode_init(if1);
#endif

#ifdef CONFIG_PLATFORM_RTD2880B
	DBG_871X("wlan link up\n");
	rtd2885_wlan_netlink_sendMsg("linkup", "8712");
#endif

#ifdef RTK_DMP_PLATFORM
	rtw_proc_init_one(if1->pnetdev);
#endif

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-871x_drv - drv_init, success!\n"));

	status = _SUCCESS;

free_if2:
	if(status != _SUCCESS && if2) {
		#ifdef CONFIG_CONCURRENT_MODE
		rtw_drv_if2_stop(if2);
		rtw_drv_if2_free(if2);
		#endif
	}
free_if1:
	if (status != _SUCCESS && if1) {
		rtw_usb_if1_deinit(if1);
	}
free_dvobj:
	if (status != _SUCCESS)
		usb_dvobj_deinit(pusb_intf);
exit:
	return status == _SUCCESS?0:-ENODEV;
}

/*
 * dev_remove() - our device is being removed
*/
//rmmod module & unplug(SurpriseRemoved) will call r871xu_dev_remove() => how to recognize both
static void rtw_dev_remove(struct usb_interface *pusb_intf)
{
	struct dvobj_priv *dvobj = usb_get_intfdata(pusb_intf);
	_adapter *padapter = dvobj->if1;
	struct net_device *pnetdev = padapter->pnetdev;
	struct mlme_priv *pmlmepriv= &padapter->mlmepriv;

_func_enter_;

	DBG_871X("+rtw_dev_remove\n");
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+dev_remove()\n"));

	if(usb_drv.drv_registered == _TRUE)
	{
		//DBG_871X("r871xu_dev_remove():padapter->bSurpriseRemoved == _TRUE\n");
		padapter->bSurpriseRemoved = _TRUE;
	}
	/*else
	{
		//DBG_871X("r871xu_dev_remove():module removed\n");
		padapter->hw_init_completed = _FALSE;
	}*/

#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_ANDROID_POWER)
	rtw_unregister_early_suspend(&padapter->pwrctrlpriv);
#endif

	rtw_pm_set_ips(padapter, IPS_NONE);
	rtw_pm_set_lps(padapter, PS_MODE_ACTIVE);

	LeaveAllPowerSaveMode(padapter);

#ifdef CONFIG_CONCURRENT_MODE
#ifdef CONFIG_MULTI_VIR_IFACES
	rtw_drv_stop_vir_ifaces(dvobj);
#endif //CONFIG_MULTI_VIR_IFACES
	rtw_drv_if2_stop(dvobj->if2);
#endif //CONFIG_CONCURRENT_MODE

	rtw_usb_if1_deinit(padapter);

#ifdef CONFIG_CONCURRENT_MODE
#ifdef CONFIG_MULTI_VIR_IFACES
	rtw_drv_free_vir_ifaces(dvobj);
#endif //CONFIG_MULTI_VIR_IFACES
	rtw_drv_if2_free(dvobj->if2);
#endif //CONFIG_CONCURRENT_MODE

	usb_dvobj_deinit(pusb_intf);

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-dev_remove()\n"));
	DBG_871X("-r871xu_dev_remove, done\n");


#ifdef CONFIG_INTEL_PROXIM
	rtw_sw_export=NULL;
#endif

	#ifdef DBG_MEM_ALLOC
	rtw_dump_mem_stat ();
	#endif
_func_exit_;

	return;

}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
extern int console_suspend_enabled;
#endif

static int __init rtw_drv_entry(void)
{
#ifdef CONFIG_PLATFORM_RTK_DMP
	u32 tmp;
	tmp=readl((volatile unsigned int*)0xb801a608);
	tmp &= 0xffffff00;
	tmp |= 0x55;
	writel(tmp,(volatile unsigned int*)0xb801a608);//write dummy register for 1055
#endif
#ifdef CONFIG_PLATFORM_ARM_SUNxI
#ifndef CONFIG_RTL8723A
	int ret = 0;
	/* ----------get usb_wifi_usbc_num------------- */
	ret = script_parser_fetch("usb_wifi_para", "usb_wifi_usbc_num", (int *)&usb_wifi_host, 64);
	if(ret != 0){
		DBG_8192C("ERR: script_parser_fetch usb_wifi_usbc_num failed\n");
		ret = -ENOMEM;
		return ret;
	}
	DBG_8192C("sw_usb_enable_hcd: usbc_num = %d\n", usb_wifi_host);
	sw_usb_enable_hcd(usb_wifi_host);
#endif //CONFIG_RTL8723A
#endif //CONFIG_PLATFORM_ARM_SUNxI

#ifdef CONFIG_PLATFORM_ARM_SUN6I
	script_item_value_type_e type;

	type = script_get_item("wifi_para", "wifi_usbc_id", &item);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type){
		printk("ERR: script_get_item wifi_usbc_id failed\n");
		return -ENOMEM;
	}

	printk("sw_usb_enable_hcd: usbc_num = %d\n", item.val);
	wifi_pm_power(1);
	mdelay(10);
	sw_usb_enable_hcd(item.val);
#endif //CONFIG_PLATFORM_ARM_SUN6I

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+rtw_drv_entry\n"));

	DBG_871X(DRV_NAME " driver version=%s\n", DRIVERVERSION);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
	//console_suspend_enabled=0;
#endif

	rtw_suspend_lock_init();

	usb_drv.drv_registered = _TRUE;
	return usb_register(&usb_drv.usbdrv);
}

static void __exit rtw_drv_halt(void)
{
	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+rtw_drv_halt\n"));
	DBG_871X("+rtw_drv_halt\n");

	rtw_suspend_lock_uninit();

	usb_drv.drv_registered = _FALSE;
	usb_deregister(&usb_drv.usbdrv);

#ifdef CONFIG_PLATFORM_ARM_SUNxI
#ifndef CONFIG_RTL8723A
	DBG_8192C("sw_usb_disable_hcd: usbc_num = %d\n", usb_wifi_host);
	sw_usb_disable_hcd(usb_wifi_host);
#endif //ifndef CONFIG_RTL8723A
#endif	//CONFIG_PLATFORM_ARM_SUNxI

#ifdef CONFIG_PLATFORM_ARM_SUN6I
	sw_usb_disable_hcd(item.val);
	wifi_pm_power(0);
#endif

	DBG_871X("-rtw_drv_halt\n");
}


module_init(rtw_drv_entry);
module_exit(rtw_drv_halt);

#ifdef CONFIG_INTEL_PROXIM
_adapter  *rtw_usb_get_sw_pointer(void)
{
	return rtw_sw_export;
}
EXPORT_SYMBOL(rtw_usb_get_sw_pointer);
#endif	//CONFIG_INTEL_PROXIM

