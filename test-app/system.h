/* system.h
 *
 *
 * Copyright (C) 2021 wolfSSL Inc.
 *
 * This file is part of wolfBoot.
 *
 * wolfBoot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * wolfBoot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#define PLL_FULL_MASK (0x7F037FFF)

/* Assembly helpers */
#ifndef DMB
#define DMB() asm volatile ("dmb");
#endif
#define WFI() asm volatile ("wfi");

/* Master clock setting */
void clock_config(void);


/* NVIC */
/* NVIC ISER Base register (Cortex-M) */

#define NVIC_TIM2_IRQN          (28)
#define NVIC_ISER_BASE (0xE000E100)
#define NVIC_ICER_BASE (0xE000E180)
#define NVIC_IPRI_BASE (0xE000E400)

static inline void nvic_irq_enable(uint8_t n)
{
    int i = n / 32;
    volatile uint32_t *nvic_iser = ((volatile uint32_t *)(NVIC_ISER_BASE + 4 * i));
    *nvic_iser |= (1 << (n % 32));
}

static inline void nvic_irq_disable(uint8_t n)
{
    int i = n / 32;
    volatile uint32_t *nvic_icer = ((volatile uint32_t *)(NVIC_ICER_BASE + 4 * i));
    *nvic_icer |= (1 << (n % 32));
}

static inline void nvic_irq_setprio(uint8_t n, uint8_t prio)
{
    volatile uint8_t *nvic_ipri = ((volatile uint8_t *)(NVIC_IPRI_BASE + n));
    *nvic_ipri = prio;
}

#endif /* !SYSTEM_H_INCLUDED */
