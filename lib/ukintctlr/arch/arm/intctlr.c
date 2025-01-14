/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Authors: Wei Chen <wei.chen@arm.com>
 *
 * Copyright (c) 2018, Arm Ltd., All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <uk/assert.h>
#include <uk/intctlr.h>
#include <arm/cpu.h>
#include <arm/irq.h>
#include <uk/intctlr/gic.h>
#include <uk/essentials.h>
#include <uk/plat/common/bootinfo.h>

/** Corresponding driver for GIC present on the hardware */
struct _gic_dev *gic;

void intctrl_init(void)
{
	int rc;

	/* Initialize GIC from DTB */
	rc = init_gic(&gic);
	if (unlikely(rc))
		goto EXIT_ERR;

	/* Initialize GIC */
	rc = gic->ops.initialize();
	if (unlikely(rc))
		goto EXIT_ERR;

	return;

EXIT_ERR:
	UK_CRASH("Initialize GIC from DTB failed (code: %d)!\n", rc);
}

void intctrl_ack_irq(unsigned int irq __unused)
{
	gic->ops.ack_irq();
}

void intctrl_mask_irq(unsigned int irq)
{
	gic->ops.disable_irq(irq);
}

void intctrl_clear_irq(unsigned int irq)
{
	gic->ops.enable_irq(irq);
}

void intctrl_send_ipi(__u8 sgintid, __u32 cpuid)
{
	gic->ops.gic_sgi_gen(sgintid, cpuid);
}
