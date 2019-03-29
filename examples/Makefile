# Copyright (C) 2019 Marko Nippula
# All Rights Reserved.
#
# Build instructions.

# Default target is all, which will compile.
all: compile

ifeq '$(TARGET)' 'lm' # Short
override TARGET=lm3s6965evb-semihosting
endif

ifeq '$(TARGET)' ''
TARGET=native
# Other options that have been tested:
#TARGET=lm3s6965evb-semihosting
$(message using $(TARGET) as target, because no target was specified.)
endif
TARGET_SPEC=../target/$(TARGET)
include $(TARGET_SPEC)/flags.mk
EXE=$(EXE_ALL)
EXE_ALL_TEMPLATE=triple_hello.ext delay_hello.ext priority_hello.ext hello.ext native_hello.ext
EXE_ALL=$(foreach target,$(EXE_ALL_TEMPLATE),$(target:%.ext=%)$(EXT))

LDFLAGS_KERNEL=-L ../src
LDLIBS_KERNEL=-lmarkos
LDFLAGS += $(LDFLAGS_KERNEL)
LDLIBS += $(LDLIBS_KERNEL)

CFLAGS_INCLUDE = -I ../include
CFLAGS += $(CFLAGS_INCLUDE)

CFLAGS_USE_DEBUGF=-DUSE_DEBUGF
ifneq '$(DEBUGF)' ''
CFLAGS += $(CFLAGS_USE_DEBUGF)
endif

CFLAGS_NDEBUG=-DNDEBUG
ifneq '$(NDEBUG)' ''
CFLAGS += $(CFLAGS_NDEBUG)
endif


# Compile, product exe.
compile: $(EXE)

.SECONDARY: $(EXE_ALL_TEMPLATE:%.ext=%.o) $(EXE_ALL)

# Provide target that will run programs. For native binaries running binaries is easy, but this make target
# is mainly to help with platforms that require execution via qemu.
run:
	for x in $(EXE); do $(QEMU_CMD) ./$$x;done 

%.axf : %.o
	$(CC) -o $@ $(STARTUP) $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

% : %.o
	$(CC) -o $@ $(STARTUP) $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(OBJ) $(EXE_ALL_TEMPLATE) $(EXE_ALL) $(EXE_ALL_TEMPLATE:%.ext=%.axf) $(EXE_ALL_TEMPLATE:%.ext=%.o) $(EXE_ALL_TEMPLATE:%.ext=%.map)
