#
# This configuration files extends the default Makefiles to work with mbedtls
#
CC_FLAGS+= \
-fomit-frame-pointer \
-g \
-nostdlib \
-nostartfiles \
-O0 \
-fmessage-length=0 \
-fsigned-char \
-ffunction-sections \
-fdata-sections \
-fno-common \
-ffreestanding \
-Wall \
-Wextra \
-nostartfiles \
-Xlinker \
--gc-sections \
-Wl,-Map,"mpi_bugnum.map"

# The thumb version does not give the expected result
# The same problem has been observed when executing code on the device
#-DUSE_THUMB

LD           = arm-none-eabi-gcc

LD_FLAGS+= -lc -lgcc

LINKER_SCRIPT=./link.ld
