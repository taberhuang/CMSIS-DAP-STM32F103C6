/**************************************************************************//**
 * @file     cmsis_gcc.h
 * @brief    CMSIS compiler GCC header file (subset for Cortex-M3, V5.x style)
 *
 * Subset of ARM CMSIS 5 cmsis_gcc.h (Apache-2.0). Only the bits that this
 * Cortex-M3 firmware actually consumes are kept. Pairs with
 * Firmware/Core/core_cm3.h (V5.1.1) shipped in this repository.
 ******************************************************************************/
#ifndef __CMSIS_GCC_H
#define __CMSIS_GCC_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* CMSIS compiler-specific defines */
#ifndef   __ASM
  #define __ASM                                  __asm
#endif
#ifndef   __INLINE
  #define __INLINE                               inline
#endif
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE                        static inline
#endif
#ifndef   __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static inline
#endif
#ifndef   __NO_RETURN
  #define __NO_RETURN                            __attribute__((__noreturn__))
#endif
#ifndef   __USED
  #define __USED                                 __attribute__((used))
#endif
#ifndef   __WEAK
  #define __WEAK                                 __attribute__((weak))
#endif
#ifndef   __PACKED
  #define __PACKED                               __attribute__((packed, aligned(1)))
#endif
#ifndef   __PACKED_STRUCT
  #define __PACKED_STRUCT                        struct __attribute__((packed, aligned(1)))
#endif
#ifndef   __PACKED_UNION
  #define __PACKED_UNION                         union  __attribute__((packed, aligned(1)))
#endif
#ifndef   __UNALIGNED_UINT32        /* deprecated */
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  struct __attribute__((packed)) T_UINT32 { uint32_t v; };
  #pragma GCC diagnostic pop
  #define __UNALIGNED_UINT32(x)                  (((struct T_UINT32 *)(x))->v)
#endif
#ifndef   __UNALIGNED_UINT16_WRITE
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  __PACKED_STRUCT T_UINT16_WRITE { uint16_t v; };
  #pragma GCC diagnostic pop
  #define __UNALIGNED_UINT16_WRITE(addr, val)    (void)((((struct T_UINT16_WRITE *)(void*)(addr))->v) = (val))
#endif
#ifndef   __UNALIGNED_UINT16_READ
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  __PACKED_STRUCT T_UINT16_READ { uint16_t v; };
  #pragma GCC diagnostic pop
  #define __UNALIGNED_UINT16_READ(addr)          (((const struct T_UINT16_READ *)(const void *)(addr))->v)
#endif
#ifndef   __UNALIGNED_UINT32_WRITE
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  __PACKED_STRUCT T_UINT32_WRITE { uint32_t v; };
  #pragma GCC diagnostic pop
  #define __UNALIGNED_UINT32_WRITE(addr, val)    (void)((((struct T_UINT32_WRITE *)(void*)(addr))->v) = (val))
#endif
#ifndef   __UNALIGNED_UINT32_READ
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpacked"
  #pragma GCC diagnostic ignored "-Wattributes"
  __PACKED_STRUCT T_UINT32_READ { uint32_t v; };
  #pragma GCC diagnostic pop
  #define __UNALIGNED_UINT32_READ(addr)          (((const struct T_UINT32_READ *)(const void *)(addr))->v)
#endif
#ifndef   __ALIGNED
  #define __ALIGNED(x)                           __attribute__((aligned(x)))
#endif
#ifndef   __RESTRICT
  #define __RESTRICT                             __restrict
#endif
#ifndef   __COMPILER_BARRIER
  #define __COMPILER_BARRIER()                   __ASM volatile("":::"memory")
#endif
#ifndef __NO_INIT
  #define __NO_INIT                              __attribute__ ((section (".bss.noinit")))
#endif
#ifndef __ALIAS
  #define __ALIAS(x)                             __attribute__ ((alias(x)))
#endif

/* ###########################  Core Function Access  ########################### */

__STATIC_FORCEINLINE void __enable_irq(void)               { __ASM volatile ("cpsie i" : : : "memory"); }
__STATIC_FORCEINLINE void __disable_irq(void)              { __ASM volatile ("cpsid i" : : : "memory"); }

__STATIC_FORCEINLINE uint32_t __get_CONTROL(void)
{
  uint32_t r;
  __ASM volatile ("MRS %0, control" : "=r" (r));
  return r;
}
__STATIC_FORCEINLINE void __set_CONTROL(uint32_t v)
{
  __ASM volatile ("MSR control, %0" : : "r" (v) : "memory");
}

__STATIC_FORCEINLINE uint32_t __get_IPSR(void)
{
  uint32_t r;
  __ASM volatile ("MRS %0, ipsr" : "=r" (r));
  return r;
}
__STATIC_FORCEINLINE uint32_t __get_APSR(void)
{
  uint32_t r;
  __ASM volatile ("MRS %0, apsr" : "=r" (r));
  return r;
}
__STATIC_FORCEINLINE uint32_t __get_xPSR(void)
{
  uint32_t r;
  __ASM volatile ("MRS %0, xpsr" : "=r" (r));
  return r;
}

__STATIC_FORCEINLINE uint32_t __get_PSP(void)
{
  uint32_t r;
  __ASM volatile ("MRS %0, psp" : "=r" (r));
  return r;
}
__STATIC_FORCEINLINE void __set_PSP(uint32_t topOfProcStack)
{
  __ASM volatile ("MSR psp, %0" : : "r" (topOfProcStack) : );
}

__STATIC_FORCEINLINE uint32_t __get_MSP(void)
{
  uint32_t r;
  __ASM volatile ("MRS %0, msp" : "=r" (r));
  return r;
}
__STATIC_FORCEINLINE void __set_MSP(uint32_t topOfMainStack)
{
  __ASM volatile ("MSR msp, %0" : : "r" (topOfMainStack) : );
}

__STATIC_FORCEINLINE uint32_t __get_PRIMASK(void)
{
  uint32_t r;
  __ASM volatile ("MRS %0, primask" : "=r" (r));
  return r;
}
__STATIC_FORCEINLINE void __set_PRIMASK(uint32_t priMask)
{
  __ASM volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
}

__STATIC_FORCEINLINE uint32_t __get_BASEPRI(void)
{
  uint32_t r;
  __ASM volatile ("MRS %0, basepri" : "=r" (r));
  return r;
}
__STATIC_FORCEINLINE void __set_BASEPRI(uint32_t value)
{
  __ASM volatile ("MSR basepri, %0" : : "r" (value) : "memory");
}
__STATIC_FORCEINLINE void __set_BASEPRI_MAX(uint32_t value)
{
  __ASM volatile ("MSR basepri_max, %0" : : "r" (value) : "memory");
}

__STATIC_FORCEINLINE uint32_t __get_FAULTMASK(void)
{
  uint32_t r;
  __ASM volatile ("MRS %0, faultmask" : "=r" (r));
  return r;
}
__STATIC_FORCEINLINE void __set_FAULTMASK(uint32_t faultMask)
{
  __ASM volatile ("MSR faultmask, %0" : : "r" (faultMask) : "memory");
}

/* ##########################  Core Instruction Access  ######################### */

#define __NOP()                             __ASM volatile ("nop")
#define __WFI()                             __ASM volatile ("wfi":::"memory")
#define __WFE()                             __ASM volatile ("wfe":::"memory")
#define __SEV()                             __ASM volatile ("sev")
#define __ISB()                             __ASM volatile ("isb 0xF":::"memory")
#define __DSB()                             __ASM volatile ("dsb 0xF":::"memory")
#define __DMB()                             __ASM volatile ("dmb 0xF":::"memory")

__STATIC_FORCEINLINE uint32_t __REV(uint32_t value)
{
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
  return __builtin_bswap32(value);
#else
  uint32_t result;
  __ASM volatile ("rev %0, %1" : "=r" (result) : "r" (value));
  return result;
#endif
}
__STATIC_FORCEINLINE uint32_t __REV16(uint32_t value)
{
  uint32_t result;
  __ASM volatile ("rev16 %0, %1" : "=r" (result) : "r" (value));
  return result;
}
__STATIC_FORCEINLINE int16_t __REVSH(int16_t value)
{
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
  return (int16_t)__builtin_bswap16(value);
#else
  int16_t result;
  __ASM volatile ("revsh %0, %1" : "=r" (result) : "r" (value));
  return result;
#endif
}
__STATIC_FORCEINLINE uint32_t __ROR(uint32_t op1, uint32_t op2)
{
  op2 %= 32U;
  if (op2 == 0U) return op1;
  return (op1 >> op2) | (op1 << (32U - op2));
}

#define __BKPT(value)                       __ASM volatile ("bkpt "#value)

__STATIC_FORCEINLINE uint32_t __RBIT(uint32_t value)
{
  uint32_t result;
#if (defined (__ARM_ARCH_7M__      ) && (__ARM_ARCH_7M__      == 1)) || \
    (defined (__ARM_ARCH_7EM__     ) && (__ARM_ARCH_7EM__     == 1)) || \
    (defined (__ARM_ARCH_8M_MAIN__ ) && (__ARM_ARCH_8M_MAIN__ == 1))
  __ASM volatile ("rbit %0, %1" : "=r" (result) : "r" (value));
#else
  int32_t s = (4U /*sizeof(v)*/ * 8U) - 1U;
  result = value;
  for (value >>= 1U; value != 0U; value >>= 1U) {
    result <<= 1U;
    result |= value & 1U;
    s--;
  }
  result <<= s;
#endif
  return result;
}
__STATIC_FORCEINLINE uint8_t __CLZ(uint32_t value)
{
  if (value == 0U) return 32U;
  return (uint8_t)__builtin_clz(value);
}

__STATIC_FORCEINLINE uint8_t __LDREXB(volatile uint8_t *addr)
{
  uint32_t result;
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
  __ASM volatile ("ldrexb %0, %1" : "=r" (result) : "Q" (*addr));
#else
  __ASM volatile ("ldrexb %0, [%1]" : "=r" (result) : "r" (addr) : "memory");
#endif
  return ((uint8_t) result);
}
__STATIC_FORCEINLINE uint16_t __LDREXH(volatile uint16_t *addr)
{
  uint32_t result;
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
  __ASM volatile ("ldrexh %0, %1" : "=r" (result) : "Q" (*addr));
#else
  __ASM volatile ("ldrexh %0, [%1]" : "=r" (result) : "r" (addr) : "memory");
#endif
  return ((uint16_t) result);
}
__STATIC_FORCEINLINE uint32_t __LDREXW(volatile uint32_t *addr)
{
  uint32_t result;
  __ASM volatile ("ldrex %0, %1" : "=r" (result) : "Q" (*addr));
  return result;
}
__STATIC_FORCEINLINE uint32_t __STREXB(uint8_t value, volatile uint8_t *addr)
{
  uint32_t result;
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
  __ASM volatile ("strexb %0, %2, %1" : "=&r" (result), "=Q" (*addr) : "r" ((uint32_t)value));
#else
  __ASM volatile ("strexb %0, %2, [%1]" : "=&r" (result) : "r" (addr), "r" ((uint32_t)value) : "memory");
#endif
  return result;
}
__STATIC_FORCEINLINE uint32_t __STREXH(uint16_t value, volatile uint16_t *addr)
{
  uint32_t result;
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
  __ASM volatile ("strexh %0, %2, %1" : "=&r" (result), "=Q" (*addr) : "r" ((uint32_t)value));
#else
  __ASM volatile ("strexh %0, %2, [%1]" : "=&r" (result) : "r" (addr), "r" ((uint32_t)value) : "memory");
#endif
  return result;
}
__STATIC_FORCEINLINE uint32_t __STREXW(uint32_t value, volatile uint32_t *addr)
{
  uint32_t result;
  __ASM volatile ("strex %0, %2, %1" : "=&r" (result), "=Q" (*addr) : "r" (value));
  return result;
}
__STATIC_FORCEINLINE void __CLREX(void)
{
  __ASM volatile ("clrex" ::: "memory");
}

#define __SSAT(ARG1, ARG2) \
__extension__ \
({                          \
  int32_t __RES, __ARG1 = (ARG1); \
  __ASM volatile ("ssat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) : "cc"); \
  __RES; \
 })
#define __USAT(ARG1, ARG2) \
__extension__ \
({                          \
  uint32_t __RES, __ARG1 = (ARG1); \
  __ASM volatile ("usat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) : "cc"); \
  __RES; \
 })

#pragma GCC diagnostic pop

#endif /* __CMSIS_GCC_H */
