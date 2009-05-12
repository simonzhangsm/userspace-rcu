#ifndef _ARCH_ATOMIC_X86_H
#define _ARCH_ATOMIC_X86_H

/* 
 * Copyright (c) 1991-1994 by Xerox Corporation.  All rights reserved.
 * Copyright (c) 1996-1999 by Silicon Graphics.  All rights reserved.
 * Copyright (c) 1999-2004 Hewlett-Packard Development Company, L.P.
 * Copyright (c) 2009      Mathieu Desnoyers
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 * Code inspired from libatomic_ops-1.2, inherited in part from the
 * Boehm-Demers-Weiser conservative garbage collector.
 */

#ifndef BITS_PER_LONG
#define BITS_PER_LONG	(__SIZEOF_LONG__ * 8)
#endif

#ifndef _INCLUDE_API_H

/*
 * Derived from AO_compare_and_swap() and AO_test_and_set_full().
 */

static __attribute__((always_inline))
unsigned int atomic_exchange_32(volatile unsigned int *addr, unsigned int val)
{
	unsigned int result;

	__asm__ __volatile__(
		/* Note: the "xchg" instruction does not need a "lock" prefix */
		"xchgl %0, %1"
			: "=&r"(result), "=m"(*addr)
			: "0" (val), "m"(*addr)
			: "memory");

	return result;
}

#if (BITS_PER_LONG == 64)

static __attribute__((always_inline))
unsigned long atomic_exchange_64(volatile unsigned long *addr,
				 unsigned long val)
{
	unsigned long result;

	__asm__ __volatile__(
		/* Note: the "xchg" instruction does not need a "lock" prefix */
		"xchgq %0, %1"
			: "=&r"(result), "=m"(*addr)
			: "0" (val), "m"(*addr)
			: "memory");

	return result;
}

#endif

static __attribute__((always_inline))
unsigned long _atomic_exchange(volatile void *addr, unsigned long val, int len)
{
	switch (len) {
	case 4:	return atomic_exchange_32(addr, val);
#if (BITS_PER_LONG == 64)
	case 8:	return atomic_exchange_64(addr, val);
#endif
	}
	/* generate an illegal instruction. Cannot catch this with linker tricks
	 * when optimizations are disabled. */
	__asm__ __volatile__("ud2");
	return 0;
}

#define xchg(addr, v)							    \
	((__typeof__(*(addr))) _atomic_exchange((addr), (unsigned long)(v), \
						sizeof(*(addr))))

#endif /* #ifndef _INCLUDE_API_H */

#endif /* ARCH_ATOMIC_X86_H */