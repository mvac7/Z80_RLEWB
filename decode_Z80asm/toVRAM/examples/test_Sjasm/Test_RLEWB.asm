;-------------------------------------------------------------------------------
; test unWBRLE2VRAM - by mvac7  (27/06/2014)
; Assemble with Sjasm (by XL2S Entertainment) http://www.xl2s.tk/
;-------------------------------------------------------------------------------

  OUTPUT TESTRLEW.ROM
  DEFPAGE 0,4000h,4000h		


;-------------------------------------------------------------------------------
;This label allows you to distinguish the assembler,
;when you need it when using conditional assembly
;Labels: ASMSX , SJASM or TNIASM
;Use it with the IFDEF statement
DEFINE SJASM

;This label allows you to distinguish the runtime environment,
;when you need it when using conditional assembly
;Labels: MSXROM , MSXDOS or MSXBASIC
;Use it with the IFDEF statement
DEFINE MSXROM
;-------------------------------------------------------------------------------


VDPVRAM   EQU $98 ;VRAM Data (Read/Write)
VDPSTATUS EQU $99 ;VDP Status Registers

;System variables RAM addresses
;info by MSX Resource Center <https: //www.msx.org/wiki/System_variables_and_work_area>
MSXVER EQU $002D
FORCLR EQU $F3E9 ;Foreground colour
BAKCLR EQU $F3EA ;Background colour
BDRCLR EQU $F3EB ;Border colour
CLIKSW EQU $F3DB ;0 disabled / 1 enabled
RG1SAV EQU $F3E0 ;VDP REG 1
EXTVDP EQU $FFE7 ;
HIMEM  EQU $FC4A ;Stack at the top of memory



;BIOS
;info by MSX Assembly Page
;System Functions addresses <http://map.grauw.nl/resources/msxbios.php>
CHKRAM EQU $0000 ;Tests RAM and sets RAM slot for the system
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

GTSTCK EQU $00D5 ;Returns the joystick status
GTTRIG EQU $00D8 ;Returns current trigger status
SNSMAT EQU $0141 ;Returns the value of the specified line from the keyboard matrix
KILBUF EQU $0156 ;Clear keyboard buffer


BASE10 EQU $1800 ;base 10 name table
BASE11 EQU $2000 ;base 11 color table
BASE12 EQU $0000 ;base 12 character pattern table
BASE13 EQU $1B00 ;base 13 sprite attribute table
BASE14 EQU $3800 ;base 14 sprite pattern table




;-------------------------------------------------------------------- ROM header
  PAGE 0
  CODE
  
  DB    "AB"
  DW    MAIN
  DW    0,0,0,0,0,0

;--------------------------------------------------------------------- main code
MAIN:
  DI    
  LD   SP,[HIMEM]             ;Stack at the top of memory
  EI


  ;color ,1,1  
  ld   A,1
  ld   [BAKCLR],A
  ld   A,1
  ld   [BDRCLR],A  
  call CHGCLR
  
  ;screen 2
  ld   A,2
  call CHGMOD
	
	
	
  ;carga graficos
  halt
  ld   HL,TILESET_pat
  ld   DE,BASE12
  call unRLEWBtoVRAM
  
  halt
  ld   HL,TILESET_col
  ld   DE,BASE11
  call unRLEWBtoVRAM
 


LOOP:		
  halt
  
  
  ld   A,7         
  call SNSMAT
  sub  A,11111011B    ;ESC
  jp   Z,CHKRAM 
  
  jr   LOOP




;include	"unRLEWBtoRAM.asm"
  include "unRLEWBtoVRAM.asm"  




; Tileset Pattern data All BANKs
; RLE WB compressed - Original size= 6144 - Compress size= 1278
TILESET_pat:
	DB $80,$FE,$00,$80,$0A,$00,$01,$03,$07,$0F,$1F,$3F,$0F,$7F,$80,$D5
	DB $FF,$F0,$FE,$80,$05,$FF,$00,$00,$80,$00,$C0,$E0,$F0,$F8,$FC,$80
	DB $0F,$00,$3F,$80,$02,$7F,$80,$03,$FF,$07,$1F,$3F,$7F,$7F,$80,$D2
	DB $FF,$E0,$F8,$FC,$FE,$FE,$80,$02,$FF,$03,$80,$02,$01,$80,$03,$00
	DB $80,$0F,$FF,$80,$07,$00,$80,$DF,$FF,$80,$07,$00,$80,$0F,$FF,$80
	DB $07,$00,$80,$DF,$FF,$80,$07,$00,$80,$0F,$FF,$80,$07,$00,$80,$0F
	DB $FF,$80,$07,$C0,$80,$07,$07,$80,$07,$FE,$80,$07,$01,$80,$0E,$FF
	DB $80,$00,$80,$06,$FF,$00,$80,$06,$C0,$00,$80,$07,$FF,$80,$07,$C0
	DB $80,$07,$00,$80,$07,$07,$80,$07,$FE,$80,$11,$00,$01,$03,$07,$0F
	DB $0F,$1F,$0F,$7F,$80,$03,$FF,$FE,$FC,$FC,$80,$04,$FF,$1F,$0F,$00
	DB $80,$00,$E0,$F0,$F8,$FC,$FC,$FE,$80,$37,$00,$80,$1F,$FF,$80,$07
	DB $00,$80,$07,$FF,$80,$07,$C0,$80,$07,$07,$80,$07,$FE,$80,$07,$01
	DB $80,$07,$FF,$80,$07,$80,$80,$0F,$00,$80,$07,$FF,$80,$07,$C0,$80
	DB $07,$00,$80,$07,$07,$80,$07,$FE,$80,$0F,$00,$1F,$1F,$80,$05,$3F
	DB $F8,$F8,$80,$05,$F0,$07,$07,$80,$05,$03,$FE,$FE,$80,$05,$FF,$80
	DB $37,$00,$80,$1F,$FF,$80,$07,$00,$80,$0E,$FF,$C0,$80,$06,$FF,$07
	DB $80,$07,$FE,$80,$07,$01,$80,$0E,$FF,$80,$00,$80,$06,$FC,$80,$08
	DB $00,$80,$07,$FF,$80,$07,$C0,$80,$07,$00,$80,$07,$07,$80,$07,$FE
	DB $80,$0F,$00,$80,$07,$3F,$80,$07,$F0,$80,$07,$03,$80,$07,$FF,$80
	DB $37,$00,$80,$1F,$FF,$80,$07,$00,$80,$07,$FF,$80,$07,$C0,$80,$07
	DB $07,$80,$07,$FE,$80,$07,$01,$80,$07,$FF,$80,$07,$80,$80,$0F,$00
	DB $80,$07,$FF,$80,$07,$C0,$80,$07,$00,$80,$07,$07,$80,$07,$FE,$80
	DB $0F,$00,$80,$05,$3F,$1F,$1F,$80,$05,$F0,$F8,$F8,$80,$05,$03,$07
	DB $07,$80,$05,$FF,$FE,$FE,$80,$37,$00,$80,$1F,$FF,$80,$07,$00,$80
	DB $07,$FF,$80,$07,$C0,$80,$07,$07,$80,$07,$FE,$80,$07,$01,$80,$07
	DB $FF,$80,$00,$80,$06,$FF,$00,$80,$06,$FF,$00,$80,$06,$C0,$80,$07
	DB $FF,$C0,$80,$06,$FF,$00,$80,$06,$FF,$07,$80,$06,$87,$FE,$80,$06
	DB $FF,$00,$80,$06,$FF,$00,$80,$06,$FC,$1F,$0F,$0F,$07,$03,$01,$00
	DB $00,$FC,$FE,$80,$03,$FF,$7F,$0F,$0F,$1F,$80,$04,$FF,$FC,$FE,$FC
	DB $FC,$F8,$F0,$E0,$80,$00,$80,$38,$00,$80,$1F,$FF,$80,$DF,$00,$80
	DB $1F,$FF,$80,$DF,$00,$80,$1F,$FF,$80,$DF,$00,$80,$1F,$FF,$80,$07
	DB $00,$FF,$FF,$80,$03,$7F,$3F,$3F,$C0,$C0,$80,$03,$E0,$F0,$F0,$0F
	DB $0F,$80,$03,$1F,$3F,$3F,$FC,$FC,$80,$03,$FE,$FF,$FF,$00,$00,$80
	DB $03,$01,$03,$03,$80,$07,$FF,$C0,$C0,$80,$02,$80,$81,$01,$03,$01
	DB $0F,$3F,$7F,$80,$09,$FF,$C3,$81,$80,$00,$F0,$FC,$FE,$80,$03,$FF
	DB $80,$04,$00,$80,$00,$80,$00,$C0,$80,$07,$1F,$80,$06,$FF,$F8,$80
	DB $06,$FF,$0F,$00,$E0,$F8,$FC,$FE,$FE,$FF,$FF,$80,$07,$00,$80,$07
	DB $FF,$80,$07,$C0,$80,$07,$00,$80,$07,$07,$80,$06,$FF,$FE,$80,$06
	DB $FF,$0F,$80,$00,$F0,$FC,$FE,$80,$03,$FF,$80,$04,$00,$80,$00,$80
	DB $00,$C0,$80,$04,$07,$80,$02,$03,$80,$04,$FF,$80,$02,$FE,$80,$07
	DB $00,$80,$1F,$FF,$80,$07,$00,$3F,$3F,$80,$03,$1F,$0F,$0F,$F0,$F0
	DB $80,$03,$F8,$FC,$FC,$3F,$3F,$80,$03,$7F,$80,$09,$FF,$03,$03,$80
	DB $03,$87,$CF,$CF,$FF,$FF,$80,$03,$FE,$FC,$FC,$03,$03,$80,$05,$07
	DB $FF,$FF,$80,$05,$FE,$80,$07,$00,$FF,$FF,$80,$05,$7F,$C0,$C0,$80
	DB $05,$E0,$80,$07,$1F,$80,$07,$F8,$03,$80,$05,$01,$03,$80,$07,$FF
	DB $00,$80,$05,$80,$00,$80,$07,$FF,$80,$07,$C0,$80,$07,$00,$80,$07
	DB $07,$80,$07,$FE,$03,$01,$01,$80,$04,$00,$80,$05,$FF,$7F,$7F,$C0
	DB $C0,$80,$05,$E0,$80,$05,$03,$01,$01,$80,$05,$FE,$FC,$FC,$80,$07
	DB $00,$80,$1F,$FF,$80,$07,$00,$0F,$0F,$80,$03,$07,$03,$03,$FC,$FC
	DB $80,$13,$FF,$3F,$3F,$CF,$CF,$80,$05,$FF,$FC,$FC,$80,$03,$F8,$F0
	DB $F0,$80,$07,$07,$80,$07,$FE,$80,$07,$00,$80,$07,$7F,$80,$07,$E0
	DB $80,$07,$1F,$F8,$80,$06,$FF,$0F,$80,$08,$FF,$FE,$FE,$FC,$F8,$F0
	DB $C0,$80,$07,$00,$80,$07,$FF,$80,$07,$C0,$80,$07,$00,$80,$07,$07
	DB $80,$07,$FE,$80,$07,$00,$80,$07,$7F,$80,$07,$E0,$80,$06,$01,$00
	DB $80,$06,$FC,$F8,$80,$07,$00,$80,$1F,$FF,$80,$07,$00,$03,$03,$80
	DB $03,$01,$00,$00,$80,$09,$FF,$80,$03,$FE,$FC,$FC,$3F,$3F,$80,$03
	DB $1F,$0F,$0F,$80,$07,$FF,$F0,$F0,$80,$03,$E0,$C0,$C0,$80,$05,$07
	DB $03,$03,$80,$05,$FE,$FF,$FF,$80,$07,$00,$80,$05,$7F,$FF,$FF,$80
	DB $05,$E0,$C0,$C0,$80,$07,$1F,$80,$07,$F8,$7F,$7F,$3F,$3F,$1F,$1F
	DB $0F,$0F,$E0,$E0,$F0,$F0,$F8,$F8,$FC,$FC,$80,$07,$00,$80,$07,$FF
	DB $80,$07,$C0,$80,$07,$00,$80,$07,$07,$80,$07,$FE,$80,$04,$00,$01
	DB $01,$03,$7F,$7F,$80,$05,$FF,$80,$05,$E0,$C0,$C0,$80,$06,$00,$01
	DB $80,$03,$F8,$80,$02,$00,$FC,$80,$07,$00,$80,$1F,$FF,$80,$0F,$00
	DB $FF,$FF,$80,$03,$7F,$3F,$3F,$FC,$FC,$80,$03,$F8,$F0,$F0,$0F,$0F
	DB $80,$03,$07,$03,$03,$80,$07,$FF,$C0,$C0,$80,$03,$80,$00,$00,$03
	DB $01,$01,$80,$04,$00,$80,$03,$FF,$7F,$3F,$0F,$01,$81,$C3,$80,$09
	DB $FF,$FE,$FC,$F0,$80,$00,$C0,$80,$00,$80,$00,$80,$04,$00,$80,$07
	DB $1F,$80,$07,$F8,$07,$07,$03,$03,$01,$01,$00,$00,$FE,$FE,$80,$05
	DB $FF,$80,$03,$00,$80,$00,$80,$00,$C0,$C0,$80,$07,$FF,$C0,$80,$06
	DB $FF,$00,$80,$06,$FF,$07,$80,$06,$87,$FE,$80,$06,$FF,$0F,$80,$0A
	DB $FF,$FE,$FC,$F0,$80,$00,$C0,$80,$00,$80,$00,$80,$04,$00,$80,$06
	DB $03,$01,$80,$06,$FE,$FC,$80,$07,$00,$80,$1F,$FF,$80,$DF,$00,$80
	DB $1B,$FF,$80,$02,$80,$C0,$80,$02,$FF,$7F,$7F,$3F,$1F,$07,$80,$D2
	DB $FF,$FE,$FE,$FC,$F8,$E0,$80,$03,$00,$80,$02,$01,$03,$80,$0F,$FF
	DB $C0,$E0,$F0,$F8,$FC,$FE,$FF,$FF,$80,$05,$00,$80,$00,$F0,$80,$07
	DB $00,$01,$80,$06,$00,$80,$08,$FF,$FE,$FC,$F8,$F0,$E0,$C0,$80,$00
	DB $80,$00,$80,$B4,$00,$01,$0F,$03,$07,$0F,$1F,$3F,$7F,$80,$29,$FF
	DB $80,$07,$00,$FF,$FE,$FC,$F8,$F0,$E0,$C0,$80,$04,$00,$01,$03,$07
	DB $0F,$0F,$3F,$7F,$80,$E4,$FF,$80,$06,$00,$80,$00,$80,$03,$00,$01
	DB $03,$07,$1F,$1F,$3F,$7F,$80,$FE,$FF,$80,$CD,$FF,$80,$FF

; Tileset Color data All BANKs
; RLE WB compressed - Original size= 6144 - Compress size= 209
TILESET_col:
	DB $80,$FE,$00,$80,$0A,$00,$80,$FE,$70,$80,$06,$70,$80,$DF,$D7,$80
	DB $1F,$07,$80,$E7,$D7,$80,$17,$07,$80,$E7,$D7,$80,$17,$07,$80,$07
	DB $D7,$80,$07,$F7,$80,$CF,$FD,$80,$07,$7D,$80,$0F,$0D,$80,$0F,$7D
	DB $80,$D7,$FD,$80,$07,$7D,$80,$0F,$0D,$80,$0F,$7D,$80,$D7,$FD,$80
	DB $07,$7D,$80,$0F,$0D,$80,$0F,$7D,$80,$D7,$FD,$80,$07,$7D,$80,$0F
	DB $0D,$80,$0F,$7D,$80,$D7,$FD,$80,$07,$7D,$80,$0F,$0D,$80,$EF,$7D
	DB $80,$0F,$0D,$80,$EF,$7D,$80,$0F,$0D,$80,$EF,$7D,$80,$0F,$0D,$80
	DB $0F,$7D,$80,$D7,$FD,$80,$07,$7D,$80,$0F,$0D,$80,$0F,$7D,$80,$D7
	DB $FD,$80,$07,$7D,$80,$0F,$0D,$80,$0F,$7D,$80,$D7,$FD,$80,$07,$7D
	DB $80,$0F,$0D,$80,$0F,$7D,$80,$D7,$FD,$80,$07,$7D,$80,$0F,$0D,$80
	DB $17,$7D,$80,$CF,$FD,$80,$07,$7D,$80,$0F,$0D,$80,$EF,$7D,$80,$0F
	DB $0D,$80,$03,$7D,$80,$03,$07,$80,$E3,$D7,$80,$2B,$07,$80,$CD,$D7
	DB $80,$39,$07,$80,$0B,$D7,$80,$FE,$07,$80,$FE,$07,$80,$CD,$07,$80
	DB $FF



