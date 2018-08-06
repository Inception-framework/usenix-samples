#
# This configuration files extends the default Makefiles to work with mbedtls
#
CLANG_ARGS+= \
-fomit-frame-pointer \
-g \
-nostdlib \
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
-I/usr/include/newlib/

#-DUSE_THUMB


#CLANG += -DUSE_THUMB


MODE=NEWLIB

LINKER_SCRIPT=link.ld
