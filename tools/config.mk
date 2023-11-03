#Defaults, used by CI
ifeq ($(ARCH),)
  ARCH?=ARM
  TARGET?=stm32f4
  SIGN?=ED25519
  HASH?=SHA256
  MCUXSDK?=1
  MCUXPRESSO?=$(PWD)/mcux-sdk
  MCUXPRESSO_CPU=MK64FN1M0VLL12
  MCUXPRESSO_DRIVERS?=$(MCUXPRESSO)/devices/MK64F12
  MCUXPRESSO_CMSIS?=$(PWD)/CMSIS_5/CMSIS
  FREEDOM_E_SDK?=$(HOME)/src/freedom-e-sdk
  STM32CUBE?=$(HOME)/STM32Cube/Repository/STM32Cube_FW_WB_V1.3.0
  CYPRESS_PDL?=$(HOME)/src/psoc6pdl
  CYPRESS_TARGET_LIB?=$(HOME)/src/TARGET_CY8CKIT-062S2-43012
  CYPRESS_CORE_LIB?=$(HOME)/src/cypress-core-lib
  DEBUG?=0
  VTOR?=1
  CORTEX_M0?=0
  CORTEX_M33?=0
  CORTEX_M7?=0
  CORTEX_M3?=0
  NO_ASM?=0
  EXT_FLASH?=0
  SPI_FLASH?=0
  QSPI_FLASH?=0
  NO_XIP?=0
  UART_FLASH?=0
  ALLOW_DOWNGRADE?=0
  NVM_FLASH_WRITEONCE?=0
  DISABLE_BACKUP?=0
  WOLFBOOT_VERSION?=0
  V?=0
  LMS_LEVELS?=0
  LMS_HEIGHT?=0
  LMS_WINTERNITZ?=0
  XMSS_PARAMS?=XMSS-SHA2_10_256
  NO_MPU?=0
  ENCRYPT?=0
  ENCRYPT_WITH_CHACHA?=0
  ENCRYPT_WITH_AES128?=0
  ENCRYPT_WITH_AES256?=0
  FLAGS_HOME?=0
  FLAGS_INVERT?=0
  SPMATH?=1
  SPMATHALL?=0
  RAM_CODE?=0
  DUALBANK_SWAP?=0
  IMAGE_HEADER_SIZE?=256
  PKA?=1
  PSOC6_CRYPTO?=1
  WOLFTPM?=0
  WOLFBOOT_TPM_VERIFY?=0
  MEASURED_BOOT?=0
  WOLFBOOT_TPM_SEAL?=0
  WOLFBOOT_TPM_KEYSTORE?=0
  WOLFBOOT_UNIVERSAL_KEYSTORE?=0
  TZEN?=0
  WOLFCRYPT_TZ?=0
  WOLFCRYPT_TZ_PKCS11?=0
  WOLFBOOT_PARTITION_SIZE?=0x20000
  WOLFBOOT_SECTOR_SIZE?=0x20000
  WOLFBOOT_PARTITION_BOOT_ADDRESS?=0x08020000
  WOLFBOOT_PARTITION_UPDATE_ADDRESS?=0x08040000
  WOLFBOOT_PARTITION_SWAP_ADDRESS?=0x08060000
  WOLFBOOT_DTS_BOOT_ADDRESS?=0x30000
  WOLFBOOT_DTS_UPDATE_ADDRESS=0x50000
  WOLFBOOT_LOAD_ADDRESS?=0x200000
  WOLFBOOT_LOAD_DTS_ADDRESS?=0x400000
  WOLFBOOT_SMALL_STACK?=0
  DELTA_UPDATES?=0
  DELTA_BLOCK_SIZE?=256
  WOLFBOOT_HUGE_STACK?=0
  ARMORED?=0
  ELF?=0
  FORCE_32BIT=0
  DISK_LOCK?=0
  DISK_LOCK_PASSWORD?=
endif

CONFIG_VARS:= ARCH TARGET SIGN HASH MCUXSDK MCUXPRESSO MCUXPRESSO_CPU MCUXPRESSO_DRIVERS \
	MCUXPRESSO_CMSIS FREEDOM_E_SDK STM32CUBE CYPRESS_PDL CYPRESS_CORE_LIB CYPRESS_TARGET_LIB DEBUG VTOR \
	CORTEX_M0 CORTEX_M7 CORTEX_M33 NO_ASM EXT_FLASH SPI_FLASH NO_XIP UART_FLASH ALLOW_DOWNGRADE NVM_FLASH_WRITEONCE \
	DISABLE_BACKUP WOLFBOOT_VERSION V NO_MPU ENCRYPT FLAGS_HOME FLAGS_INVERT \
	SPMATH SPMATHALL RAM_CODE DUALBANK_SWAP IMAGE_HEADER_SIZE PKA TZEN PSOC6_CRYPTO \
    WOLFTPM WOLFBOOT_TPM_VERIFY MEASURED_BOOT WOLFBOOT_TPM_SEAL WOLFBOOT_TPM_KEYSTORE \
	WOLFCRYPT_TZ WOLFCRYPT_TZ_PKCS11 \
	WOLFBOOT_PARTITION_SIZE WOLFBOOT_SECTOR_SIZE  \
	WOLFBOOT_PARTITION_BOOT_ADDRESS WOLFBOOT_PARTITION_UPDATE_ADDRESS \
	WOLFBOOT_PARTITION_SWAP_ADDRESS WOLFBOOT_LOAD_ADDRESS \
	WOLFBOOT_LOAD_DTS_ADDRESS WOLFBOOT_DTS_BOOT_ADDRESS WOLFBOOT_DTS_UPDATE_ADDRESS \
	WOLFBOOT_SMALL_STACK DELTA_UPDATES DELTA_BLOCK_SIZE \
	WOLFBOOT_HUGE_STACK FORCE_32BIT\
	ENCRYPT_WITH_CHACHA ENCRYPT_WITH_AES128 ENCRYPT_WITH_AES256 ARMORED \
	LMS_LEVELS LMS_HEIGHT LMS_WINTERNITZ \
	WOLFBOOT_UNIVERSAL_KEYSTORE \
	XMSS_PARAMS \
	ELF
