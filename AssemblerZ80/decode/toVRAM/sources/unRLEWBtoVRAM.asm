;===============================================================================
; unRLEWBtoVRAM v1.4 (24 January 2022)
;
; Function : Decompress RLEWB data to VRAM
;
; Historic of versions:
; - v1.4 (24 January 2022) Sjasm and tniASM support. MSX-DOS support.
; - v1.3 (24 March   2020) Enhanced to pass the Acid2Test (by SDsnatcher)
; - v1.2 (20 April 2019)
; - v1.1 (26 June 2014)
;
;-------------------------------------------------------------------------------
; LABELS:
; You can add the following Labels to your main code, to configure the 
; decompressor for different environments:
; For Sjasm assembler, use DEFINE + Label
;
; Identify the assembler (ASMSX , SJASM or TNIASM) Default: asMSX/Sjasm
;  
; Runtime (MSXROM , MSXDOS or MSXBASIC) Default: MSXROM/MSXBASIC
;
; DIRECTRANGE if you have a compressor that generated a range from 3 to 254 reps
;
;-------------------------------------------------------------------------------	 
; Input	: HL - source RLEWB data memory address
;         DE - target VRAM address
;-------------------------------------------------------------------------------
; CD = Control Digit = $80
;
; CD + $0         --> for one $80 value
; CD + $FF        --> end of data block
; CD + nn + dd    --> repeat nn ($1-$FE)+1 dd value
; dd (!= CD)      --> raw data		    
;=============================================================================== 



;--------------------------------------------------------------------- constants
IFDEF TNIASM
;tniASM	
    RLEWB_CD:    EQU $80  ;Control Digit
    RLEWB_END:   EQU $FF  ;End

ELSE
;asMSX and Sjasm
RLEWB_CD    EQU $80  ;Control Digit
RLEWB_END   EQU $FF  ;End

ENDIF
;-------------------------------------------------------------------------------



unRLEWBtoVRAM:

IFDEF MSXDOS
    push  HL         ;when using RDSLT the values of AF, BC, DE and HL are lost 
    push  DE                    
    LD    HL,$0007   ;VDP_DW=BIOS System constant (Base port address for VDP data write)
	LD    A,[$FCC1]  ;EXPTBL=main BIOS-ROM slot address
	CALL  $000C      ;RDSLT=Reads the value of an address in another slot
	EI               ;RDSLT leaves interrupts disabled.
ELSE
	ld    A,[$0007]  ;VDP_DW=BIOS System constant (Base port address for VDP data write)
ENDIF
	inc   A
	ld    C,A

IFDEF MSXDOS
    push  BC
    LD    HL,$002D   ;MSXID3=BIOS System constant (MSX version number)
	LD    A,[$FCC1]  ;EXPTBL=main BIOS-ROM slot address
	CALL  $000C      ;RDSLT=Reads the value of an address in another slot
	EI               ;RDSLT leaves interrupts disabled.
    pop   BC                          
ELSE
	ld    A,[$002D]  ;MSXID3=BIOS System constant (MSX version number)
ENDIF
    di
	or    A	
	jr    Z,RLEWB_setVADDR	;if MSX1(TMS9918) goto RLEWB_setVADDR
	;V9938 or higher
	xor   A
	out   [C],A		 ;clear three upper bits for 16bit VRAM ADDR (128K)
	ld    A,14+128	 ;V9938 reg 14 - Control Register
	out   [C],A

RLEWB_setVADDR:
;set VRAM address
IFDEF MSXDOS
    pop   DE         ;restore input parameters
    pop   HL
ENDIF	
	out   [C],E		 ;first 8bits from VRAM ADDR
	ld    A,D		 ;6 bits from VRAM ADDR 
	and   $3F
	or    $40		 ;active write access
	out   [C],A
	dec   C
	ei

        
RLEWB_ANALYZE:
	ld    A,[HL]          ;get byte
	cp    RLEWB_CD        ;if CD?		         
	jr    Z,RLEWB_unpack  ;Yes, unpack
	outi                  ;send value to VDP
	jr    RLEWB_ANALYZE

RLEWB_unpack:
	inc   HL              ;get next byte 
	ld    A,[HL]
	or    A		        
	jr    Z,RLEWB_writeCONTROL	;if A=0 then write Control Digit value
	cp    RLEWB_END       ;if A=$FF (DC $FF)
	ret   Z               ;THEN exit
	
	;CD nn dd
IFDEF DIRECTRANGE
    ;you can modify the code of the compressor so that it counts only ranges from 3 to 254 repetitions
    ;allows to suppress the increment statement and optimize the decompression
ELSE    
	inc   A               ;A=number of repetitions (3 to 255)
ENDIF
	inc   HL
	ld    B,[HL]          ;(dd) get value
	
RLEWB_loop:
	out   [C],B           ;send value to VDP (13ts)
	dec   A               ;(5ts)
	jr    NZ,RLEWB_loop   ;(13ts) 13+5+13=31ts	(time for write 29 T-states)
	inc   HL
	jr    RLEWB_ANALYZE

RLEWB_writeCONTROL:
	;output value equal to Control Digit
	ld    A,RLEWB_CD
	out   [C],A
	inc   HL
	jr    RLEWB_ANALYZE