#ifndef H_TARGETS_TARGET_
#define H_TARGETS_TARGET_


/* Example flash partitioning.
 * Ensure that your firmware entry point is
 * at FLASH_AREA_IMAGE_0_OFFSET + 0x100
 */
#   define WOLFBOOT_SECTOR_SIZE			        0x20000
#   define WOLFBOOT_PARTITION_BOOT_ADDRESS      0x20000

#ifdef EXT_FLASH

/* Test configuration with 1MB external memory */
/* (Addresses are relative to the beginning of the ext)*/

#   define WOLFBOOT_PARTITION_SIZE			    0x80000
#   define WOLFBOOT_PARTITION_UPDATE_ADDRESS    0x00000
#   define WOLFBOOT_PARTITION_SWAP_ADDRESS      0x80000

#else

/* Test configuration with internal memory */
#   define WOLFBOOT_PARTITION_SIZE			    0x20000
#   define WOLFBOOT_PARTITION_UPDATE_ADDRESS    0x40000
#   define WOLFBOOT_PARTITION_SWAP_ADDRESS      0x60000
#endif

#endif /* H_TARGETS_TARGET_ */
