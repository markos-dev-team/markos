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
LIB=libmarkos.a
OBJ=context_ucontext.o context_setjmp.o task_list.o task.o irq.o mailbox.o fifo.o itron.o

CFLAGS_USE_DEBUGF=-DUSE_DEBUGF
ifneq '$(DEBUGF)' ''
CFLAGS += $(CFLAGS_USE_DEBUGF)
endif

CFLAGS_NDEBUG=-DNDEBUG
ifneq '$(NDEBUG)' ''
CFLAGS += $(CFLAGS_NDEBUG)
endif

CFLAGS_INCLUDE=-I ../include
CFLAGS += $(CFLAGS_INCLUDE)

# Compile, product exe.
compile: $(LIB)

.SECONDARY: $(OBJ)

$(LIB) : $(OBJ)
	$(AR) crs $@ $^ 

EXE_ALL = $(EXE) $(EXE_ALT)
clean:
	rm -f $(OBJ) $(LIB) $(EXE_ALL) $(EXE_ALL:%.axf=%) $(EXE_ALL:%.axf=%.o) $(EXE_ALL:%.axf=%.map) hello.map
