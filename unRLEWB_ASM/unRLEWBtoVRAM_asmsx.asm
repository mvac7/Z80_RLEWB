;system vars
MSXVER		EQU	$002D
VDP_DW		EQU	$0007

;constants	
RLEWB_CD	EQU	 $80  ; Control Digit

;DEFINE EXTRLERANGE	; Use a modified repeat range from 2 to 255 instead (sjasm)
			        ; of the standard 1 to 254

;===========================================================================
; unRLEWBtoVRAM v1.3 (24 March 2020)
;
; Function : Decompress RLEWB data to VRAM
;
; Historic of versions:
;	v1.1 (26 jun 2014)
;	v1.2 (20 April 2019)
;   v1.3 (24 March 2020) SDsnatcher Issue #1 - Enhanced to pass the Acid2Test
;	 
; Input	: HL - source RAM RLEWB data address
;         DE - target VRAM address
;
; CD = Control Digit = $80
;
; CD + $0         --> for one $80 value
; CD + $FF        --> end of data block
; CD + nn + dd    --> repeat nn ($1-$FE)+1 dd value
; dd (!= CD)      --> raw data		    
;=========================================================================== 
unRLEWBtoVRAM:
	ld	A,[VDP_DW]
	inc	A
	ld	C,A
	ld	A,[MSXVER]
	or	A
	di
	jr	Z,RLEWB_setVADDR	;if MSX1(TMS9918) goto RLEWB_setVADDR
	;V9938 or higher
	xor	A
	out	[C],A		;clear three upper bits for 16bit VRAM ADDR (128K)
	ld	A,14+128	;V9938 reg 14 - Control Register
	out	[C],A

RLEWB_setVADDR:
;set address counter	
	out	[C],E		;first 8bits from VRAM ADDR
	ld	A,D		;6 bits from VRAM ADDR 
	and	$3F
	or	$40		;active write access
	ei
	out	[C],A
	dec	C
RLEWB_ANALYZE:
	ld	A,[HL]			;get byte
	cp	RLEWB_CD		;if DC?		         
	jr	Z,RLEWB_unpack		;Yes, unpack
	outi				;send value to VDP
	jp	RLEWB_ANALYZE

RLEWB_unpack:
	inc	HL			;get next byte 
	ld	A,[HL]
	or	A		        
	jr	Z,RLEWB_writeCONTROL	;if A=0 then write DC value (DC $0)
	cp	$FF			;if A=$FF (DC $FF)
	ret	Z			;THEN exit
	
	;DC nn dd
; IFDEF EXTRLERANGE (sjasm)
	inc	A			;2 to 255 (1? what a waste! ;D )
; ENDIF
	inc	HL
	ld	B,[HL]			;(dd) get value
	
RLEWB_loop:
	out	[C],B			;send value to VDP (13ts)
	dec	A			;(5ts)
	jr	NZ,RLEWB_loop		;(13ts) 13+5+13=31ts	(time for write 29 T-states)
	inc	HL
	jr	RLEWB_ANALYZE

RLEWB_writeCONTROL:
	;output value equal to DC
	ld	A,RLEWB_CD
	out	[C],A
	inc	HL
	jr	RLEWB_ANALYZE
