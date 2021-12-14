# Z80 RLEWB for SDCC and Assembler

```
Author: mvac7
Architecture: MSX
Programming language: C or Assembler
```



## Sorry! This text is pending correction of the English translation


---

## History of versions:
- v1.1 (26 jun 2014) First version 
- v1.2 (20 April 2019) optimized and first version on github
- v1.3 (24 March 2020) SDsnatcher Issue #1 - Enhanced to pass the Acid2Test

<br/>

---

## Description

This project is an Open Source library.

Object functions for C (SDCC), and Z80 Assembler source for decompress RLE-WB data encoding.  

RLEWB is a compressor of the RLE type, with the advantage that it improves the 
results in data sequences where there are not many series of repeated values, 
because it does not penalize for the simple values.

It is designed for graphic data of the TMS9918A.
  
Includes applications for testing and learning purposes.

It is inspired by the [Wonder Boy RLE](https://www.smspower.org/Development/Compression#WonderBoyRLE) compression algorithm, published on the [SMS POWER!](https://www.smspower.org/) WEBSITE.

<br/>

---

## Requirements

### For C:

- [Small Device C Compiler (SDCC) v4.1](http://sdcc.sourceforge.net/)
- [Hex2bin v2.5](http://hex2bin.sourceforge.net/)


### For Assembler:

A cross assembler:

- [asMSX](https://github.com/Fubukimaru/asMSX) v1.0.1
- [SJasm](http://www.xl2s.tk/) v0.42 by XL2S Entertainment

<br/>

---

## Encoder description

```
 CD = Control Digit = $80

 CD + $0         --> When the value to be written to the output is equal to the Control Digit
 CD + $FF        --> End - Decompressed until it finds this value.
 CD + nn + dd    --> Repeat nn ($1-$FE)+1 dd value
 dd (!= CD)      --> Raw data. Values without repetition.
```

<br/>

---

## Functions for C

### unRLEWBtoRAM

- `unRLEWBtoRAM.rel`
- `unRLEWBtoRAM.h`

<table>
<tr><th colspan=3 align="left">unRLEWBtoRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to RAM</td></tr>
<tr><th>Function</th><td colspan=2>void unRLEWBtoRAM (unsigned int sourceADDR, unsigned int targetADDR)</td></tr>
<tr><th rowspan=3>Input</th><td>unsigned int</td><td>source data address</td></tr>
<tr><td>unsigned int</td><td>target VRAM address</td></tr>
</table>

**Example**

`unRLEWB_C/examples/toRAM/test01_SDCC`

```c            
// RLE WB compressed - Original size= 2048 - Compress size= 33
const char DATA_COL[]={
  0x80,0xFE,0xFC,0x80,0xFE,0xFC,0xFC,0xFC,0x80,0x3F,0xF2,0x80,
  0x0F,0x51,0x80,0x0F,0xF2,0x80,0x0F,0x51,0x80,0x0F,0xF2,0x80,
  0x07,0x51,0x80,0xAF,0xF2,0x80,0x07,0x51,0x80,0x17,0xF2,0x80,
  0x07,0x51,0x80,0x0F,0xF2,0x80,0x07,0x51,0x80,0x07,0xF2,0x80,
  0x07,0x51,0x80,0xAF,0xF2,0x80,0x07,0x51,0x80,0x17,0xF2,0x80,
  0x07,0x51,0x80,0x0F,0xF2,0x80,0x07,0x51,0x80,0x07,0xF2,0x80,
  0x07,0x51,0x80,0xB7,0xF2,0x80,0x0F,0x51,0x80,0x0F,0xF2,0x80,
  0x0F,0x51,0x80,0x0F,0xF2,0x80,0x17,0x51,0x80,0x67,0xF2,0x80,
  0xFE,0xF3,0x80,0xFE,0xF3,0xF3,0xF3,0x80,0xFF};

void main()
{
    unRLEWBtoRAM ((unsigned int) DATA_COL, 0xD000);
}
```

<br/>

### unRLEWBRAM (for Assembler inline)

- `unRLEWBtoRAM.rel`
- `unRLEWBtoRAM.h`

<table>
<tr><th colspan=3 align="left">unRLEWBRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to RAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target RAM address</td></tr>
</table>
 
**Example**

`unRLEWB_C/examples/toRAM/test02_SDCC`

```assembly
void SetDATA() __naked
{
__asm 
  ;decompress to RAM
  ld   HL,#DATA_COL
  ld   DE,#0xD000 
  call unRLEWBRAM
  
  ;copy to VRAM
  ld   HL,#0xD000 ;RAM addr
  ld   DE,#0x2000 ;VRAM addr
  ld   BC,#0x800  ;length
  call 0x005C     ;LDIRVM
  
  ret
  
DATA_COL:
  .db 0x80,0xFE,0xFC,0x80,0xFE,0xFC,0xFC,0xFC,0x80,0x3F,0xF2,0x80
  .db 0x0F,0x51,0x80,0x0F,0xF2,0x80,0x0F,0x51,0x80,0x0F,0xF2,0x80
  .db 0x07,0x51,0x80,0xAF,0xF2,0x80,0x07,0x51,0x80,0x17,0xF2,0x80
  .db 0x07,0x51,0x80,0x0F,0xF2,0x80,0x07,0x51,0x80,0x07,0xF2,0x80
  .db 0x07,0x51,0x80,0xAF,0xF2,0x80,0x07,0x51,0x80,0x17,0xF2,0x80
  .db 0x07,0x51,0x80,0x0F,0xF2,0x80,0x07,0x51,0x80,0x07,0xF2,0x80
  .db 0x07,0x51,0x80,0xB7,0xF2,0x80,0x0F,0x51,0x80,0x0F,0xF2,0x80
  .db 0x0F,0x51,0x80,0x0F,0xF2,0x80,0x17,0x51,0x80,0x67,0xF2,0x80
  .db 0xFE,0xF3,0x80,0xFE,0xF3,0xF3,0xF3,0x80,0xFF

__endasm;
}
```

<br/> 

### unRLEWBtoVRAM

- `unRLEWBtoVRAM.rel`
- `unRLEWBtoVRAM.h`

<table>
<tr><th colspan=3 align="left">unRLEWBtoVRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to VRAM</td></tr>
<tr><th>Function</th><td colspan=2>void unRLEWBtoVRAM (unsigned int sourceADDR, unsigned int targetADDR)</td></tr>
<tr><th rowspan=3>Input</th><td>unsigned int</td><td>source data address</td></tr>
<tr><td>unsigned int</td><td>target VRAM address</td></tr>
</table>
 
**Example**
 
`unRLEWB_C/examples/toVRAM/test03_SDCC` 


```c            
// RLE WB compressed - Original size= 2048 - Compress size= 33
const char DATA_COL[]={
  0x80,0xFE,0xFC,0x80,0xFE,0xFC,0xFC,0xFC,0x80,0x3F,0xF2,0x80,
  0x0F,0x51,0x80,0x0F,0xF2,0x80,0x0F,0x51,0x80,0x0F,0xF2,0x80,
  0x07,0x51,0x80,0xAF,0xF2,0x80,0x07,0x51,0x80,0x17,0xF2,0x80,
  0x07,0x51,0x80,0x0F,0xF2,0x80,0x07,0x51,0x80,0x07,0xF2,0x80,
  0x07,0x51,0x80,0xAF,0xF2,0x80,0x07,0x51,0x80,0x17,0xF2,0x80,
  0x07,0x51,0x80,0x0F,0xF2,0x80,0x07,0x51,0x80,0x07,0xF2,0x80,
  0x07,0x51,0x80,0xB7,0xF2,0x80,0x0F,0x51,0x80,0x0F,0xF2,0x80,
  0x0F,0x51,0x80,0x0F,0xF2,0x80,0x17,0x51,0x80,0x67,0xF2,0x80,
  0xFE,0xF3,0x80,0xFE,0xF3,0xF3,0xF3,0x80,0xFF};

void main()
{  
  unRLEWBtoVRAM ((unsigned int) DATA_COL, 0x2000);
}
```          
          
<br/>

### unRLEWBVRAM (for Assembler inline)

- `unRLEWBtoVRAM.rel`
- `unRLEWBtoVRAM.h`

<table>
<tr><th colspan=3 align="left">unRLEWBVRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to VRAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target VRAM address</td></tr>
</table>
 
**Example**

`unRLEWB_ASM/examples/toVRAM/test04_SDCC_AsmInline`
 

```assembly
void SetGFX() __naked
{
__asm  
  ld   HL,#DATA_COL
  ld   DE,#0x2000   ;BASE11 Color Table
  call unRLEWBVRAM
  
  ret
  
  
; RLE WB compressed - Original size= 2048 - Compress size= 105  
DATA_COL:
  .db 0x80,0xFE,0xFC,0x80,0xFE,0xFC,0xFC,0xFC,0x80,0x3F,0xF2,0x80
  .db 0x0F,0x51,0x80,0x0F,0xF2,0x80,0x0F,0x51,0x80,0x0F,0xF2,0x80
  .db 0x07,0x51,0x80,0xAF,0xF2,0x80,0x07,0x51,0x80,0x17,0xF2,0x80
  .db 0x07,0x51,0x80,0x0F,0xF2,0x80,0x07,0x51,0x80,0x07,0xF2,0x80
  .db 0x07,0x51,0x80,0xAF,0xF2,0x80,0x07,0x51,0x80,0x17,0xF2,0x80
  .db 0x07,0x51,0x80,0x0F,0xF2,0x80,0x07,0x51,0x80,0x07,0xF2,0x80
  .db 0x07,0x51,0x80,0xB7,0xF2,0x80,0x0F,0x51,0x80,0x0F,0xF2,0x80
  .db 0x0F,0x51,0x80,0x0F,0xF2,0x80,0x17,0x51,0x80,0x67,0xF2,0x80
  .db 0xFE,0xF3,0x80,0xFE,0xF3,0xF3,0xF3,0x80,0xFF
__endasm;
}
```

<br/>

---

## Assembler Routines            

### unRLEWBtoRAM

- `unRLEWBtoRAM.asm`

<table>
<tr><th colspan=3 align="left">unRLEWBtoRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to RAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target RAM address</td></tr>
</table>
 
**Example**

`unRLEWB_ASM/examples/test03_toRAM_Sjasm`
 
```assembly
  ;decompress to RAM
  ld   HL,DATA_COL
  ld   DE,0xD000
  call unRLEWBtoRAM
  
  ;copy to VRAM
  ld   HL,#0xD000 ;RAM addr
  ld   DE,#0x2000 ;VRAM addr
  ld   BC,#800    ;length
  call 0x005C     ;LDIRVM
  
  ret

; RLE WB compressed - Original size= 2048 - Compress size= 105  
DATA_COL:
  db $80,$FE,$FC,$80,$FE,$FC,$FC,$FC,$80,$3F,$F2,$80,$0F,$51,$80,$0F
  db $F2,$80,$0F,$51,$80,$0F,$F2,$80,$07,$51,$80,$AF,$F2,$80,$07,$51
  db $80,$17,$F2,$80,$07,$51,$80,$0F,$F2,$80,$07,$51,$80,$07,$F2,$80
  db $07,$51,$80,$AF,$F2,$80,$07,$51,$80,$17,$F2,$80,$07,$51,$80,$0F
  db $F2,$80,$07,$51,$80,$07,$F2,$80,$07,$51,$80,$B7,$F2,$80,$0F,$51
  db $80,$0F,$F2,$80,$0F,$51,$80,$0F,$F2,$80,$17,$51,$80,$67,$F2,$80
  db $FE,$F3,$80,$FE,$F3,$F3,$F3,$80,$FF
```

<br/>

###  unRLEWBtoVRAM

- `unRLEWBtoVRAM_asmsx.asm`
- `unRLEWBtoVRAM_sjasm.asm`

<table>
<tr><th colspan=3 align="left">unRLEWBtoVRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to VRAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target VRAM address</td></tr>
</table>
 
**Example** 

`unRLEWB_ASM/examples/test02_toVRAM_Sjasm`

```assembly 
  ld   HL,DATA_COL
  ld   DE,$2000   ;BASE11 Color Table
  call unRLEWBtoVRAM            
  ret

; RLE WB compressed - Original size= 2048 - Compress size= 105  
DATA_COL:
  db $80,$FE,$FC,$80,$FE,$FC,$FC,$FC,$80,$3F,$F2,$80,$0F,$51,$80,$0F
  db $F2,$80,$0F,$51,$80,$0F,$F2,$80,$07,$51,$80,$AF,$F2,$80,$07,$51
  db $80,$17,$F2,$80,$07,$51,$80,$0F,$F2,$80,$07,$51,$80,$07,$F2,$80
  db $07,$51,$80,$AF,$F2,$80,$07,$51,$80,$17,$F2,$80,$07,$51,$80,$0F
  db $F2,$80,$07,$51,$80,$07,$F2,$80,$07,$51,$80,$B7,$F2,$80,$0F,$51
  db $80,$0F,$F2,$80,$0F,$51,$80,$0F,$F2,$80,$17,$51,$80,$67,$F2,$80
  db $FE,$F3,$80,$FE,$F3,$F3,$F3,$80,$FF 
```            

<br/>

---

## Acknowledgments
  
I want to give a special thanks to all those who freely share their knowledge with the Retrocomputing developer community.

- SMS Power > [`WEB`](https://www.smspower.org/)
- SDsnatcher > [`WEB`](https://github.com/sdsnatcher)
- Avelino Herrera > [`WEB`](http://msx.avelinoherrera.com/index_es.html)
- Nerlaska > [`Blog`](http://albertodehoyonebot.blogspot.com.es)
- MSXKun/Paxanga soft > [`WEB`](http://paxangasoft.retroinvaders.com/)
- Fubukimaru [`gitHub`](https://github.com/Fubukimaru)
- Marq > [`Marq`](http://www.kameli.net/marq/)
- Sapphire/Z80ST > [`WEB`](http://z80st.auic.es/)
- Andrear > [`Blog`](http://andrear.altervista.org/home/msxsoftware.php)
- Konamiman [`gitHub`](https://github.com/Konamiman) [`WEB`](https://www.konamiman.com/msx/msx-e.html)
- MSX Assembly Page > [`WEB`](http://map.grauw.nl/resources/msxbios.php)
- Portar MSX Tech Doc > [`WEB`](https://problemkaputt.de/portar.htm)
- MSX Resource Center > [`WEB`](http://www.msx.org/)
- Karoshi MSX Community > [`WEB`](http://karoshi.auic.es/)
- BlueMSX emulator >> [`WEB`](http://www.bluemsx.com/)
- OpenMSX emulator >> [`WEB`](http://openmsx.sourceforge.net/)
- [`WebMSX`](https://webmsx.org/) emulator by Paulo A. Peccin >> [`gitHub`](https://github.com/ppeccin/webmsx)
- fMSX emulator by Marat Fayzullin [`WEB`](https://fms.komkon.org/fMSX/)
- Meisei emulator by Hap >> `?`

---
 