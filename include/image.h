#ifndef IMAGE_H
#define IMAGE_H
#include <stdint.h>
#include <target.h>
#include <wolfboot/wolfboot.h>

#if defined(__WOLFBOOT) && defined(RAM_CODE)
#  if defined(ARCH_ARM)
#    define RAMFUNCTION __attribute__((used,section(".ramcode"),long_call))
#  else
#    define RAMFUNCTION __attribute__((used,section(".ramcode")))
#  endif
#else
# define RAMFUNCTION
#endif


#define SECT_FLAG_NEW 0x0F
#define SECT_FLAG_SWAPPING 0x07
#define SECT_FLAG_BACKUP 0x03
#define SECT_FLAG_UPDATED 0x00


struct wolfBoot_image {
    uint8_t *hdr;
    uint8_t *trailer;
    int hdr_ok;
    int signature_ok;
    int sha_ok;
    uint8_t *fw_base;
    uint32_t fw_size;
    uint8_t part;
};


int wolfBoot_open_image(struct wolfBoot_image *img, uint8_t part);
int wolfBoot_verify_integrity(struct wolfBoot_image *img);
int wolfBoot_verify_authenticity(struct wolfBoot_image *img);
int wolfBoot_set_partition_state(uint8_t part, uint8_t newst);
int wolfBoot_set_sector_flag(uint8_t part, uint8_t sector, uint8_t newflag);
int wolfBoot_get_partition_state(uint8_t part, uint8_t *st);
int wolfBoot_get_sector_flag(uint8_t part, uint8_t sector, uint8_t *flag);

/* Defined in libwolfboot */
uint8_t wolfBoot_find_header(uint8_t *haystack, uint8_t type, uint8_t **ptr);

#ifdef EXT_FLASH
# ifdef PART_UPDATE_EXT
#  define UPDATE_EXT 1
# else
#  define UPDATE_EXT 0
# endif
# ifdef PART_SWAP_EXT
#  define SWAP_EXT 1
# else
#  define SWAP_EXT 0
# endif
# define PART_IS_EXT(x) (((x)->part == PART_UPDATE)?UPDATE_EXT:(((x)->part == PART_SWAP)?SWAP_EXT:0))
#include "hal.h"

static inline int wb_flash_erase(struct wolfBoot_image *img, uint32_t off, uint32_t size)
{
    if (PART_IS_EXT(img))
        return ext_flash_erase((uint32_t)(img->hdr) + off, size);
    else
        return hal_flash_erase((uint32_t)(img->hdr) + off, size);
}

static inline int wb_flash_write(struct wolfBoot_image *img, uint32_t off, const void *data, uint32_t size)
{
    if (PART_IS_EXT(img))
        return ext_flash_write((uint32_t)(img->hdr) + off, data, size);
    else
        return hal_flash_write((uint32_t)(img->hdr) + off, data, size);
}

static inline int wb_flash_write_verify_word(struct wolfBoot_image *img, uint32_t off, uint32_t word)
{
    int ret;
    volatile uint32_t copy;
    if (PART_IS_EXT(img)) 
    {
        ext_flash_read((uint32_t)(img->hdr) + off, (void *)&copy, sizeof(uint32_t));
        while (copy != word) {
            ret = ext_flash_write((uint32_t)(img->hdr) + off, (void *)&word, sizeof(uint32_t));
            if (ret < 0)
                return ret;
            ext_flash_read((uint32_t)(img->hdr) + off, (void *)&copy, sizeof(uint32_t));
        }
    } else {
        volatile uint32_t *pcopy = (volatile uint32_t*)(img->hdr + off);
        while(*pcopy != word) {
            hal_flash_write((uint32_t)pcopy, (void *)&word, sizeof(uint32_t));
        }
    }
    return 0;
}


#else 
# define PART_IS_EXT(x) (0)
# define wb_flash_erase(im, of, siz)  hal_flash_erase(((uint32_t)(((im)->hdr)) + of), siz)
# define wb_flash_write(im, of, dat, siz)  hal_flash_write(((uint32_t)((im)->hdr)) + of, dat, siz)
#endif /* EXT_FLASH */

#endif /* IMAGE_H */
