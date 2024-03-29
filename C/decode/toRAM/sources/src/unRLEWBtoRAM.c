/* =============================================================================                                                                            
 unRLEWBtoRAM v1.1  (26 jun 2014)                                                                     
 Function : Decompress RLEWB data to RAM

 Historic of versions:
 - v1.1  (26 jun 2014) 
  
 CD = Control Digit = $80

 CD + $0         --> for one $80 value
 CD + $FF        --> end of data block
 CD + nn + dd    --> repeat nn ($1-$FE)+1 dd value
 dd (!= CD)      --> raw data                              
============================================================================= */ 
 

#define RLEWB_CD   0x80
#define RLEWB_END  0xFF
  

void unRLEWBtoRAM (unsigned int sourceADDR, unsigned int targetADDR) __naked
{
sourceADDR;
targetADDR;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
      
  ld   L,4(ix) ; source RAM RLE data address
  ld   H,5(ix)
    
  ld   E,6(ix) ; target RAM address
  ld   D,7(ix)
   
  call unRLEWBRAM
    
  pop  IX
  ret


;===========================================================================
; unRLEWBRAM
; Function : RLE WB to RAM unpacker
;     
; Input    : HL - source RAM RLE data address
;            DE - target RAM address            
;=========================================================================== 
unRLEWBRAM::
  
  ld   A,(HL)         ;get byte
  cp   #RLEWB_CD      ;if DC?                
  jr   NZ,write_Byte2RAM   ;if raw?
  
  inc  HL             ; get next byte 
  ld   A,(HL)
  or   A
  jr   Z,write_DC2RAM ;(DC $00) if A=0 then write DC value
  cp   #RLEWB_END     ;if A=$FF 
  ret  Z              ;(DC $FF) then exit
  
  ;(DC nn dd)
  inc  A              ;2 to 255 (1? what a waste! ;D ) 
  ld   B,A            ;(nn) loop length
  inc  HL
  ld   A,(HL)         ;(dd) get value
  
RLEWBram_loop:
  ld   (DE),A     ;write value
  inc  DE
  djnz RLEWBram_loop
  
  inc  HL
  jr   unRLEWBRAM

write_DC2RAM:
  ;output value equal to DC
  ld   A,#RLEWB_CD  ;write DC value
  
write_Byte2RAM:
  ld   (DE),A     ;write value
  inc  DE
  inc  HL
  jr   unRLEWBRAM

__endasm;
}
