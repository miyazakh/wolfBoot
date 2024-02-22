/* This file is used to include the application image into the loader project. */

.section .IMAGE_APP_FLASH_section, "ax", %progbits
.incbin "../../app_RZ/Debug/app_RZ_v1.0_signed.bin"

