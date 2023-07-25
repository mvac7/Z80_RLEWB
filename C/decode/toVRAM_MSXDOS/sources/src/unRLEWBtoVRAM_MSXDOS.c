/*============================================================================== 
 unRLEWBtoVRAM MSX-DOS decoder v1.0  (7 february 2022)

 Function: 
   Decompress WBRLE data to VRAM for MSX-DOS runtime environment

 Historic of versions:
 - v1.0 (7 february 2022)  
                                                              
 CD = Control Digit = $80

 CD + $0         --> for one $80 value
 CD + $FF        --> end of data block
 CD + nn + dd    --> repeat nn ($1-$FE)+1 dd value
 dd (!= CD)      --> raw data                            
==============================================================================*/ 


  
#define RLEWB_CD   0x80
#define RLEWB_END  0xFF 


/* =============================================================================
 unRLEWBtoVRAM
 Function : unpack RLE-WB to VRAM
  
 Input    : [unsigned int] dataAddr <- source RLEWB data address
            [unsigned int] VRAMaddr <- target VRAM address       
 Output   : -                  
============================================================================= */
void unRLEWBtoVRAM (unsigned int dataAddr, unsigned int VRAMaddr) __naked
{
dataAddr;
VRAMaddr;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
      
  ld   L,4(ix) ; Data RAM address
  ld   H,5(ix)
    
  ld   E,6(ix) ; VRAM address
  ld   D,7(ix)
   
  call unRLEWBVRAM
    
  pop  IX
  ret

;===========================================================================
; unRLEWBVRAM
; Function : RLE WB to VRAM unpacker
;     
; Input    : HL - source RLEWB data address
;            DE - target VRAM address            
;=========================================================================== 
unRLEWBVRAM::

    push  HL           ;when using RDSLT the values of AF, BC, DE and HL are lost 
    push  DE                    
    ld    HL,#0x0007   ;VDP_DW=BIOS System constant (Base port address for VDP data write)
	ld    A,(#0xFCC1)  ;EXPTBL=main BIOS-ROM slot address
	call  0x000C       ;RDSLT=Reads the value of an address in another slot
	ei                 ;RDSLT leaves interrupts disabled.
	inc   A
	ld    C,A


    push  BC
    ld    HL,#0x002D   ;MSXID3=BIOS System constant (MSX version number)
	ld    A,(#0xFCC1)  ;EXPTBL=main BIOS-ROM slot address
	call  0x000C       ;RDSLT=Reads the value of an address in another slot
	;EI                 ;RDSLT leaves interrupts disabled.
    pop   BC                          
    ;di
	or    A	
	jr    Z,RLEWB_setVADDR	;if MSX1(TMS9918) goto RLEWB_setVADDR
	;V9938 or higher
	xor   A
	out   (C),A		 ;clear three upper bits for 16bit VRAM ADDR (128K)
	ld    A,#14+128	 ;V9938 reg 14 - Control Register
	out   (C),A


RLEWB_setVADDR:      ;set VRAM address
    pop   DE         ;restore input parameters
    pop   HL
	
	out   (C),E		 ;first 8bits from VRAM ADDR
	ld    A,D		 ;6 bits from VRAM ADDR 
	and   #0x3F
	or    #0x40		 ;active write access
	out   (C),A
	dec   C
	ei

        
RLEWB_ANALYZE:
	ld    A,(HL)          ;get byte
	cp    #RLEWB_CD       ;if CD?		         
	jr    Z,RLEWB_unpack  ;Yes, unpack
	outi                  ;send value to VDP
	jr    RLEWB_ANALYZE

RLEWB_unpack:
	inc   HL              ;get next byte 
	ld    A,(HL)
	or    A		        
	jr    Z,RLEWB_writeCONTROL	;if A=0 then write Control Digit value
	cp    #RLEWB_END      ;if A=$FF (DC $FF)
	ret   Z               ;THEN exit
	
	;DC nn dd     nn = A = 1 to 254
	inc   A               ;2 to 255 (1? what a waste! ;D )
	inc   HL
	ld    B,(HL)          ;(dd) get value
	
RLEWB_loop:
	out   (C),B           ;send value to VDP (13ts)
	dec   A               ;(5ts)
	jr    NZ,RLEWB_loop   ;(13ts) 13+5+13=31ts	(time for write 29 T-states)
	inc   HL
	jr    RLEWB_ANALYZE

RLEWB_writeCONTROL:
	;output value equal to Control Digit
	ld    A,#RLEWB_CD
	out   (C),A
	inc   HL
	jr    RLEWB_ANALYZE

__endasm;
}

