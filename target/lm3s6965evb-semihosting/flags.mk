CC=arm-none-eabi-gcc
AR=arm-none-eabi-gcc-ar
CFLAGS_FLTO=-flto
CFLAGS_TARGET=-mcpu=cortex-m3 -Os $(CFLAGS_FLTO) -ffreestanding --specs=nano.specs --specs=rdimon.specs
LDFLAGS_TARGET=-T $(TARGET_SPEC)/link.ld
LDLIBS_TARGET=-lc -lc -lrdimon
STARTUP=-D__STARTUP_CLEAR_BSS -D__START=main /usr/share/doc/gcc-arm-none-eabi/examples/startup/startup_ARMCM3.S
QEMU_CMD=qemu-system-arm -M lm3s6965evb -cpu cortex-m3 -nographic -monitor null -serial null -semihosting -kernel
EXT=.axf

CFLAGS+=$(CFLAGS_TARGET)
LDFLAGS+=$(LDFLAGS_TARGET)
LDLIBS+=$(LDLIBS_TARGET)
