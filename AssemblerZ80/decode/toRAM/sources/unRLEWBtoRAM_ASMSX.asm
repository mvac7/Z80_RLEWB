;===============================================================================
; unRLEWBtoRAM v1.2 (6 February 2022) for asMSX cross-assembler
;
; Function : Decompress RLEWB data to RAM
;
; Historic of versions:
; - v1.2 (6 February 2022)
; - v1.1 (26 jun 2014)
;	 
; Input	: HL - source RLEWB data memory address
;         DE - target RAM address
;
; CD = Control Digit = $80
;
; CD + $0         --> for one $80 value
; CD + $FF        --> end of data block
; CD + nn + dd    --> repeat nn ($1-$FE)+1 dd value
; dd (!= CD)      --> raw data		    
;=============================================================================== 



;--------------------------------------------------------------------- constants
RLEWB_CD    EQU $80  ;Control Digit
RLEWB_END   EQU $FF  ;End
;-------------------------------------------------------------------------------


unRLEWBtoRAM:
  
  ld    A,[HL]          ;get byte
  cp    RLEWB_CD        ;if DC?               
  jr    Z,read_COMVAL
  ldi                   ;write raw value
  jr    unRLEWBtoRAM  

write_CD2RAM:
  ;output value equal to CD
  ld    A,RLEWB_CD     
  ld    [DE],A         ;write CD value
  inc   DE           
  inc   HL           
  jr    unRLEWBtoRAM 

read_COMVAL:  
  inc   HL              ;get next byte 
  ld    A,[HL]
  or    A              
  jr    Z,write_CD2RAM  ;(CD $00) if A=0 then write DC value
  cp    RLEWB_END       ;if A=$FF
  ret   Z               ;(CD $FF) then exit
  
  ;(DC nn dd)
  inc   A               ;2 to 255 (1? what a waste! ;D ) 
  ld    B,A             ;(nn) loop length 
  inc   HL
  ld    A,[HL]          ;(dd) get value
  
RLEWBram_loop:
  ld    [DE],A          ;write value
  inc   DE
  djnz  RLEWBram_loop
  
  inc   HL
  jr    unRLEWBtoRAM

  
