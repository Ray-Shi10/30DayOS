#ifndef _BOOT_PACK_H_
#define _BOOT_PACK_H_

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

#define nullptr ((void*)0)
#define pointer(p) ((void*)(p))

#define BootInfoAddr (void*)0x0ff0
#define MemManagerAddr (void*)0x003c0000
#define MemoryStart (void*)0x00400000
#define MemoryEnd (void*)0xbfffffff

typedef uchar Color;
typedef struct {
    char cyls, leds, vmode, reserve;
    short xsize, ysize;
    Color *pixels;
} BootInfo;

typedef uchar FontRow;
typedef FontRow Font[16];
Font *getDefaultFont();

void io_hlt();
void io_cli();
void io_sti();
uint load_eflags();
void store_eflags(uint eflags);
void io_out8(uint port, uint data);

typedef struct {
    int xpos, ypos;
    int xsize, ysize;
    Color *pixels;
    struct Window *next;
} Window;
typedef struct {
    int xsize, ysize;
    Color *pixels;
    Window windows[1];
} Screen;
Screen _screen;

enum COLORS {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Purple,
    Cyan,
    White,
    LightGray,
    DarkRed,
    DarkGreen,
    DarkYellow,
    DarkBlue,
    DarkPurple,
    DarkCyan,
    DarkGray,
    _amount,
};
void init_palette();
void set_palette(uchar *rgb, uint start, uint end);
void putFont8(Window *window, Color color, int x, int y, Font font);
void putFonts8(Window *window, Color color, int x, int y, char *s, Font *fonts);
void boxfill8(Window *window, Color color, int x0, int y0, int x1, int y1);
Window *init_screen(Screen *screen);

typedef struct {
    void *addr;
    uint size;
    //struct MemFreeInfo *next;
} MemFreeInfo;
typedef struct {
    uint amount, totalSize, lostSize, lostAmount;
    MemFreeInfo free[1000];
} MemoryManager;
MemoryManager *memMan = MemManagerAddr;
void *memTest(void *start, void *end);
void store_cr0(uint cr0);
uint load_cr0();
uint memTest_asm(void *start, void *end);
void memMan_init(MemoryManager *memMan, void *start, void *end);
uint memMan_rest(MemoryManager *memMan);
void memory_free(MemoryManager *memMan, void *addr, uint size);
void *memory_alloc(MemoryManager *memMan, uint size);

#endif // _BOOT_PACK_H_
