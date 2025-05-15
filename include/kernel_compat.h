/******************************************************************************
 *
 * Copyright(c) 2007 - 2025 Realtek Corporation. All rights reserved.
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
 ******************************************************************************/

#ifndef __KERNEL_COMPAT_H__
#define __KERNEL_COMPAT_H__

#include <linux/version.h>

/* Compatibility with new cfg80211 APIs in kernel 6.x */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0)
/* Define select_queue parameters for 6.1+ kernels */
#define RTW_SELECT_QUEUE_ARGS net_device *dev, struct sk_buff *skb, struct net_device *sb_dev, select_queue_fallback_t fallback
#define RTW_SELECT_QUEUE_INIT_ARGS dev, skb, sb_dev, fallback
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(5, 2, 0)
#define RTW_SELECT_QUEUE_ARGS net_device *dev, struct sk_buff *skb, struct net_device *sb_dev
#define RTW_SELECT_QUEUE_INIT_ARGS dev, skb, sb_dev
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0) 
#define RTW_SELECT_QUEUE_ARGS net_device *dev, struct sk_buff *skb, void *accel_priv, select_queue_fallback_t fallback
#define RTW_SELECT_QUEUE_INIT_ARGS dev, skb, accel_priv, fallback
#else
#define RTW_SELECT_QUEUE_ARGS net_device *dev, struct sk_buff *skb
#define RTW_SELECT_QUEUE_INIT_ARGS dev, skb
#endif

/* IEEE80211_BAND_* was renamed to NL80211_BAND_* in kernel 4.7 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0)
#define IEEE80211_BAND_2GHZ NL80211_BAND_2GHZ
#define IEEE80211_BAND_5GHZ NL80211_BAND_5GHZ
#define IEEE80211_NUM_BANDS NUM_NL80211_BANDS
#endif

/* In kernel 6.0+, some wireless parameters changed */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 0, 0)
#define IEEE80211_MAX_AMPDU_BUF 0xFF /* removed in kernel 6.0 */
#endif

#endif /* __KERNEL_COMPAT_H__ */
