;===============================================================================
; unRLEWBtoVRAM v1.2.1 asMSX for MSX-DOS runtime (24 January 2022)
;
; Function : Decompress RLEWB data to VRAM
;
; Historic of versions:
; - v1.4 (24 January 2022) Sjasm and tniASM support. MSX-DOS support.
; - v1.3 (24 March   2020) SDsnatcher Issue #1 - Enhanced to pass the Acid2Test
; - v1.2.1 asMSX for MSX ROM runtime (24 January 2022)
; - v1.2.1 asMSX for MSX-DOS runtime (24 January 2022)
; - v1.2 (20 April 2019)
; - v1.1 (26 jun 2014)
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
;=============================================================================== 



;--------------------------------------------------------------------- constants
;asMSX
RLEWB_CD    EQU $80  ;Control Digit
RLEWB_END   EQU $FF  ;End

;VDPVRAM     EQU $98 ;VRAM Data (Read/Write)
;VDPSTATUS   EQU $99 ;VDP Status Registers
;-------------------------------------------------------------------------------


;
unRLEWBtoVRAM:
    push  HL
    push  DE
    ;di
    LD    HL,$002D          ;MSXID3=BIOS System constant (MSX version number)
    LD    A,[$FCC1]         ;EXPTBL=main BIOS-ROM slot address
    CALL  $000C             ;RDSLT=Reads the value of an address in another slot
    ;ei                     ;RDSLT leaves interrupts disabled. We leave it like this
    or   A
    jr   Z,RLEWB_setVADDR	  ;if MSX1(TMS9918) goto RLEWB_setVADDR
    ;V9938 or higher
    xor  A
    out  [$99],A            ;clear three upper bits for 16bit VRAM ADDR (128K)
    ld   A,14+128	          ;V9938 reg 14 - Control Register
    out  [$99],A

RLEWB_setVADDR:
    pop   DE
      
    ld    A,E    
    out   [$99],A           ;first 8bits from VRAM ADDR
    ld    A,D               ;6 bits from VRAM ADDR
    and   $3F
    or    $40               ;active write access
    out   [$99],A
    ei

    pop   HL
RLEWB_ANALYZE:
	ld	A,[HL]			  ;get byte
	cp	RLEWB_CD		  ;if CD?		         
	jr	Z,RLEWB_unpack    ;Yes, unpack
RLEWB_writeByte:
	out [$98],A           ;write in VRAM 
	inc	HL				
	jr	RLEWB_ANALYZE

RLEWB_unpack:
	inc  HL               ;get next byte 
	ld   A,[HL]
	or   A		        
	jr   Z,RLEWB_writeCONTROL	;if A=0 then write DC value (DC $0)
	cp   RLEWB_END			    ;if A=$FF (DC $FF)
	ret  Z			            ;THEN exit
	
;CD nn dd
	inc  A			      ;2 to 255 (1? what a waste! ;D )
    ld   B,A              ;(nn) loop length 
	inc  HL
	ld   A,[HL]			  ;(dd) get value
	
RLEWB_loop:
	out  [$98],A          ;(12ts) write in VRAM
	dec  B			      ;(5ts)
	jr   NZ,RLEWB_loop	  ;(13ts) 12+5+13=30ts	(time for write 29 T-states)

	inc  HL
	jr   RLEWB_ANALYZE

RLEWB_writeCONTROL:
	;output value equal to DC
	ld	A,RLEWB_CD
	jr	RLEWB_writeByte
