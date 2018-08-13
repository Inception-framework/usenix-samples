
#
#	Adjust for your system!
#
#	Common options for generic UNIX and Microsoft C (under DOS)
#	are listed here.  You can change them by switching the order,
#	placing the ones you want last.  Pay particular attention to
#	the HZ parameter, which may or may not be listed in some
#	header file on your system, such as <sys/param.h> or <limits.h>
#	(as CLK_TCK).  Even if it is listed, it may be incorrect.
#	Also, some operating systems (notably some (all?) versions
#	of Microport UNIX) lie about the time.  Sanity check with a
#	stopwatch.
#
#	For Microsoft C under DOS, you need a real make, not MSC make,
#	to run this Makefile.  The public domain "ndmake" will suffice.
#
CC=		arm-none-eabi-gcc			# C compiler name goes here (MSC)
#CC=		cl			# C compiler name goes here (UNIX)
GCC=		arm-none-eabi-gcc
CLANG=          clang
LINKER=         llvm-link
AS=             llvm-as

PROGS=		msc			# Programs to build (MSC)
PROGS=		unix			# Programs to build (UNIX)

#TIME_FUNC=	-DMSC_CLOCK		# Use Microsoft clock() for measurement
TIME_FUNC=	-DTIME			# Use time(2) for measurement
#TIME_FUNC=	-DTIMES			# Use times(2) for measurement
#HZ=		50			# Frequency of times(2) clock ticks
#HZ=		60			# Frequency of times(2) clock ticks
#HZ=		100			# Frequency of times(2) clock ticks
#HZ=		1			# Give bogus result unless changed!

STRUCTASSIGN=	-DNOSTRUCTASSIGN	# Compiler cannot assign structs
STRUCTASSIGN=				# Compiler can assign structs

ENUMS=		-DNOENUMS		# Compiler doesn't have enum type
ENUMS=					# Compiler does have enum type

#OPTIMIZE=	-Ox -G2			# Optimization Level (MSC, 80286)
OPTIMIZE=	-O4			# Optimization Level (generic UNIX)
GCCOPTIM=       -O

LFLAGS=-Tstm32l152xe.ld			#Loader Flags

CLANG_FLAGS=-mthumb --target=thumbv7m-eabi -mcpu=cortex-m3
CLANG_FLAGS    += -emit-llvm -g -S
CLANG_FLAGS    += ../../Analyzer/include/
CLANG_FLAGS += $(GCCOPTIM) $(TIME_FUNC) -DHZ=$(HZ) $(ASM) $(ENUMS) $(STRUCTASSIGN) $(CFL)
CLANG_FLAGS += -DKLEE

CFLAGS=	 $(OPTIMIZE) $(TIME_FUNC) -DHZ=$(HZ) $(ASM) $(ENUMS) $(STRUCTASSIGN) $(CFL)

CFLAGS = -nostdlib -nostartfiles -fno-common
CFLAGS  +=-I/usr/include/newlib/ -g -march=armv7-m -mthumb -mcpu=cortex-m3 -Wa,-mimplicit-it=thumb
GCCFLAGS= $(CFLAGS) $(GCCOPTIM) $(TIME_FUNC) -DHZ=$(HZ) $(ENUMS) $(STRUCTASSIGN) $(CFL)

#
#		You shouldn't need to touch the rest
#
SRC=		dhry_1.c dhry_2.c
HDR=		dhry.h
LLFILES=        $(patsubst %.c,%.ll,$(SRC))

UNIX_PROGS=	cc_dry2 cc_dry2reg gcc_dry2 gcc_dry2reg
MSC_PROGS=	sdry2.exe sdry2reg.exe mdry2.exe mdry2reg.exe \
		ldry2.exe ldry2reg.exe cdry2.exe cdry2reg.exe \
		hdry2.exe hdry2reg.exe

# Files added by rer:
FILES1=		README.RER clarify.doc Makefile submit.frm pure2_1.dif \
		dhry_c.dif
# Reinhold's files:
FILES2=		README RATIONALE $(HDR) $(SRC)
FILES3=		dhry.p

all:	$(PROGS)

unix:	$(UNIX_PROGS)

msc:	$(MSC_PROGS)

klee_dry2:		$(SRC) $(HDR)
		$(CLANG) $(CLANG_FLAGS) $(SRC)
		$(LINKER) -S $(LLFILES)  -o $@.ll
		$(AS) $@.ll -o $@.bc
		inception-cl gcc_dry2 $@.bc
		$(AS) gcc_dry2.ll -o $@_merged.bc

run-klee:		klee_dry2
		echo $(TIMES) | klee --allocate-determ --allocate-determ-start-address 0xf0000000 klee_dry2_merged.bc

clean-klee:
		rm klee_dry2.bc

cc_dry2:		$(SRC) $(HDR)
		$(CC) $(CFLAGS) $(SRC) $(LFLAGS) -o $@

cc_dry2reg:	$(SRC) $(HDR)
		$(CC) $(CFLAGS) -DREG=register $(SRC) $(LFLAGS) -o $@

gcc_dry2:		$(SRC) $(HDR)
		$(GCC) $(GCCFLAGS) $(SRC) $(LFLAGS) -o $@

gcc_dry2reg:	$(SRC) $(HDR)
		$(GCC) $(GCCFLAGS) -DREG=register $(SRC) $(LFLAGS) -o $@

sdry2.exe:	$(SRC) $(HDR)
		$(CC) -AS $(CFLAGS) $(SRC) $(LFLAGS) -o $@

sdry2reg.exe:	$(SRC) $(HDR)
		$(CC) -AS $(CFLAGS) -DREG=register $(SRC) $(LFLAGS) -o $@

mdry2.exe:	$(SRC) $(HDR)
		$(CC) -AM $(CFLAGS) $(SRC) $(LFLAGS) -o $@

mdry2reg.exe:	$(SRC) $(HDR)
		$(CC) -AM $(CFLAGS) -DREG=register $(SRC) $(LFLAGS) -o $@

ldry2.exe:	$(SRC) $(HDR)
		$(CC) -AL $(CFLAGS) $(SRC) $(LFLAGS) -o $@

ldry2reg.exe:	$(SRC) $(HDR)
		$(CC) -AL $(CFLAGS) -DREG=register $(SRC) $(LFLAGS) -o $@

cdry2.exe:	$(SRC) $(HDR)
		$(CC) -AC $(CFLAGS) $(SRC) $(LFLAGS) -o $@

cdry2reg.exe:	$(SRC) $(HDR)
		$(CC) -AC $(CFLAGS) -DREG=register $(SRC) $(LFLAGS) -o $@

hdry2.exe:	$(SRC) $(HDR)
		$(CC) -AH $(CFLAGS) $(SRC) $(LFLAGS) -o $@

hdry2reg.exe:	$(SRC) $(HDR)
		$(CC) -AH $(CFLAGS) -DREG=register $(SRC) $(LFLAGS) -o $@

shar:	dry2shar.1 dry2shar.2 dry2shar.3

dry2.arc:	$(FILES1) $(FILES2) $(FILES3)
	arc a dry2.arc $(FILES1) $(FILES2) $(FILES3)

dry2shar.1: $(FILES1)
	shar -vc -p X $(FILES1) >$@

dry2shar.2: $(FILES2)
	shar -vc -p X $(FILES2) >$@

dry2shar.3: $(FILES3)
	shar -v -p X $(FILES3) >$@

clean:
	-rm -f *.o *.obj
	-rm gcc_dry2

clobber: clean
	-rm -f $(UNIX_PROGS) $(MSC_PROGS) dry2shar.* dry2.arc

post:	dry2shar.1	dry2shar.2	dry2shar.3
	for i in 1 2 3;\
	do\
		cat HEADERS BOILER.$$i dry2shar.$$i |\
		inews -h -t "Dhrystone 2.1 ($$i of 3)" -n comp.arch;\
	done

repost:	dry2shar.1	dry2shar.2	dry2shar.3
	for i in 3;\
	do\
		cat HEADERS BOILER.$$i dry2shar.$$i |\
		inews -h -t "REPOST: Dhrystone 2.1 ($$i of 3)" -n comp.arch;\
	done

mail:	dry2shar.1	dry2shar.2	dry2shar.3
	for i in 1 2 3;\
	do\
		cat BOILER.$$i dry2shar.$$i |\
		mailx -s "Dhrystone 2.1 ($$i of 3)" $(ADDR);\
	done

dos:
	doscopy -a $(FILES1) $(FILES2) $(FILES3) dos!a:
