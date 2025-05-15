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

#endif /* __RTL8812AU_COMPAT_LINUX_H */
