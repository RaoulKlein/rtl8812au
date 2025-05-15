#ifndef __RTL8812AU_COMPAT_LINUX_H
#define __RTL8812AU_COMPAT_LINUX_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0)
/* For memory barriers on AMD processors */
#ifndef smp_mb__before_atomic
#define smp_mb__before_atomic() smp_mb()
#endif
#ifndef smp_mb__after_atomic
#define smp_mb__after_atomic()  smp_mb()
#endif

/* Fallback values for CPU families */
#ifndef X86_FEATURE_MWAIT
#define X86_FEATURE_MWAIT (4*32+29)
#endif
#endif /* KERNEL_VERSION >= 6.1.0 */

/* Add compatibility for kernel 6.8+ */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 8, 0)
/* New USB handling for 6.8+ */
#include <linux/usb.h>
#include <linux/skbuff.h>

/* Support for new netlink API */
#ifndef NLA_PUT_U64_64BIT
#define NLA_PUT_U64_64BIT(_skb, _attrtype, _value, _padattr) \
	do { \
		u64 _tmp = _value; \
		NLA_PUT(_skb, _attrtype, sizeof(u64), &_tmp); \
	} while(0)
#endif

/* Timer handling for 6.8+ kernels */
#ifndef setup_timer
#define setup_timer(timer, fn, data) \
	timer_setup((timer), (void (*)(struct timer_list *))fn, 0)
#define mod_timer(timer, expires) mod_timer(timer, expires)
#endif

/* SKB handling for 6.8+ kernels */
#ifndef skb_reset_tail_pointer
#define skb_reset_tail_pointer(a) \
	do { \
		skb_reset_tail_pointer(a); \
	} while (0)
#endif

#endif /* KERNEL_VERSION >= 6.8.0 */

#endif /* __RTL8812AU_COMPAT_LINUX_H */
