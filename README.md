# RLEWB Run-length WB 


## Sorry! This text is pending correction of the English translation

---

## Description

RLEWB is a compressor of the RLE type [(Run-Length Encoding)](https://en.wikipedia.org/wiki/Run-length_encoding), with the advantage that it improves the results in data sequences where there are not many series of repeated values, 
because it does not penalize for the simple values.

This repository collects resources to use this compression algorithm definition.
It includes the encoder for Visual Basic .net and the decoder for C and Assembler.

This project is an Open Source library.

It is designed for graphical data from the TMS9918A.
  
In the source code you can find applications for testing and learning purposes.

It is inspired by the [Wonder Boy RLE](https://www.smspower.org/Development/Compression#WonderBoyRLE) compression algorithm, published on the [SMS POWER!](https://www.smspower.org/) WEBSITE.

<br/>

---

## Requirements

### For C:

- [Small Device C Compiler (SDCC) v4.1](http://sdcc.sourceforge.net/)
- [Hex2bin v2.5](http://hex2bin.sourceforge.net/)


### For Assembler:

A cross assembler:

- [`asMSX`](https://github.com/Fubukimaru/asMSX) v1.0.1
- [`Sjasm`](http://www.xl2s.tk/) v0.42 by XL2S Entertainment 
- [`tniASM`](http://www.tni.nl/products/tniasm.html) v0.45 by The New Image

<br/>

---

## Encoder format

```
 CD = Control Digit = $80

 CD + $0       --> When the value to be written to the output is equal to the Control Digit
 CD + $FF      --> End - Decompressed until it finds this value.
 CD + nn + dd  --> Repeat nn+1 ($2 to $FE) value equivalent to 3 up to 255 repetitions. In dd is the value to repeat.
 dd (!= CD)    --> Raw data. Values without repetition.
```

<br/>

---


## Software to compress in RLEWB

- [RLE-WB compressor](https://github.com/ericb59/rlewb-Compressor) for MSX Fusion-C by Eric Boez. `MacOS` `Windows 64bits` `Linux 64bits`
- [ByteniZ3R devtool](https://github.com/mvac7/mSXdevtools_ByteniZ3R) Another generator of data tables in Bytes. `.net 4`


<br/>

---

## Functions for C

Two libraries (.rel) are available, depending on the execution environment (MSX-DOS or BIOS ROM/BASIC).


### unRLEWBtoRAM

- [`unRLEWBtoRAM.rel`](unRLEWB_C/toRAM/build)
- [`unRLEWBtoRAM.h`](unRLEWB_C/toRAM/unRLEWBtoRAM.h)

<table>
<tr><th colspan=3 align="left">unRLEWBtoRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to RAM</td></tr>
<tr><th>Function</th><td colspan=2>void unRLEWBtoRAM (unsigned int sourceADDR, unsigned int targetADDR)</td></tr>
<tr><th rowspan=3>Input</th><td>unsigned int</td><td>source data address</td></tr>
<tr><td>unsigned int</td><td>target VRAM address</td></tr>
</table>

**Example**

[`unRLEWB_C/examples/toRAM/test01_SDCC`](unRLEWB_C/examples/toRAM/test01_SDCC)

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

- [`unRLEWBtoRAM.rel`](unRLEWB_C/toRAM/build)
- [`unRLEWBtoRAM.h`](unRLEWB_C/toRAM/unRLEWBtoRAM.h)

<table>
<tr><th colspan=3 align="left">unRLEWBRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to RAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target RAM address</td></tr>
</table>
 
**Example**

[`unRLEWB_C/examples/toRAM/test02_SDCC_AsmInline`](unRLEWB_C/examples/toRAM/test02_SDCC_AsmInline)

```assembly
void SetDATA() __naked
{
__asm 
  ;decompress to RAM
  ld   HL,#DATA_COL
  ld   DE,#0xD000 ;RAM addr
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

- [`unRLEWBtoVRAM.rel`](unRLEWB_C/toVRAM/build)
- [`unRLEWBtoVRAM.h`](unRLEWB_C/toVRAM/unRLEWBtoVRAM.h)

<table>
<tr><th colspan=3 align="left">unRLEWBtoVRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to VRAM</td></tr>
<tr><th>Function</th><td colspan=2>void unRLEWBtoVRAM (unsigned int sourceADDR, unsigned int targetADDR)</td></tr>
<tr><th rowspan=3>Input</th><td>unsigned int</td><td>source data address</td></tr>
<tr><td>unsigned int</td><td>target VRAM address</td></tr>
</table>

 
**Example**
 
[`unRLEWB_C/examples/toVRAM/test03_SDCC`](unRLEWB_C/examples/toVRAM/test03_SDCC) 

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

- [`unRLEWBtoVRAM.rel`](unRLEWB_C/toVRAM/build)
- [`unRLEWBtoVRAM.h`](unRLEWB_C/toVRAM/unRLEWBtoVRAM.h)

<table>
<tr><th colspan=3 align="left">unRLEWBVRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to VRAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target VRAM address</td></tr>
</table>
 
**Example**

[`unRLEWB_C/examples/toVRAM/test04_SDCC_AsmInline`](unRLEWB_C/examples/toVRAM/test04_SDCC_AsmInline)

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

It is available with support for cross-development assemblers: asMSX, Sjasm and tniASM.

An adapted source is available for all three assemblers but it doesn't work with the current version of [asMSX v.1.0.1](https://github.com/Fubukimaru/asMSX/releases), as there is a pending Bug related to Conditional Assembly `[#90](https://github.com/Fubukimaru/asMSX/issues/90)`. 
Therefore you will find asMSX-specific sources for BIOS-based environments (ROM and BASIC) and MSX-DOS.

You can add the following Labels to your main code, to configure the decompressor for different environments:

**Note:** For Sjasm assembler, use DEFINE + Label

- Identify the assembler (`ASMSX` , `SJASM` or `TNIASM`) Default: ASMSX/SJASM
- Runtime (`MSXROM` , `MSXDOS` or `MSXBASIC`) Default: MSXROM/MSXBASIC
- `DIRECTRANGE` if you have a compressor that generated a range from 3 to 254 repetitions.


### unRLEWBtoRAM

[`unRLEWBtoRAM.asm`](unRLEWB_ASM/unRLEWBtoRAM.asm)

<table>
<tr><th colspan=3 align="left">unRLEWBtoRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to RAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target RAM address</td></tr>
</table>
 
**Example**

[`examples/test03_toRAM_Sjasm`](unRLEWB_ASM/examples/test03_toRAM_Sjasm)
 
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

- [`unRLEWB_ASM/unRLEWBtoVRAM_asmsx.asm`](unRLEWB_ASM/unRLEWBtoVRAM_asmsx.asm)
- [`unRLEWB_ASM/unRLEWBtoVRAM_sjasm.asm`](unRLEWB_ASM/unRLEWBtoVRAM_sjasm.asm)

<table>
<tr><th colspan=3 align="left">unRLEWBtoVRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to VRAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target VRAM address</td></tr>
</table>
 
**Example** 

[`unRLEWB_ASM/examples/test02_toVRAM_Sjasm`](unRLEWB_ASM/examples/test02_toVRAM_Sjasm)

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

## MSX BASIC

The RLEWB encoder is so simple that it can be easily programmed in MSX BASIC, but it is very slow.

I've included the decoder, in case you need to reduce the size of your program and you can't use upload files 
(in case the rules of some programming contest require it).

The best option would be to include the decompression routine in the binary itself that includes the data and execute it on load, 
but it would be a task that you would have to do yourself since there is no tool that makes it easy for you.

### Decompress RLEWB to RAM

```basic
4000 REM ==============================================
4010 REM unRLEWB to RAM for BASIC v1 (06 Feb 2022)
4020 REM Decompress RLEWB data to RAM
4030 REM Input: 
4040 REM        RESTORE [line number] --> RLEWB datas
4050 REM        HL --> RAM address            
4060 REM ==============================================
4100 READ A
4110 IF A=&H80 THEN 4150
4120 POKE HL,A
4130 HL=HL+1
4140 GOTO 4100
4150 READ A
4160 IF A=&HFF THEN RETURN
4170 IF A=0 THEN A=&H80:GOTO 4120
4180 READ D
4190 FOR HL=HL TO HL+A
4200 POKE HL,D
4210 NEXT
4220 GOTO 4100
```

To use it, you will have to do a `RESTORE` with the line number where the data starts, 
provide the value of the RAM address to the `ADDR` variable and do a `GOSUB 4100`.

**Example**
* 
```basic
100 REM Test unRLEWB to RAM
110 RESTORE 1020
120 HL=&HE000
130 GOSUB 4100
140 END
1000 REM Tileset Pattern data All BANKs
1010 REM RLE WB compressed - Original size= 6144 - Compress size= 1368
1020 DATA 128,254,,128,10,,1,3,7,15,31,63,15,127,128,213
1030 DATA 255,240,254,128,5,255,,,128,,192,224,240,248,252,128
1040 DATA 15,,63,128,2,127,128,3,255,248,224,192,128,,128,
1050 DATA 128,210,,31,7,3,1,1,128,2,,252,128,2,254,128
...
1870 DATA 128,254,,128,205,,128,255
```

### Decompress RLEWB to VRAM

```basic
4000 REM ==============================================
4010 REM unRLEWB to VRAM for BASIC v1 (06 Feb 2022)
4020 REM Decompress RLEWB data to VRAM
4030 REM Input: 
4040 REM        RESTORE [line number] --> RLEWB datas
4050 REM        HL --> VRAM address            
4060 REM ==============================================
4100 READ A
4110 IF A=&H80 THEN 4150
4120 VPOKE HL,A
4130 HL=HL+1
4140 GOTO 4100
4150 READ A
4160 IF A=&HFF THEN RETURN
4170 IF A=0 THEN A=&H80:GOTO 4120
4180 READ D
4190 FOR HL=HL TO HL+A
4200 VPOKE HL,D
4210 NEXT
4220 GOTO 4100 
```

To use it, you will have to do a `RESTORE` with the line number where the data starts, 
provide the value of the VRAM address to the `VADDR` variable and do a `GOSUB 4100`.

**Example**

You can find the complete example in `\decode_MSXBASIC\examples\TESTVRAM.BAS`

```basic
100 REM Test unRLEWB to VRAM
110 DEFINT A-Z
120 COLOR 15,4,4
130 SCREEN 2
140 RESTORE 1020
150 HL=BASE(12)
160 GOSUB 4100
170 IF INKEY$="" THEN 170
180 END
1000 REM Tileset Pattern data All BANKs
1010 REM RLE WB compressed - Original size= 6144 - Compress size= 1368
1020 DATA 128,254,,128,10,,1,3,7,15,31,63,15,127,128,213
1030 DATA 255,240,254,128,5,255,,,128,,192,224,240,248,252,128
1040 DATA 15,,63,128,2,127,128,3,255,248,224,192,128,,128,
1050 DATA 128,210,,31,7,3,1,1,128,2,,252,128,2,254,128
...
1870 DATA 128,254,,128,205,,128,255
```

<br/>

---

## Visual Basic .net   

### RLEWB Class

[`VisualBasic_dotnet/RLEWB.vb`](VisualBasic_dotnet/RLEWB.vb)

<table>
<tr><th colspan=3 align="left">GetRLEWB</th></tr>
<tr><th>Function</th><td colspan=2>GetRLEWB(ByVal data() As Byte) As Byte()</td></tr>
<tr><td colspan=3>Compress input Byte Array</td></tr>
<tr><th>Input</th><td>Byte()</td><td>Raw data</td></tr>
<tr><th>Output</th><td>Byte()</td><td>Compress data</td></tr>
</table>

<br/>

---

## Acknowledgments
  
I want to give a special thanks to all those who freely share their knowledge with the Retrocomputing developer community.

- SMS Power > [`WEB`](https://www.smspower.org/)
- SDsnatcher > [`WEB`](https://github.com/sdsnatcher)
- Eric Boez > [`gitHub`](https://github.com/ericb59)
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
 