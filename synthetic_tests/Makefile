# Define version and name

LLVM_VERSION=3.6

TARGET=TEST_CASE

OUTPUT=$(TARGET)-llvm-$(LLVM_VERSION)

# Toolchain Path

TOOL_PATH=

# Toolchain

IR           = $(TOOL_PATH)clang
#CC          = $(TOOL_PATH)clang
#CXX         = $(TOOL_PATH)clang++
CC           = arm-none-eabi-gcc
CXX          = arm-none-eabi-g++
LLVM_LINK    = $(TOOL_PATH)llvm-link
LLVM_AS      = $(TOOL_PATH)llvm-as
#AS           = $(TOOL_PATH)clang
AS           = arm-none-eabi-as
IC           = inception-cl
LD           = arm-none-eabi-ld
OBJDUMP      = arm-none-eabi-objdump
SIZE         = arm-none-eabi-size

# Arguments

COMMON_ARGS  = -target arm-none-eabi -mcpu=cortex-m3 -O0 -mfloat-abi=soft
GCC_ARGS     = -march=armv7-m -mthumb -mcpu=cortex-m3 -Wa,-mimplicit-it=thumb

CC_FLAGS     = $(GCC_ARGS)
CXX_ARGS     = $(GCC_ARGS)
ASM_FLAGS    = -mcpu=cortex-m3 -mthumb -mfloat-abi=softfp
#ASM_FLAGS    = $(COMMON_ARGS) -nostdlib
LD_FLAGS     =
IR_FLAGS     = $(COMMON_ARGS) -emit-llvm -S -g -DKLEE
IC_FLAGS     = -disable-interrupt

# Linker Option

LINKED       =

LINKER_SCRIPT= ../../link.ld

# Include sub config

-include config.mk

# Linker Option suite

LD_FLAGS     += -T $(LINKER_SCRIPT)

# Include

IR_FLAGS     += -I/usr/include/newlib/
#IR_FLAGS     += -I

# Define sources

C_FILES      := $(shell find ./ -type f -name "*.c" -printf "%p\n")
CXX_FILES    := $(shell find ./ -type f -name "*.cpp" -printf "%p\n")
ASM_FILES    := $(shell find ./ -type f -name "*.s" -printf "%p\n")

HEADER       := $(shell find ./ -type f -name "*.h" -exec dirname {} \; | sort -u | sed ':a;N;$$!ba;s/\n/ -I/g' | xargs -0 printf "%s")

C_OBJS        = $(patsubst %.c,%.o,$(C_FILES))
CXX_OBJS      = $(patsubst %.cpp,%.o,$(CXX_FILES))
ASM_OBJS      = $(patsubst %.s,%.o,$(ASM_FILES))

LL_OBJS       = $(patsubst %.c,%.ll,$(C_FILES))
LL_OBJS      += $(patsubst %.cpp,%.ll,$(CXX_FILES))

OBJS          = $(C_OBJS) $(CXX_OBJS) $(ASM_OBJS)

# Verbose

V = 0
ifeq ($V, 0)
        Q = @
        P = 2>&1 >/dev/null
endif

#################### Rules ###########################
######################################################

all: BIN IR

BIN: $(C_OBJS) $(CXX_OBJS) $(ASM_OBJS) ELF

IR: $(LL_OBJS) BC ADAPT_BC_ASM

# Compilation rules

%.ll : %.cpp
	@echo "[LL]      $(notdir $<)"
	$Q$(IR) $(IR_FLAGS) -o $@ -c $<

%.ll : %.c
	@echo "[LL]      $(notdir $<)"
	$(IR) $(IR_FLAGS) -o $@ -c $<

%.o : %.c
	@echo "[CC]      $(notdir $<)"
	$Q$(CC) $(CC_FLAGS) $(DEBUG) -o $@ -c $<

%.o : %.s
	@echo "[AS]      $(notdir $<)"
	$Q$(AS) $(ASM_FLAGS) -o $@ -c $<

%.o : %.cpp
	@echo "[CXX]      $(notdir $<)"
	$Q$(CXX) $(CXX_FLAGS) -c -o $@ $< -MMD -MF dep/$(*F).d

# Link rules

ELF: $(OBJS)
	@echo "[LD]      $(OUTPUT).elf"
	$Q$(LD) $(LD_FLAGS) $^ -o $(OUTPUT).elf
	@echo "[OBJDUMP] $(OUTPUT).lst"
	$Q$(OBJDUMP) -St $(OUTPUT).elf >$(TARGET).lst
	@echo "[SIZE]    $(OUTPUT).elf"
	$Q$(SIZE) $(OUTPUT).elf

BC: $(LL_OBJS)
	@echo "[LLVM-LINK]      $(OUTPUT).ll"
	$Q$(LLVM_LINK) -S $^ -o $(OUTPUT).ll
	sed -i 's/ linkonce_odr / /g' $(OUTPUT).ll
	sed -i 's/ internal / /g' $(OUTPUT).ll
	sed -ri 's/external global ([\._%0-9a-zA-Z]*)\*/ global \1\* undef/g' $(OUTPUT).ll
	sed -ri 's/external global ([0-9a-zA-Z]*)/ global \1 0/g' $(OUTPUT).ll
	sed -ri 's/extern_weak global ([0-9a-zA-Z]*)/ global \1 0/g' $(OUTPUT).ll
	echo "[LLVM-AS]       $(OUTPUT).ll"
	$Q$(LLVM_AS) $(OUTPUT).ll -o $(OUTPUT).bc

# Clean rules

clean:
	$Qrm -rf *.o $P
	$Qrm -rf *.ll $P
	$Qrm -rf klee-* $P
	$Qrm -rf *.ll $P

# Inception-Compiler rules

ADAPT_BC_ASM:
	@echo "[INCEPTION-COMPILER]      $(OUTPUT).bc"
	$Q$(IC) $(IC_FLAGS) $(OUTPUT).elf $(OUTPUT).bc $P
	@echo "[LLVM-AS]       $(OUTPUT).ll"
	$Q$(LLVM_AS) $(OUTPUT).elf.ll -o $(OUTPUT).bc $P
