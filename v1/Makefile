make = ..\z_tools\make.exe
makefile = $(make) -r
nask = ..\z_tools\nask.exe
edimg = ..\z_tools\edimg.exe

default :
	$(make) img

%.bin : %.nas
	$(nask) $*.nas $*.bin $*.lst

myOS.img : ipl.bin
	$(edimg) imgin:..\z_tools\fdimg0at.tek wbinimg src:ipl.bin len:512 from:0 to:0   imgout:myOS.img

asm :
	$(make) -r ipl.bin

img :
	$(make) -r myOS.img

run :
	$(make) img
	copy myOS.img ..\z_tools\qemu\fdimage0.bin
	$(make) -C ..\z_tools\qemu

install :
	$(make) img
	..\z_tools\imgtol.com w a: myOS.img

clean :
	-del ipl.bin
	-del ipl.lst

clear :
	$(make) clean
	-del myOS.img
