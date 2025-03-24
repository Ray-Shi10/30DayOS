[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "eflagsNask.nas"]

GLOBAL  _io_cli, _io_sti
GLOBAL	_load_eflags, _store_eflags

[SECTION .text]

_io_cli:	; void io_cli();
		CLI
		RET

_io_sti:	; void io_sti();
		STI
		RET

_load_eflags:	; int io_load_eflags();
		PUSHFD		; PUSH EFLAGS
		POP		EAX
		RET

_store_eflags:	; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; POP  EFLAGS
		RET