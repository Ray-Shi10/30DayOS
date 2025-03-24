#include "bootpack.h"

void HariMain() {
    extern unsigned char hankaku[4096];
	Font *font = (Font*)hankaku;
	BootInfo *bootinfo = BootInfoAddr;
	Screen *screen = &_screen;
	screen->pixels = bootinfo->pixels;
	screen->xsize = bootinfo->xsize;
	screen->ysize = bootinfo->ysize;
	init_palette();
	Window *window = init_screen(screen);
	memMan_init(memMan, MemoryStart, MemoryEnd);
	char s[40];
	sprintf(s, "screen: %d x %d", screen->xsize, screen->ysize);
	putFonts8(window, White, 8, 10, s, font);
	void *test = memory_alloc(memMan, 10<<20);
	sprintf(s, "memory: %dMB total, %dMB free", memMan->totalSize / (1<<(10*2)), memMan_rest(memMan) / (1<<(10*2)));
	putFonts8(window, White, 8, 30, s, font);
	for(;;) { io_hlt();	}
}
