 
;constants
RLEWB_DC   EQU   $80

;===========================================================================
; unRLEWBtoRAM v1.1 (26 jun 2014)
; Function : Decompress RLEWB data to RAM
;     
; Input    : HL - source RAM RLEWB data address
;            DE - target RAM address
;
; DC nn dd            ; repeat nn ($1-$FE)+1 dd value
; DC $0               ; for one $80 value
; DC $FF              ; end of data block
; dd (!=DC)           ; raw data             
;=========================================================================== 
unRLEWBtoRAM:
  
  ld    A,[HL]          ;get byte
  cp    RLEWB_DC        ;if DC?               
  jr    NZ,write_Byte2RAM ;if raw?
  
  inc   HL              ;get next byte 
  ld    A,[HL]
  or    A              
  jr    Z,write_DC2RAM  ;if A=0 then write DC value (DC $0)
  cp    $FF             ;if A=$FF (DC $FF)
  ret   Z               ;then exit
  
  ;$80 nn dd
  inc   A               ;2 to 255 (1? what a waste! ;D ) 
  ld    B,A             ;(nn) loop length 
  inc   HL
  ld    A,[HL]          ;(dd) get value
  
RLEWBram_loop:
  ld    [DE],A
  inc   DE
  djnz  RLEWBram_loop
  
  inc   HL
  jr    unRLEWBtoRAM

write_DC2RAM:
  ;output value equal to DC
  ld    A,RLEWB_DC   ;write CONTROL value
  
write_Byte2RAM:
  ld    [DE],A
  inc   DE
  inc   HL
  jr    unRLEWBtoRAM