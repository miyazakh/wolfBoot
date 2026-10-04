/* nxp_imx8.h
 *
 * Copyright (C) 2025 wolfSSL Inc.
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

#ifndef _NXP_IMX8_H_
#define _NXP_IMX8_H_

#define USE_BUILTIN_STARTUP
#define USE_SIMPLE_STARTUP

/* i.MX8MM boots via TF-A (EL3) then U-Boot (EL2).
 * wolfBoot receives control at EL2 from U-Boot "go" command.
 * EL2_HYPERVISOR=1 makes do_boot() call el2_flush_and_disable_mmu()
 * (clean D-cache to PoC, MMU/caches off at EL2) right before jumping to
 * the Linux kernel, as required by the arm64 boot protocol.
 */
#ifndef EL2_HYPERVISOR
#define EL2_HYPERVISOR 1
#endif

#endif /* _NXP_IMX8_H_ */
