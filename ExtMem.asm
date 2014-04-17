VERSION			EQU		1			;VERSION
SUBVERSION		EQU		0			;SUBVERSION

_AOFF			EQU		6			;FAR STACK OFFSET

;THIS MACRO FETCHES THE DATA SEGMENT
LDATASEG		MACRO
				PUSH	AX
				MOV		AX,_DATA
				MOV		DS,AX
				POP		AX
				ENDM

ENABLE_A20		MACRO
				PUSH	AX
				PUSH	BX
				MOV		AH,5
				CALL	CS:[CONTROL]
				POP		BX
				POP		AX
				ENDM

DISABLE_A20		MACRO
				PUSH	AX
				PUSH	BX
				MOV		AH,4
				CALL	CS:[CONTROL]
				POP		BX
				POP		AX
				ENDM

EXTMEM_TEXT		SEGMENT BYTE PUBLIC 'CODE'
				ASSUME	CS:EXTMEM_TEXT,DS:_DATA

;THIS FUNCTION	INITIALIZES THE DRIVER
;				CALLED AS
;				get_extmem();
;
;				Returns the version number or -1 if no driver exists
;
				PUBLIC _get_extmem
_get_extmem		PROC FAR
				PUSH BP
				MOV BP,SP
				PUSH DS
				PUSH ES
				MOV AX,4300H
				INT 2FH
				CMP AL,80H
				JE GETX1

				MOV AX,0FFFFH
				JMP GETX2

GETX1:			MOV AX,4310H

				INT 2FH
				MOV WORD PTR CS:[_CONTOFF],BX
				MOV WORD PTR CS:[_CONTSEG],ES

				MOV AX,0000H
				CALL CS:[CONTROL]

GETX2:			POP ES
				POP DS
				POP BP
				RET
_get_extmem		ENDP

;THIS FUNCTION MOVES EXTENDED MEMORY
;				CALLED AS
;				move_extmem(p);
;				p=pointer to move structure
;				/* returns true if successful */
;
;
				PUBLIC _dealloc_extmem
_dealloc_extmem	PROC	FAR
				PUSH	BP
				MOV		BP,SP
				PUSH	DS
				PUSH	ES

				ENABLE_A20

				MOV		DX,[BP + _AOFF + 0]
				MOV		AH,10
				CALL	CS:[CONTROL]

				DISABLE_A20

				POP		ES
				POP		DS
				POP		BP
				RET
_dealloc_extmem	ENDP

;THIS FUNCTION ALLOCATES EXTENDED MEMORY
;				CALLED AS
;				alloc_extmem(n);
;				int n; /*number of kilobytes to allocate*/
;						/* returns handle or -1 if there is a fucking error =P */
;
				PUBLIC _alloc_extmem
_alloc_extmem	PROC	FAR
				PUSH	BP
				MOV		BP,SP
				PUSH	DS
				PUSH	ES

				ENABLE_A20

				MOV		AH,8
				CALL	CS:[CONTROL]
				CMP		AX,[BP + _AOFF + 0]
				JL		ALLOC1

				MOV		DX,[BP + _AOFF + 0]			;ALLOCATES MEMORY
				MOV		AH,9
				CALL	CS:[CONTROL]

				OR		AX,AX
				JZ		ALLOC1

				MOV		AX,DX
				JMP		ALLOC2

ALLOC1:			MOV		AX,0FFFFH

				DISABLE_A20

ALLOC2:			POP		ES
				POP		DS
				POP		BP
				RET
_alloc_extmem	ENDP

;THIS FUNCTION RETURNS THE AMOUNT OF FREE XMS MEMORY
;				CALLED AS
;				coreleft_extmem();
;
				PUBLIC	_coreleft_extmem
_coreleft_extmem	PROC	FAR
					PUSH	BP
					MOV		BP,SP
					PUSH	DS
					PUSH	ES

					ENABLE_A20

					MOV		AH,8
					CALL	CS:[CONTROL]
					DISABLE_A20

					POP		ES
					POP		DS
					POP		BP
					RET
_coreleft_extmem	ENDP

;THIS FUNCTION CONVERTS A POINTER TO AN INTEL LONG
;				CALLED AS
;				long ptr2long(p);
;				char *p;
;
				PUBLIC _ptr2long
_ptr2long		PROC	FAR
				PUSH	BP
				MOV		BP,SP

				MOV		AX,[BP + _AOFF + 0]			;OFFSET OF POINTER
				MOV		DX,[BP + _AOFF + 2]			;SEGMENT OF POINTER

				POP		BP
				RET
_ptr2long		ENDP

;THIS FUNCTION IS A DUMMY RETURN FOR UN-SET PROC
_DUMMY			PROC	FAR
				MOV		AX,0FFFFH
				RET
_DUMMY			ENDP

CONTROL			LABEL	DWORD
_CONTOFF		DW		_DUMMY
_CONTSEG		DW		EXTMEM_TEXT

EXTMEM_TEXT		ENDS
DGROUP			GROUP	_DATA,_BSS
_DATA			SEGMENT WORD PUBLIC 'DATA'

_DATA			ENDS

_BSS			SEGMENT WORD PUBLIC 'BSS'
_BSS			ENDS
				END
