/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2020 ARM Ltd.
 */
#ifndef __ASM_VDSO_PROCESSOR_H
#define __ASM_VDSO_PROCESSOR_H

#ifndef __ASSEMBLY__

==> https://stackoverflow.com/questions/7086220/what-does-rep-nop-mean-in-x86-assembly-is-it-the-same-as-the-pause-instru
==> The rep; nop is same as pause. But some processor doesn't support pause instruction.
==> This provides backward compatibility.
==> The pause instruction hints to cpu that this is busy wait loop, and do not add memory reorder for these instruction when hyperthread is supported.
/* REP NOP (PAUSE) is a good thing to insert into busy-wait loops. */
static __always_inline void rep_nop(void)
{
	asm volatile("rep; nop" ::: "memory");
}

static __always_inline void cpu_relax(void)
{
	rep_nop();
}

#endif /* __ASSEMBLY__ */

#endif /* __ASM_VDSO_PROCESSOR_H */
