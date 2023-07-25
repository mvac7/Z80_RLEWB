;===============================================================================
; unRLEWBtoRAM v1.2 (6 February 2022)
;
; Function : Decompress RLEWB data to RAM
;
; Historic of versions:
; - v1.2 (6 February 2022)
; - v1.1 (26 jun 2014)
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
;         DE - target RAM address
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
  
  ;(CD nn dd)
IFDEF DIRECTRANGE
    ;you can modify the code of the compressor so that it counts only ranges from 3 to 254 repetitions
    ;allows to suppress the increment statement and optimize the decompression
ELSE    
	inc   A               ;A=number of repetitions (3 to 255)
ENDIF
  ld    B,A             ;(nn) loop length 
  inc   HL
  ld    A,[HL]          ;(dd) get value
  
RLEWBram_loop:
  ld    [DE],A          ;write value
  inc   DE
  djnz  RLEWBram_loop
  
  inc   HL
  jr    unRLEWBtoRAM

  
