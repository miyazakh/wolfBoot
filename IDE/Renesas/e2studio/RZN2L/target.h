/* target.h
 *
 * User configurable build-time options for bootloader and application offsets
 *
 * This is for Renesas RA6M4 board.
 *
 * Copyright (C) 2023 wolfSSL Inc.
 *
 * This file is part of wolfBoot.
 *
 * wolfBoot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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

#ifndef H_TARGETS_TARGET_
#define H_TARGETS_TARGET_

#include "user_settings.h"
#include "bsp_api.h"

#ifndef WOLFBOOT_NO_PARTITIONS
#    define WOLFBOOT_FIXED_PARTITIONS
#endif

extern uint32_t IMAGE_APP_RAM_start;
extern uint32_t IMAGE_APP_FLASH_section_start;
extern uint32_t IMAGE_APP_FLASH_section_size;
extern uint32_t IMAGE_APP_FLASH_update_start;
extern uint32_t IMAGE_APP_FLASH_update_size;

#define WOLFBOOT_FLASH_ADDR                 0x60100000 /* &IMAGE_APP_FLASH_section_start */
#define WOLFBOOT_FLASH_SIZE                 0x100000

#define WOLFBOOT_SECTOR_SIZE                0x10000
#define WOLFBOOT_PARTITION_SIZE             0x60000

#define WOLFBOOT_PARTITION_BOOT_ADDRESS     WOLFBOOT_FLASH_ADDR
#define WOLFBOOT_PARTITION_UPDATE_ADDRESS   (WOLFBOOT_FLASH_ADDR + 0x80000)
#define WOLFBOOT_PARTITION_SWAP_ADDRESS     WOLFBOOT_FLASH_ADDR + 0xf0000


#define WOLFBOOT_LOAD_ADDRESS               0x10010000 /* &IMAGE_APP_RAM_start */

#endif /* !H_TARGETS_TARGET_ */
