#include "bootpack.h"

#define EFLAGS_AC_BIT		0x00040000 //用来检查CPU是否为486及以上
#define CR0_CACHE_BIT		0x60000000 //禁止缓存

void *memTest(void *start, void *end) {/**/
	uint eflg = load_eflags();
	store_eflags(eflg | EFLAGS_AC_BIT); // AC-bit = 1
	eflg = load_eflags();
	char is486 = eflg & EFLAGS_AC_BIT; // 386CPU的AC位会回到0
	store_eflags(eflg & ~EFLAGS_AC_BIT); // AC-bit = 0
	if(is486) {
		store_cr0(load_cr0() | CR0_CACHE_BIT);
	}
	void *ans = memTest_asm(start, end);
	if(is486) {
		store_cr0(load_cr0() & ~CR0_CACHE_BIT);
	}
	return ans;
}

void memMan_init(MemoryManager *memMan, void *start, void *_end) {
	void *end = memTest(start, _end);
	memMan->amount = 1;
	memMan->totalSize = end - start;
	memMan->lostSize = 0;
	memMan->lostAmount = 0;
	memMan->free[0].addr = start;
	memMan->free[0].size = end - start;
	return;
}
/**/
void memory_free(MemoryManager *memMan, void *addr, uint size) {
	uint i, j;
	for(i = 0; i < memMan->amount; i++) {
		if(memMan->free[i].addr > addr) {
			break;
		}
	}
	if(i > 0) {
		if(memMan->free[i-1].addr + memMan->free[i-1].size == addr) {
			memMan->free[i-1].size += size;
			if(i < memMan->amount) {
				if(addr + size == memMan->free[i].addr) {
					memMan->free[i-1].size += memMan->free[i].size;
					memMan->amount--;
					for(; i < memMan->amount; i++) {
						memMan->free[i] = memMan->free[i+1];
					}
				}
			}
			return;
		}
	}
	if(i < memMan->amount) {
		if(addr + size == memMan->free[i].addr) {
			memMan->free[i].addr = addr;
			memMan->free[i].size += size;
			return;
		}
	}
	if(memMan->amount < 1000) {
		for(j = memMan->amount; j > i; j--) {
			memMan->free[j] = memMan->free[j-1];
		}
		memMan->amount++;
		if(memMan->lostAmount > 0) {
			memMan->lostAmount--;
			memMan->lostSize -= size;
		}
		memMan->free[i].addr = addr;
		memMan->free[i].size = size;
		return;
	}
	memMan->lostAmount++;
	memMan->lostSize += size;
	return;
}/**/

void *memory_alloc(MemoryManager *memMan, uint size) {
	uint i;
	for(i = 0; i < memMan->amount; i++) {
		if(memMan->free[i].size >= size) {
			void *addr = memMan->free[i].addr;
			memMan->free[i].addr += size;
			memMan->free[i].size -= size;
			if(memMan->free[i].size == 0) {
				memMan->amount--;
				for(; i < memMan->amount; i++) {
					memMan->free[i] = memMan->free[i+1];
				}
			}
			return addr;
		}
	}
	return 0;
}

uint memMan_rest(MemoryManager *memMan) {
	uint i, t = 0;
	for(i = 0; i < memMan->amount; i++) {
		t += memMan->free[i].size;
	}
	return t;
}
