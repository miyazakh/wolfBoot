test-delta-update:SIGN_ARGS?=--ecc256
test-delta-update:USBTTY?=/dev/ttyACM0
test-delta-update:EXPVER=tools/test-expect-version/test-expect-version /dev/ttyACM0

test-delta-update-ext:SIGN_ARGS?=--ecc256
test-delta-update-ext:USBTTY?=/dev/ttyACM0
test-delta-update-ext:TIMEOUT?=70
test-delta-update-ext:EXPVER=tools/test-expect-version/test-expect-version /dev/ttyACM0

test-delta-enc-update-ext:SIGN_ARGS=--ecc256 --sha256
test-delta-enc-update-ext:USBTTY?=/dev/ttyACM0
test-delta-enc-update-ext:TIMEOUT?=50
test-delta-enc-update-ext:EXPVER=tools/test-expect-version/test-expect-version /dev/ttyACM0
test-delta-enc-update-ext:PART_SIZE=131023
test-delta-enc-update-ext:APP=test-app/image_v7_signed_diff_encrypted.bin

test-delta-update: factory.bin test-app/image.bin tools/uart-flash-server/ufserver tools/delta/bmdiff tools/test-expect-version/test-expect-version
	@killall ufserver || true
	@st-flash reset
	@sleep 2
	@dd if=/dev/zero of=zero.bin bs=4096 count=1
	@st-flash write zero.bin 0x0800B000
	@st-flash reset
	@st-flash write zero.bin 0x0802B000
	@st-flash reset
	@st-flash erase || st-flash erase
	@rm -f zero.bin
	@diff .config config/examples/stm32wb-delta.config || (echo "\n\n*** Error: please copy config/examples/stm32wb-delta.config to .config to run this test\n\n" && exit 1)
	$(SIGN_TOOL) $(SIGN_ARGS) --delta test-app/image_v1_signed.bin test-app/image.bin \
		$(PRIVATE_KEY) 7
	$(SIGN_TOOL) $(SIGN_ARGS) --delta test-app/image_v1_signed.bin test-app/image.bin \
		$(PRIVATE_KEY) 2
	@st-flash write factory.bin 0x08000000
	@echo Expecting version '1'
	@(test `$(EXPVER)` -eq 1) || (st-flash reset && test `$(EXPVER)` -eq 1)
	@echo
	@sleep 2
	@st-flash write test-app/image_v7_signed_diff.bin 0x0802C000
	@sleep 2
	@st-flash reset
	@echo Expecting version '7'
	@(test `$(EXPVER)` -eq 7) || (st-flash reset && test `$(EXPVER)` -eq 7)
	@sleep 2
	@st-flash reset
	@echo Expecting version '1'
	@(test `$(EXPVER)` -eq 1)
	@sleep 2
	@st-flash reset
	@sleep 2
	@st-flash erase || st-flash erase
	@st-flash write factory.bin 0x08000000
	@echo Expecting version '1'
	@test `$(EXPVER)` -eq 1
	@sleep 2
	@st-flash reset
	@sleep 2
	@st-flash write test-app/image_v2_signed_diff.bin 0x0802C000
	@st-flash reset
	@echo Expecting version '2'
	@(test `$(EXPVER)` -eq 2) || (st-flash reset && test `$(EXPVER)` -eq 2)
	@sleep 2
	@st-flash reset
	@echo Expecting version '2'
	@(test `$(EXPVER)` -eq 2)
	@sleep 2
	@st-flash reset
	@echo Expecting version '2'
	@(test `$(EXPVER)` -eq 2)
	@echo "TEST SUCCESSFUL"


test-delta-update-ext: factory.bin test-app/image.bin tools/uart-flash-server/ufserver tools/delta/bmdiff tools/test-expect-version/test-expect-version
	@killall ufserver || true
	@st-flash reset
	@dd if=/dev/zero of=zero.bin bs=4096 count=1
	@st-flash write zero.bin 0x0800B000
	@st-flash reset
	@st-flash write zero.bin 0x0802B000
	@st-flash reset
	@st-flash erase || st-flash erase
	@rm -f zero.bin
	@diff .config config/examples/stm32wb-delta-ext.config || (echo "\n\n*** Error: please copy config/examples/stm32wb-delta-ext.config to .config to run this test\n\n" && exit 1)
	$(SIGN_TOOL) $(SIGN_ARGS) --delta test-app/image_v1_signed.bin test-app/image.bin \
		$(PRIVATE_KEY) 7
	@(tools/uart-flash-server/ufserver test-app/image_v7_signed_diff.bin $(USBTTY))&
	@st-flash reset
	@st-flash write factory.bin 0x08000000
	@st-flash reset
	@sync
	@echo Waiting $(TIMEOUT) seconds...
	@sleep $(TIMEOUT)
	@st-flash reset
	@sleep 10
	@st-flash reset
	@sleep 5
	@st-flash read boot_full.bin 0x0800C000 0x8000
	@SIZE=`wc -c test-app/image_v7_signed.bin | cut -d" " -f 1`;  \
		dd if=boot_full.bin of=boot.bin bs=1 count=$$SIZE
	@diff boot.bin test-app/image_v7_signed.bin || (echo "TEST FAILED" && exit 1)
	@rm boot.bin boot_full.bin
	@echo "TEST SUCCESSFUL"
	@sleep 1
	@echo
	@echo
	@echo TEST INVERSE
	@st-flash reset
	@sleep 1
	@st-flash reset
	@sleep $(TIMEOUT)
	@st-flash reset
	@killall ufserver
	@st-flash reset
	@st-flash read boot_full.bin 0x0800C000 0x8000
	@SIZE=`wc -c test-app/image_v1_signed.bin | cut -d" " -f 1`;  \
		dd if=boot_full.bin of=boot.bin bs=1 count=$$SIZE
	@diff boot.bin test-app/image_v1_signed.bin || (echo "TEST INVERSE FAILED" && exit 1)
	@rm boot.bin boot_full.bin
	@echo "TEST SUCCESSFUL"

test-delta-enc-update-ext: factory.bin test-app/image.bin tools/uart-flash-server/ufserver tools/delta/bmdiff tools/test-expect-version/test-expect-version
	@killall ufserver || true
	@st-flash reset
	@dd if=/dev/zero of=zero.bin bs=4096 count=1
	@st-flash write zero.bin 0x0800B000
	@st-flash reset
	@st-flash write zero.bin 0x0802B000
	@st-flash reset
	@st-flash erase || st-flash erase
	@rm -f zero.bin
	@diff .config config/examples/stm32wb-delta-enc-ext.config || (echo "\n\n*** Error: please copy config/examples/stm32wb-delta-enc-ext.config to .config to run this test\n\n" && exit 1)
	$(SIGN_TOOL) $(SIGN_ARGS) --delta test-app/image_v1_signed.bin \
		--encrypt enc_key.der \
		test-app/image.bin \
		$(PRIVATE_KEY) 7
	@(tools/uart-flash-server/ufserver test-app/image_v7_signed_diff_encrypted.bin $(USBTTY))&
	@st-flash write factory.bin 0x08000000
	@sync
	@sleep 5
	@st-flash reset
	@echo Waiting $(TIMEOUT) seconds...
	@sleep $(TIMEOUT)
	@st-flash reset
	@st-flash read boot_full.bin 0x0800C000 0x8000
	@SIZE=`wc -c test-app/image_v7_signed.bin | cut -d" " -f 1`;  \
		dd if=boot_full.bin of=boot.bin bs=1 count=$$SIZE
	@diff boot.bin test-app/image_v7_signed.bin || (echo "TEST FAILED" && exit 1)
	@rm boot.bin boot_full.bin
	@echo "TEST SUCCESSFUL"
	@sleep 1
	@echo
	@echo
	@st-flash reset
	@echo TEST INVERSE
	@sleep $(TIMEOUT)
	@st-flash reset
	@killall ufserver
	@st-flash reset
	@st-flash read boot_full.bin 0x0800C000 0x8000
	@SIZE=`wc -c test-app/image_v1_signed.bin | cut -d" " -f 1`;  \
		dd if=boot_full.bin of=boot.bin bs=1 count=$$SIZE
	@diff boot.bin test-app/image_v1_signed.bin || (echo "TEST INVERSE FAILED" && exit 1)
	@rm boot.bin boot_full.bin
	@echo "TEST SUCCESSFUL"


