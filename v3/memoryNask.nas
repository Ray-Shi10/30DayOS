[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "memory.nas"]

GLOBAL	_memTest_asm, _store_cr0, _load_cr0
[SECTION .text]

; unsigned int memTest_asm(unsigned int start, unsigned int end)
_memTest_asm:
		PUSH	EDI
		PUSH	ESI
		PUSH	EBX
		MOV		ESI,0xaa55aa55			; pat0 = 0xaa55aa55;
		MOV		EDI,0x55aa55aa			; pat1 = 0x55aa55aa;
		MOV		EAX,[ESP+12+4]			; i = start;
loop:
		MOV		EBX,EAX
		ADD		EBX,0xffc				; p = i + 0xffc;
		MOV		EDX,[EBX]				; old = *p;
		MOV		[EBX],ESI				; *p = pat0;
		XOR		DWORD [EBX],0xffffffff	; *p ^= 0xffffffff;
		CMP		EDI,[EBX]				; if (*p != pat1) goto fin;
		JNE		fin
		XOR		DWORD [EBX],0xffffffff	; *p ^= 0xffffffff;
		CMP		ESI,[EBX]				; if (*p != pat0) goto fin;
		JNE		fin
		MOV		[EBX],EDX				; *p = old;
		ADD		EAX,0x1000				; i += 0x1000;
		CMP		EAX,[ESP+12+8]			; if (i <= end) goto loop;
		JBE		loop
		POP		EBX
		POP		ESI
		POP		EDI
		RET
fin:
		MOV		[EBX],EDX				; *p = old;
		POP		EBX
		POP		ESI
		POP		EDI
		RET

; void _store_cr0(unsigned int cr0)
_store_cr0: 
		MOV		EAX,[ESP+4]
		MOV		CR0,EAX
		RET

; unsigned int load_cr0();
_load_cr0:
		MOV EAX, CR0
		RET