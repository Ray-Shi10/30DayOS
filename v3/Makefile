TOOLPATH = ..\z_tools
INCLPATH = ..\z_tools\haribote

MAKE     = $(TOOLPATH)\make.exe -r
NASK     = $(TOOLPATH)\nask.exe
CC1      = $(TOOLPATH)\cc1.exe -I$(INCLPATH)\ -Wall -quiet
GAS2NASK = $(TOOLPATH)\gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)\obj2bim.exe
BIM2HRB  = $(TOOLPATH)\bim2hrb.exe
RULEFILE = $(INCLPATH)\haribote.rul
EDIMG    = $(TOOLPATH)\edimg.exe
IMGTOL   = $(TOOLPATH)\imgtol.com
MAKEFONT = $(TOOLPATH)\makefont.exe
BIN2OBJ  = $(TOOLPATH)\bin2obj.exe

ALL_OBJS = bootpack.obj ioNask.obj eflagsNask.obj hankaku.obj graphic.obj memory.obj memoryNask.obj

default : Makefile
	$(MAKE) img

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

bootpack.bim :  $(ALL_OBJS)	Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map $(ALL_OBJS)
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

haribote.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb haribote.sys

haribote.img : ipl10.bin haribote.sys Makefile
	$(EDIMG) \
		imgin:..\z_tools\fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
		imgout:haribote.img \

%.bin : %.nas Makefile
	$(NASK) $*.nas $*.bin $*.lst

%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

img : haribote.img Makefile

run : Makefile
	$(MAKE) img
	copy haribote.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ..\z_tools\qemu

install : Makefile
	$(MAKE) img
	$(IMGTOL) w a: haribote.img

clean : Makefile
	-del *.bin
	-del *.lst
	-del *.gas
	-del *.obj
	-del bootpack.nas
	-del bootpack.map
	-del bootpack.bim
	-del bootpack.hrb
	-del haribote.sys

clear : clean Makefile
	-del haribote.img

try : run clean Makefile
