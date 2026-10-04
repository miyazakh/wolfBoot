/* nxp_imx8.c
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
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <target.h>
#include "image.h"
#include "printf.h"
#ifndef ARCH_AARCH64
#   error "wolfBoot nxp-imx8 HAL: wrong architecture selected. Please compile with ARCH=AARCH64."
#endif

/* Fixed addresses */
extern void *kernel_addr, *update_addr, *dts_addr;

void* hal_get_primary_address(void)
{
#ifdef NXP_IMX8_KERNEL_ADDR
    /* Signed kernel image placed by the previous stage (e.g. U-Boot SPL
     * FIT loadable when wolfBoot runs as BL33) */
    return (void*)NXP_IMX8_KERNEL_ADDR;
#else
    return (void*)&kernel_addr;
#endif
}

void* hal_get_update_address(void)
{
  return (void*)&update_addr;
}

void* hal_get_dts_address(void)
{
  return (void*)&dts_addr;
}

#ifdef EXT_FLASH
int ext_flash_read(unsigned long address, uint8_t *data, int len)
{
    XMEMCPY(data, (void *)address, len);
    return len;
}

int ext_flash_erase(unsigned long address, int len)
{
    XMEMSET((void *)address, 0xFF, len);
    return len;
}

int ext_flash_write(unsigned long address, const uint8_t *data, int len)
{
    XMEMCPY((void *)address, data, len);
    return len;
}

void ext_flash_lock(void)
{
}

void ext_flash_unlock(void)
{
}

#endif


#ifdef DEBUG_UART
/* i.MX8M UART (polling TX only).
 * Clocks, pinmux and baud rate (115200 8N1) are already configured by
 * U-Boot, so only the transmit path is implemented here.
 * Default: UART2 (0x30890000), the PICO-PI console (ttymxc1).
 */
#ifndef NXP_IMX8_UART_BASE
#define NXP_IMX8_UART_BASE  0x30890000UL
#endif
#define IMX_UART_REG(off)   (*(volatile uint32_t *)(NXP_IMX8_UART_BASE + (off)))
#define IMX_UART_UTXD       IMX_UART_REG(0x40)  /* Transmitter register */
#define IMX_UART_UCR1       IMX_UART_REG(0x80)  /* Control register 1 */
#define IMX_UART_USR2       IMX_UART_REG(0x98)  /* Status register 2 */
#define IMX_UART_UTS        IMX_UART_REG(0xB4)  /* Test register */

#define UCR1_UARTEN         (1U << 0)
#define USR2_TXDC           (1U << 3)  /* Transmission complete */
#define UTS_TXFULL          (1U << 4)  /* TxFIFO full */

void uart_init(void)
{
    /* Configured by U-Boot; just make sure the UART is enabled */
    IMX_UART_UCR1 |= UCR1_UARTEN;
}

static void uart_putc(char c)
{
    while (IMX_UART_UTS & UTS_TXFULL)
        ;
    IMX_UART_UTXD = (uint32_t)(uint8_t)c;
}

void uart_write(const char* buf, unsigned int sz)
{
    unsigned int i;
    for (i = 0; i < sz; i++) {
        if (buf[i] == '\n')
            uart_putc('\r');
        uart_putc(buf[i]);
    }
    /* Drain the FIFO so nothing is lost when the kernel takes over */
    while (!(IMX_UART_USR2 & USR2_TXDC))
        ;
}
#endif /* DEBUG_UART */

void* hal_get_dts_update_address(void)
{
  return NULL; /* Not yet supported */
}

/* public HAL functions */
#ifdef NXP_IMX8_BL33
/* BL33 boot: ATF enters wolfBoot at EL2 with the MMU and caches off, so every
 * instruction fetch goes to DDR. The I-cache can be enabled without the MMU
 * (instruction fetches are then cacheable); this greatly speeds up the hash
 * and signature verification. do_boot() -> el2_flush_and_disable_mmu()
 * invalidates and disables it again before jumping to Linux.
 * Only done when the MMU is off (when started from U-Boot proper, U-Boot has
 * already enabled the MMU and caches and nothing is changed). */
static void nxp_imx8_icache_enable(void)
{
    uint64_t sctlr;

    __asm__ volatile("mrs %0, sctlr_el2" : "=r"(sctlr));
    if ((sctlr & (1UL << 0)) == 0) {            /* SCTLR_EL2.M: MMU off */
        __asm__ volatile("ic iallu\n"
                         "dsb sy\n"
                         "isb\n" ::: "memory");
        sctlr |= (1UL << 12);                   /* SCTLR_EL2.I */
        __asm__ volatile("msr sctlr_el2, %0\n"
                         "isb\n" :: "r"(sctlr) : "memory");
    }
}
#endif

void hal_init(void)
{
#ifdef NXP_IMX8_BL33
    nxp_imx8_icache_enable();
#endif
#ifdef DEBUG_UART
    uart_init();
#endif
    #if defined(TEST_ENCRYPT) && defined (EXT_ENCRYPTED)
    char enc_key[] = "0123456789abcdef0123456789abcdef"
        "0123456789abcdef";
    wolfBoot_set_encrypt_key((uint8_t *)enc_key,(uint8_t *)(enc_key +  32));
    #endif
}

/* MMU/cache teardown for the Linux arm64 boot protocol is done by do_boot()
 * via el2_flush_and_disable_mmu() (EL2_HYPERVISOR=1 from hal/nxp_imx8.h).
 * It runs after do_boot()'s last wolfBoot_printf(), so U-Boot printf is
 * never called with the MMU off. Nothing to do here.
 */
void hal_prepare_boot(void)
{
}


int RAMFUNCTION hal_flash_write(uintptr_t address, const uint8_t *data, int len)
{
    return 0;
}

void RAMFUNCTION hal_flash_unlock(void)
{
}

void RAMFUNCTION hal_flash_lock(void)
{
}


int RAMFUNCTION hal_flash_erase(uintptr_t address, int len)
{
    return 0;
}
