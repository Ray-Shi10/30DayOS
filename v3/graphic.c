#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "bootpack.h"

void init_palette() {
    static uchar table_palette[16 * 3] = {
        0x00, 0x00, 0x00,
        0xff, 0x00, 0x00,
        0x00, 0xff, 0x00,
        0xff, 0xff, 0x00,
        0x00, 0x00, 0xff,
        0xff, 0x00, 0xff,
        0x00, 0xff, 0xff,
        0xff, 0xff, 0xff,
        0xc6, 0xc6, 0xc6,
        0x84, 0x00, 0x00,
        0x00, 0x84, 0x00,
        0x84, 0x84, 0x00,
        0x00, 0x00, 0x84,
        0x84, 0x00, 0x84,
        0x00, 0x84, 0x84,
        0x84, 0x84, 0x84,
    }; set_palette(table_palette, 0, 15);
}
void set_palette(uchar *palette, uint start, uint end) {
    const uint eflags = load_eflags();
    uint i; io_cli(); io_out8(0x03c8, start);
    for (i = start; i <= end; i++, palette += 3) {
        io_out8(0x03c9, palette[0]>>2);
        io_out8(0x03c9, palette[1]>>2);
        io_out8(0x03c9, palette[2]>>2);
    } store_eflags(eflags);
}
void putFont8(Window *window, Color color, int x, int y, Font font) {
	uchar i;
	for(i = 0; i < 16; i++) {
		if(y + i >= window->ysize) { break; }
		if(y + i < 0) { continue; }
		Color *p = window->pixels + (y + i) * window->xsize + x;
		FontRow d = font[i];
		uchar j;
		for(j = 0; j < 8; j++, d <<= 1) {
			if(x + j >= window->xsize) { break; }
			if(x + j >= 0) { (d & 0x80) && (p[j]=color); }
		}
	}
}
void putFonts8(Window *window, Color color, int x, int y, char *s, Font *fonts) {
	for(; *s; s++, x += 8) {
		putFont8(window, color, x, y, fonts[*s]);
	}
}
void boxfill8(Window *window, Color color, int x0, int y0, int x1, int y1) {
	int x, y;
	for(y = y0; y <= y1; y++) {
		for(x = x0; x <= x1; x++) {
			window->pixels[y * window->xsize + x] = color;
		}
	}
	return;
}
Window *init_screen(Screen *screen) {
	Window *window = (Window*)0xfffff;
	window->xpos = 0;
	window->ypos = 0;
	window->xsize = screen->xsize;
	window->ysize = screen->ysize;
	window->pixels = screen->pixels;
	window->next = 0;

	boxfill8(window, DarkCyan,  0,         0,          window->xsize -  1, window->ysize - 29);
	boxfill8(window, LightGray,  0,         window->ysize - 28, window->xsize -  1, window->ysize - 28);
	boxfill8(window, White,  0,         window->ysize - 27, window->xsize -  1, window->ysize - 27);
	boxfill8(window, LightGray,  0,         window->ysize - 26, window->xsize -  1, window->ysize -  1);

	boxfill8(window, White,  3,         window->ysize - 24, 59,         window->ysize - 24);
	boxfill8(window, White,  2,         window->ysize - 24,  2,         window->ysize -  4);
	boxfill8(window, DarkGray,  3,         window->ysize -  4, 59,         window->ysize -  4);
	boxfill8(window, DarkGray, 59,         window->ysize - 23, 59,         window->ysize -  5);
	boxfill8(window, Black,  2,         window->ysize -  3, 59,         window->ysize -  3);
	boxfill8(window, Black, 60,         window->ysize - 24, 60,         window->ysize -  3);

	boxfill8(window, DarkGray, window->xsize - 47, window->ysize - 24, window->xsize -  4, window->ysize - 24);
	boxfill8(window, DarkGray, window->xsize - 47, window->ysize - 23, window->xsize - 47, window->ysize -  4);
	boxfill8(window, White, window->xsize - 47, window->ysize -  3, window->xsize -  4, window->ysize -  3);
	boxfill8(window, White, window->xsize -  3, window->ysize - 24, window->xsize -  3, window->ysize -  3);
	return window;
}

#endif // _GRAPHIC_H_
