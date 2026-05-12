/* -----------------------------------------------------------------------------
 * gcc_compat.h
 *
 * Force-included via `-include` so that every translation unit sees the same
 * ARMCC (Keil) -> GCC compatibility shim. The goal is to keep the original
 * ARMCC sources untouched: each ARMCC-specific keyword / attribute / pragma is
 * either translated to its GCC equivalent or expanded to nothing.
 * -------------------------------------------------------------------------- */
#ifndef __GCC_COMPAT_H__
#define __GCC_COMPAT_H__

#if defined(__GNUC__) && !defined(__CC_ARM) && !defined(__ARMCC_VERSION)

/* ---- Type / keyword polyfills ---- */
/* ARMCC: __packed struct {...}  -> GCC: __attribute__((packed)) struct {...} */
#ifndef __packed
#  define __packed              __attribute__((packed))
#endif

/* ARMCC: __declspec(noreturn) / __declspec(noinline) / __declspec(thread) */
#ifndef __declspec
#  define __declspec(x)         __declspec_##x
#endif
#define __declspec_noreturn     __attribute__((noreturn))
#define __declspec_noinline     __attribute__((noinline))
#define __declspec_thread       __thread

/* ARMCC: __forceinline */
#ifndef __forceinline
#  define __forceinline         inline __attribute__((always_inline))
#endif

/* main.c uses `__attribute((alias("..")))` (one underscore short of the
 * standard form, which ARMCC 5 tolerates). GCC does not recognise it, so
 * map the bare keyword to the standard __attribute__. */
#ifndef __attribute
#  define __attribute __attribute__
#endif

/* ARMCC: __weak as a keyword (distinct from __attribute__((weak))). */
#ifndef __weak
#  define __weak                __attribute__((weak))
#endif

/* ARMCC: __irq (interrupt marker). Not required by GCC. */
#ifndef __irq
#  define __irq
#endif

/* ARMCC RTX: __swi(n) / __svc_indirect(n) only ever appear as function
 * declaration attributes. This project is bare-metal (no RTX), so they all
 * collapse to nothing.
 *
 * Note: do NOT pre-define __SVC_0/__SVC_1/__SVC_2 here. RTL.h does
 *   `#define __SVC_0 __svc_indirect(0)` without an #ifndef guard, which would
 * trigger a macro-redefinition warning. We only polyfill __svc_indirect(n);
 * RTL.h's own __SVC_x then expands to nothing through that. */
#ifndef __swi
#  define __swi(n)
#endif
#ifndef __svc_indirect
#  define __svc_indirect(n)
#endif

/* ARMCC: __at(addr) / __attribute__((at(addr))). Not supported by GCC.
 * This project does not actually use absolute-address variables, so the macro
 * collapses to nothing. Add an ld section if that ever changes. */
#ifndef __AT_ADDR
#  define __AT_ADDR(x)
#endif

/* ARMCC: __TARGET_ARCH_* - used as a branch selector in some headers. */
#ifndef __TARGET_ARCH_7_M
#  define __TARGET_ARCH_7_M     1   /* Cortex-M3 / M4 are ARMv7-M */
#endif

/* ---- Global 1-byte alignment ----
 * The original Keil sources rely heavily on `typedef __packed struct {...} T;`.
 * GCC 14 no longer treats an __attribute__((packed)) placed in front of a
 * typedef as a modifier of the underlying struct, so the packed attribute is
 * silently dropped and USB descriptor layouts end up misaligned.
 *
 * Forcing #pragma pack(1) globally avoids that without touching the original
 * sources. Cortex-M3 supports unaligned access; the resulting performance
 * impact is negligible (CMSIS-DAP throughput is bounded by USB and SWD
 * timing, not by packed-struct overhead). This must take effect before any
 * other header is included, which is exactly what -include gcc_compat.h
 * guarantees.
 */
#pragma pack(1)

#endif /* __GNUC__ */

#endif /* __GCC_COMPAT_H__ */
