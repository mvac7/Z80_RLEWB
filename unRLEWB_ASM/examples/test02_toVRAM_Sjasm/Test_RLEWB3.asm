; test unWBRLE2VRAM - by mvac7  (27/06/2014)
; Assemble with Sjasm (by XL2S Entertainment) http://www.xl2s.tk/

  DEFPAGE 1,4000h,2000h		

VDPVRAM   EQU   $98 ;VRAM Data (Read/Write)
VDPSTATUS EQU   $99 ;VDP Status Registers

;system vars
MSXVER EQU $002D
FORCLR EQU $F3E9 ;Foreground colour
BAKCLR EQU $F3EA ;Background colour
BDRCLR EQU $F3EB ;Border colour
CLIKSW EQU $F3DB ;0 disabled / 1 enabled
RG1SAV EQU $F3E0 ;VDP REG 1
EXTVDP EQU $FFE7 ;

;BIOS (info by MSX Assembly Page)
;http://map.grauw.nl/resources/msxbios.php
DISSCR EQU $0041 ;inhibits the screen display
ENASCR EQU $0044 ;displays the screen
WRTVDP EQU $0047 ;write data in the VDP-register
RDVRM  EQU $004A ;Reads the content of VRAM
WRTVRM EQU $004D ;Writes data in VRAM
SETRD  EQU $0050 ;Enable VDP to read
SETWRT EQU $0053 ;Enable VDP to write
FILVRM EQU $0056 ;fill VRAM with value
LDIRMV EQU $0059 ;Block transfer to memory from VRAM
LDIRVM EQU $005C ;Block transfer to VRAM from memory
CHGMOD EQU $005F ;Switches to given screenmode
CHGCLR EQU $0062 ;Changes the screencolors
CLRSPR EQU $0069 ;Initialises all sprites
INITXT EQU $006C ;Switches to SCREEN 0 (text screen with 40 * 24 characters)
INIT32 EQU $006F ;Switches to SCREEN 1 (text screen with 32*24 characters)
INIGRP EQU $0072 ;Switches to SCREEN 2 (high resolution screen with 256*192 pixels)
INIMLT EQU $0075 ;Switches to SCREEN 3 (multi-color screen 64*48 pixels)
SETTXT EQU $0078 ;Switches to VDP in SCREEN 0 mode
SETT32 EQU $007B ;Switches VDP in SCREEN mode 1
SETGRP EQU $007E ;Switches VDP to SCREEN 2 mode
SETMLT EQU $0081 ;Switches VDP to SCREEN 3 mode
CALPAT EQU $0084 ;Returns the address of the sprite pattern table
CALATR EQU $0087 ;Returns the address of the sprite attribute table
GSPSIZ EQU $008A ;Returns current sprite size
GRPPRT EQU $008D ;Displays a character on the graphic screen

GICINI EQU $0090 ;Initialises PSG and sets initial value for the PLAY statement
WRTPSG EQU $0093 ;Writes data to PSG-register
RDPSG  EQU $0096 ;Reads value from PSG-register
;

; more BIOS functions
SNSMAT EQU $0141 ;
GTSTCK EQU $00D5
GTTRIG EQU $00D8
ENASLT EQU $0024
KILBUF EQU $0156


BASE10 EQU $1800 ;base 10 name table
BASE11 EQU $2000 ;base 11 color table
BASE12 EQU $0000 ;base 12 character pattern table
BASE13 EQU $1B00 ;base 13 sprite attribute table
BASE14 EQU $3800 ;base 14 sprite pattern table





;############################################################################### ROM HEADER

  page 1

  code                        

  db  "AB"
  dw  INIT
  dw  $0000
  dw  $0000
  dw  $0000
  dw  $0000
  dw  $0000
  dw  $0000
; END ROM HEADER ###############################################################  








INIT:
  di
	ld   SP,[$fc4a] ; Stack at the top of memory.
	ei

  

  
  
  		


	;color ,5,5  
  ld   A,1
  ld   [BAKCLR],A
  ld   A,1
  ld   [BDRCLR],A  
  call CHGCLR
	
	;screen 2
	ld   A,2
	call CHGMOD
	
	
	

;  ld   HL,BASE12
;  ld   BC,$1800
;  ld   A,$F0
;  call FILVRM

  
  ld   HL,DATA_COL
  ld   DE,BASE11
  call unRLEWBtoVRAM
 


LOOP:		
  halt
  
  
  ld   A,7         
	call SNSMAT
	sub  A,11111011B    ;ESC
	JR   Z,ENDTEST
	
  jp   LOOP

ENDTEST:  
  
  call 0



;include	"unRLEWBtoRAM.asm"
  include "unRLEWBtoVRAM.asm"  






; tileset color - type:G2
; Tiles range: 0 to 255
; RLE WB compressed - Original size= 2048 - Final size= 105
DATA_COL:
  db $80,$FE,$FC,$80,$FE,$FC,$FC,$FC,$80,$3F,$F2,$80,$0F,$51,$80,$0F
  db $F2,$80,$0F,$51,$80,$0F,$F2,$80,$07,$51,$80,$AF,$F2,$80,$07,$51
  db $80,$17,$F2,$80,$07,$51,$80,$0F,$F2,$80,$07,$51,$80,$07,$F2,$80
  db $07,$51,$80,$AF,$F2,$80,$07,$51,$80,$17,$F2,$80,$07,$51,$80,$0F
  db $F2,$80,$07,$51,$80,$07,$F2,$80,$07,$51,$80,$B7,$F2,$80,$0F,$51
  db $80,$0F,$F2,$80,$0F,$51,$80,$0F,$F2,$80,$17,$51,$80,$67,$F2,$80
  db $FE,$F3,$80,$FE,$F3,$F3,$F3,$80,$FF



