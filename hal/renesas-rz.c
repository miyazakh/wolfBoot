/* renesas-ra.c
 *
 * Stubs for custom HAL implementation. Defines the 
 * functions used by wolfboot for a specific target.
 *
 * Copyright (C) 2024 wolfSSL Inc.
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
#include "user_settings.h"
#include "target.h"
#include "wolfboot/wolfboot.h"
#include "printf.h"

#include "hal_data.h"

#define  BSC_SDRAM_SPACE    (0x30000000)

#ifdef DEBUG_FLASH_WRITE_VERIFY
static uint8_t readbuf[MINIMUM_BLOCK*sizeof(uint32_t)];
#endif


/* #define DEBUG_FLASH_WRITE_VERIFY */

static inline void hal_panic(void)
{
    while(1)
        ;
}

void hal_init(void);
void hal_prepare_boot(void);
int hal_flash_write(uint32_t addr, const uint8_t *data, int len);
int hal_flash_erase(uint32_t address, int int_len);
void hal_flash_unlock(void);
void hal_flash_lock(void);

int ext_flash_read(unsigned long address, uint8_t *data, int len);
int ext_flash_erase(unsigned long address, int len);
int ext_flash_write(unsigned long address, const uint8_t *data, int len);
void ext_flash_lock(void);
void ext_flash_unlock(void);
uint32_t rz_memcopy(uint32_t *src, uint32_t *dst, uint32_t bytesize);

void* hal_get_primary_address(void);
void* hal_get_update_address(void);

static uint32_t rz_memset(uint32_t *dst, uint32_t val, uint32_t bytesize)
{
    uint32_t i;
    uint32_t cnt;

    /* copy count in 4 byte unit */
    cnt = (bytesize + 3) >> 2;

    for (i = 0; i < cnt; i++)
    {
        *dst++ = val;
    }

    /* ensuring data-changing */
    __DSB();

    return bytesize;
}

uint32_t rz_memcopy(uint32_t *src, uint32_t *dst, uint32_t bytesize)
{
    uint32_t i;
    uint32_t cnt;

    /* copy count in 4 byte unit */
    cnt = (bytesize + 3) >> 2;

    for (i = 0; i < cnt; i++)
    {
        *dst++ = *src++;
    }

    /* ensuring data-changing */
    __DSB();

    return bytesize;
}

static uint32_t rz_memwrite(uint32_t *dst, const uint8_t *data, uint32_t bytesize)
{
    uint32_t i;
    uint32_t cnt;
    uint32_t val = 0;
    uint8_t  *p;
    /* copy count in 4 byte unit */
    cnt = (bytesize + 3) >> 2;

    for (i = 0; i < cnt-1; i++)
    {
        *dst++ = *data++;
    }

    if (cnt%4) {
        p = (uint8_t*)data;
        for(i = 0; i < (cnt%4);i++) {
            val |= *p;
            val <<= 2;
        }
        val &= 0xFFFFFFFF;
    }
    /* ensuring data-changing */
    __DSB();

    return bytesize;
}

#ifdef EXT_FLASH

int ext_flash_read(unsigned long address, uint8_t *data, int len)
{
    return (int)rz_memcopy((void*)address, (uint32_t*)data, (uint32_t)len);
}

int ext_flash_erase(unsigned long address, int len)
{
    return rz_memset((void *)address, 0xFFFFFFFF, (size_t)len);
}

int ext_flash_write(unsigned long address, const uint8_t *data, int len)
{
    return rz_memwrite((void *)address, data, (size_t)len);;
}

void ext_flash_lock(void)
{
}

void ext_flash_unlock(void)
{
}

#endif

void hal_init(void)
{
}

void hal_prepare_boot(void)
{
}
/* write data to sdram */
int hal_flash_write(uint32_t addr, const uint8_t *data, int len)
{
    (void)addr;
    (void)data;
    (void)len;
    return 0;
}

/* write data to sdram */
int hal_flash_erase(uint32_t address, int int_len)
{
    (void)address;
    (void)int_len;
    return 0;
}

void hal_flash_unlock(void)
{
    return;
}

void hal_flash_lock(void)
{
    return;
}


void* hal_get_primary_address(void)
{
    return (void*)WOLFBOOT_PARTITION_BOOT_ADDRESS;
}

void* hal_get_update_address(void)
{
    return (void*)WOLFBOOT_PARTITION_UPDATE_ADDRESS;
}

