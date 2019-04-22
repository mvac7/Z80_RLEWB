; ========================================================================== 
; unRLEWBtoVRAM v1.2 (20 April 2019)
;
; Function : Unpack WB RLE to VRAM
;
; Historic of versions:
;   v1.1  (26 jun 2014)
;                                                              
; DC nn dd            ; repeat nn ($1-$FE)+1 dd value
; DC $0               ; for one $80 value
; DC $FF              ; end of data block
; dd (!= DC )         ; raw data                             
; ========================================================================== 
	.area _DATA


  .area _CODE

;system vars
MSXVER     = 0x002D  
  
VDPVRAM    = 0x98 ;VRAM Data (Read/Write)
VDPSTATUS  = 0x99 ;VDP Status Registers

RLEWB_DC   = 0x80


;void unRLEWBtoVRAM (uint RAM, uint VRAM)
_unRLEWBtoVRAM::
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
; unRLEWBtoVR
; Function : RLE WB to VRAM unpacker
;     
; Input    : HL - source RAM RLE data address
;            DE - target VRAM address            
;=========================================================================== 
unRLEWBVRAM::

  in   A,(VDPSTATUS)
  
  di
  ld   A,(#MSXVER)
  or   A
  jr   Z,RLEWB_setVADDR  ;if MSX1(TMS9918) goto RLEWB_setVADDR
  ;V9938 or higher
  xor  A
  out  (VDPSTATUS),A   ;clear three upper bits for 16bit VRAM ADDR (128K)
  ld   A,#14+128       ;V9938 reg 14 - Control Register
  out  (VDPSTATUS),A

RLEWB_setVADDR:
;set address counter
  ld   A,E             ;first 8bits from VRAM ADDR
  out  (VDPSTATUS),A
  ld   A,D             ;6 bits from VRAM ADDR 
  and  #0x3F
  or   #0x40           ;active write access
  out  (VDPSTATUS),A
  
RLEWB_ANALYZE:
  ld   A,(HL)          ;get byte
  cp   #RLEWB_DC       ;if DC?                 
  jr   NZ,RLEWB_writeByte   ;if raw?
  
  inc  HL              ;get next byte 
  ld   A,(HL)
  or   A
  jr   Z,RLEWB_writeCONTROL ;if A=0 then write DC value (DC $0)
  cp   #0xFF           ;if A=$FF (DC $FF) 
  jr   Z,RLEWB_exit    ;THEN exit
  
  ;DC nn dd
  inc  A               ;2 to 255 (1? what a waste! ;D )
  ld   B,A             ;(nn) loop length 
  inc  HL
  ld   A,(HL)          ;(dd) get value
  
RLEWB_loop:
  out  (VDPVRAM),A     ;send value to VDP (12ts)
  dec  B               ;(5ts)
  jr   NZ,RLEWB_loop   ;(13ts) 12+5+13=30ts  (time for write 29 T-states)
  
  inc  HL
  jr   RLEWB_ANALYZE

RLEWB_writeCONTROL:
  ;output value equal to DC
  ld   A,#RLEWB_DC
  
RLEWB_writeByte:
  out  (VDPVRAM),A    ;send value to VDP
  inc  HL
  jr   RLEWB_ANALYZE


RLEWB_exit:
  ei
  ret

