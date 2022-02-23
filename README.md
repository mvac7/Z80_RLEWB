# RLEWB

---

## Index

- [1 Description](#1-Description)
- [2 Encoder format](#2-Encoder-format)
- [3 Software to compress in RLEWB](#3-Software-to-compress-in-RLEWB)
- [4 C decoders](#4-C-decoders)
    - [4.1 Requirements](#41-Requirements)
    - [4.2 Decompress RLEWB to RAM](#42-Decompress-RLEWB-to-RAM)
    - [4.3 Decompress RLEWB to VRAM](#43-Decompress-RLEWB-to-VRAM)
- [5 Assembler decoders](#5-Assembler-decoders)
    - [5.1 Requirements](#51-Requirements)
    - [5.2 Decompress RLEWB to RAM](#52-Decompress-RLEWB-to-RAM)
    - [5.3 Decompress RLEWB to VRAM](#53-Decompress-RLEWB-to-VRAM)
- [6 MSX BASIC](#6-MSX-BASIC)
    - [6.1 Decompress RLEWB to RAM](#61-Decompress-RLEWB-to-RAM)
    - [6.2 Decompress RLEWB to VRAM](#62-Decompress-RLEWB-to-VRAM)
- [7 Visual Basic dotnet](#7-Visual-Basic-dotnet)   
    - [7.1 RLEWB encoder](#71-RLEWB-encoder)
    - [7.2 RLEWB encoder](#72-RLEWB-decoder)
- [8 Acknowledgments](#8-Acknowledgments)
- [9 References](#9-References)

<br/>
    
---

## 1 Description

RLEWB is a compressor of the RLE type [(Run-Length
Encoding)](https://en.wikipedia.org/wiki/Run-length_encoding), with the
advantage that it improves the results in data sequences where there are not
many series of repeated values. This is because it does not penalize for the simple values.

This repository collects resources to use this compression algorithm.
It includes the encoder for Visual Basic .net and the decoder for C, Assembler and MSX BASIC.

This project is an Open Source library.

It is primarily designed for graphical data from the TMS9918A, 
where repetitions of values are often found, especially in the color table.
  
In the source code you can find applications for testing and learning purposes.

It is inspired by the [Wonder Boy RLE](https://www.smspower.org/Development/Compression#WonderBoyRLE) compression algorithm, published on the [SMS POWER!](https://www.smspower.org/) WEBSITE.

<br/>

---

## 2 Encoder format



```
 CD = Control Digit = $80

 CD + $0       --> When the value to be written to the output is equal to the Control Digit
 CD + $FF      --> End - Decompressed until it finds this value.
 CD +  nn + dd --> Repeat nn+1 ($2 to $FE) value equivalent to 3 up to 255 repetitions. 
                   In dd is the value to repeat.
 dd (!= CD)    --> Raw data. Values without repetition.
```

### Example:

#### Source:
```---------- HELLO WORLD________4444``` 34 Bytes

#### Result:
```80,09,2D,20,48,45,4C,4C,4F,20,57,4F,52,4C,44,80,07,5F,80,03,34,80,FF``` 23 Bytes

```CD + 9 + "-"``` + ```" HELLO WORLD"``` + ```CD + 7 + "_"``` + ```CD + 3 + "4"``` + ```CD + $FF```

<br/>

---


## 3 Software to compress in RLEWB

- [RLE-WB compressor](https://github.com/ericb59/rlewb-Compressor) for MSX Fusion-C by Eric Boez. `MacOS` `Windows 64bits` `Linux 64bits`
- [ByteniZ3R devtool](https://github.com/mvac7/mSXdevtools_ByteniZ3R) Another generator of data tables in Bytes. `.net 4`


<br/>

---

## 4 C decoders

For direct decompression to VRAM, two libraries (.rel) are available, 
depending on the execution environment (MSX-DOS or BIOS ROM/BASIC).


### 4.1 Requirements

- [Small Device C Compiler (SDCC) v4.1](http://sdcc.sourceforge.net/)
- [Hex2bin v2.5](http://hex2bin.sourceforge.net/)


### 4.2 Decompress RLEWB to RAM

- [`unRLEWBtoRAM.rel`](decode_C/toRAM)
- [`unRLEWBtoRAM.h`](decode_C/toRAM/unRLEWBtoRAM.h)

#### unRLEWBtoRAM

<table>
<tr><th colspan=3 align="left">unRLEWBtoRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to RAM</td></tr>
<tr><th>Function</th><td colspan=2>void unRLEWBtoRAM (unsigned int sourceADDR, unsigned int targetADDR)</td></tr>
<tr><th rowspan=3>Input</th><td>unsigned int</td><td>source data address</td></tr>
<tr><td>unsigned int</td><td>target VRAM address</td></tr>
</table>

**Example**

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

#### unRLEWBRAM (for Assembler inline)

<table>
<tr><th colspan=3 align="left">unRLEWBRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to RAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target RAM address</td></tr>
</table>
 
**Example**

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

### 4.3 Decompress RLEWB to VRAM 

- MSX with BIOS (ROM or MSX BASIC) [`unRLEWBtoVRAM.rel`](decode_C/toVRAM_MSXBIOS)
- MSX-DOS [`unRLEWBtoVRAM_MSXDOS.rel`](decode_C/toVRAM_MSXDOS)
- [`unRLEWBtoVRAM.h`](decode_C/toVRAM_MSXBIOS/unRLEWBtoVRAM.h)


#### unRLEWBtoVRAM

<table>
<tr><th colspan=3 align="left">unRLEWBtoVRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to VRAM</td></tr>
<tr><th>Function</th><td colspan=2>void unRLEWBtoVRAM (unsigned int sourceADDR, unsigned int targetADDR)</td></tr>
<tr><th rowspan=3>Input</th><td>unsigned int</td><td>source data address</td></tr>
<tr><td>unsigned int</td><td>target VRAM address</td></tr>
</table>

 
**Example**

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

#### unRLEWBVRAM (for Assembler inline)

<table>
<tr><th colspan=3 align="left">unRLEWBVRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to VRAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target VRAM address</td></tr>
</table>
 
**Example**

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

## 5 Assembler decoders           

It is available with support for cross-development assemblers: asMSX, Sjasm and tniASM.

An adapted source is available for all three assemblers but it doesn't work with the current version of [asMSX v.1.0.1](https://github.com/Fubukimaru/asMSX/releases), as there is a pending Bug related to Conditional Assembly `[#90](https://github.com/Fubukimaru/asMSX/issues/90)`. 
Therefore you will find asMSX-specific sources for BIOS-based environments (ROM and BASIC) and MSX-DOS.

You can add the following Labels to your main code, to configure the decompressor for different environments:

**Note:** For Sjasm assembler, use DEFINE + Label

- Identify the assembler (`ASMSX` , `SJASM` or `TNIASM`) Default: ASMSX/SJASM
- Runtime (`MSXROM` , `MSXDOS` or `MSXBASIC`) Default: MSXROM/MSXBASIC
- `DIRECTRANGE` if you have a compressor that generated a range from 3 to 254 repetitions.


### 5.1 Requirements

A cross assembler:

- [`asMSX`](https://github.com/Fubukimaru/asMSX) v1.0.1
- [`Sjasm`](http://www.xl2s.tk/) v0.42 by XL2S Entertainment 
- [`tniASM`](http://www.tni.nl/products/tniasm.html) v0.45 by The New Image


### 5.2 Decompress RLEWB to RAM

- For Sjasm and tniASM [`unRLEWBtoRAM.asm`](decode_Z80asm/toRAM/sources/unRLEWBtoRAM.asm)
- For asMSX [`unRLEWBtoRAM_ASMSX.asm`](decode_Z80asm/toRAM/sources/unRLEWBtoRAM_ASMSX.asm)

<table>
<tr><th colspan=3 align="left">unRLEWBtoRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to RAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target RAM address</td></tr>
</table>
 
**Example**
 
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

###  5.3 unRLEWBtoVRAM

- For Sjasm and tniASM [`unRLEWBtoRAM.asm`](decode_Z80asm/toVRAM/sources/unRLEWBtoVRAM.asm)
- For asMSX on MSX BIOS (ROM or BASIC) [`unRLEWBtoVRAM_MSXROM_ASMSX.asm`](decode_Z80asm/toVRAM/sources/unRLEWBtoVRAM_MSXROM_ASMSX.asm)
- For asMSX on MSX-DOS [`unRLEWBtoVRAM_MSXDOS_ASMSX.asm`](decode_Z80asm/toVRAM/sources/unRLEWBtoVRAM_MSXDOS_ASMSX.asm)

<table>
<tr><th colspan=3 align="left">unRLEWBtoVRAM</th></tr>
<tr><td colspan=3>Decompress RLEWB data to VRAM</td></tr>
<tr><th rowspan=3>Input</th><td>HL</td><td>source data address</td></tr>
<tr><td>DE</td><td>target VRAM address</td></tr>
</table>
 
**Example** 

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

## 6 MSX BASIC

The RLEWB decoder is so simple that it can be easily programmed in MSX BASIC. 

Although it may not seem like it, it can be faster than directly reading the data and dumping it to memory, 
since repeated data is written faster. However, the main advantage is still that you can reduce the size of your program.

Remember that when writing the DATA in BASIC it is not necessary to include the zeros.

The best option would be to include the decompression routine in the binary that includes the data and execute it on load, 
but it would be a task that you would have to do yourself since there is no tool that makes it easy for you.

### 6.1 Decompress RLEWB to RAM

- [`UNRLEWBR.BAS`](decode_MSXBASIC/UNRLEWBR.BAS)

To use it, you will have to do a `RESTORE` with the line number where the data starts, 
provide the value of the RAM address to the `DE` variable and do a `GOSUB 9100`.

**Example**
 
```basic
100 REM Test unRLEWB to RAM
110 DEFINT A-Z
120 SCREEN 1
122 WIDTH 32
130 PRINT "Decompress data to RAM"
140 RESTORE 1020
150 DE=&HE000
160 GOSUB 9100
170 PRINT "Copy RAM to VRAM"
180 DE=BASE(5)+(32*2)
190 FOR BC=&HE000 TO &HE160
200 A=PEEK(BC)
210 VPOKE DE,A
220 DE=DE+1
230 NEXT
240 IF INKEY$="" THEN 240
250 LOCATE 0,14
260 END
1000 REM map size width:32 height:11
1010 REM RLE WB compressed - Original size= 352 - Compress size= 103
1020 DATA 24,128,29,23,25,22,128,29,32,22,22,128,29,32,22,22
1030 DATA 128,29,32,22,22,128,8,32,72,101,108,108,111,32,87,111
1040 DATA 114,108,100,33,128,8,32,22,22,128,29,32,22,22,128,29
1050 DATA 32,22,22,128,29,32,22,26,128,4,23,18,128,17,23,18
1060 DATA 128,4,23,27,128,5,32,22,32,80,114,101,115,115,32,97
1070 DATA 110,121,32,107,101,121,128,3,32,22,128,11,32,26,128,17
1080 DATA 23,27,128,5,32,128,255
9000 '=================================
9010 ' unRLEWB to RAM for MSX BASIC
9020 ' Decompress RLEWB data to RAM
9030 ' Input: 
9040 '  RESTORE [line] <-- DATAs
9050 '              DE <-- RAM address
9060 '=================================
9100 READ A
9110 IF A=&H80 THEN 9150
9120 POKE DE,A
9130 DE=DE+1
9140 GOTO 9100
9150 READ A
9160 IF A=&HFF THEN RETURN
9170 IF A=0 THEN A=&H80:GOTO 9120
9180 READ B
9190 FOR DE=DE TO DE+A
9200 POKE DE,B
9210 NEXT
9220 GOTO 9100
```

Run it on [MSXPen](https://msxpen.com/codes/-MwW-L6UR7vwyRo9yMtB)

You can find another example [here](decode_MSXBASIC/examples/TESTRAM.BAS).

<br/>


### 6.2 Decompress RLEWB to VRAM

- [`UNRLEWBV.BAS`](decode_MSXBASIC/UNRLEWBV.BAS)

To use it, you will have to do a `RESTORE` with the line number where the data starts, 
provide the value of the VRAM address to the `DE` variable and do a `GOSUB 9100`.

**Example**

```basic
100 REM Test unRLEWB to VRAM
101 DEFINT A-Z
110 COLOR 15,4,5
120 SCREEN 1
122 WIDTH 32
130 RESTORE 1020
140 DE=BASE(5)
150 GOSUB 9100
160 LOCATE 22,9
170 IF INKEY$="" THEN 170
190 END
1000 REM map size width:32 height:11
1010 REM RLE WB compressed - Original size= 352 - Compress size= 103
1020 DATA 24,128,29,23,25,22,128,29,32,22,22,128,29,32,22,22
1030 DATA 128,29,32,22,22,128,8,32,72,101,108,108,111,32,87,111
1040 DATA 114,108,100,33,128,8,32,22,22,128,29,32,22,22,128,29
1050 DATA 32,22,22,128,29,32,22,26,128,4,23,18,128,17,23,18
1060 DATA 128,4,23,27,128,5,32,22,32,80,114,101,115,115,32,97
1070 DATA 110,121,32,107,101,121,128,3,32,22,128,11,32,26,128,17
1080 DATA 23,27,128,5,32,128,255
9000 '=================================
9010 ' unRLEWB to VRAM for MSX BASIC
9020 ' Decompress RLEWB data to VRAM
9030 ' Input:
9040 '  RESTORE [line] <-- DATAs
9050 '              DE <-- VRAM address
9060 '=================================
9100 READ A
9110 IF A=&H80 THEN 9150
9120 VPOKE DE,A
9130 DE=DE+1
9140 GOTO 9100
9150 READ A
9160 IF A=&HFF THEN RETURN
9170 IF A=0 THEN A=&H80:GOTO 9120
9180 READ B
9190 FOR DE=DE TO DE+A
9200 VPOKE DE,B
9210 NEXT
9220 GOTO 9100
```

Run it on [MSXPen](https://msxpen.com/codes/-MwVuKO4ag8EKVap4Wcv)

You can find another example [here](decode_MSXBASIC/examples/TESTVRAM.BAS).

<br/>

---

## 7 Visual Basic dotnet   

[`VisualBasic_dotnet/RLEWB.vb`](VisualBasic_dotnet/RLEWB.vb)

### 7.1 RLEWB encoder

<table>
<tr><th colspan=3 align="left">Compress</th></tr>
<tr><th>Function</th><td colspan=2>Compress(ByVal data() As Byte) As Byte()</td></tr>
<tr><td colspan=3>Compress the input Byte Array</td></tr>
<tr><th>Input</th><td>Byte()</td><td>Raw data</td></tr>
<tr><th>Output</th><td>Byte()</td><td>Compress data</td></tr>
</table>

<br/>

### 7.2 RLEWB decoder

<table>
<tr><th colspan=3 align="left">Decompress</th></tr>
<tr><th>Function</th><td colspan=2>Decompress(ByVal data() As Byte) As Byte()</td></tr>
<tr><td colspan=3>Decompress the input Byte Array.</td></tr>
<tr><th>Input</th><td>Byte()</td><td>Compress data</td></tr>
<tr><th>Output</th><td>Byte()</td><td>Raw data</td></tr>
</table>

<br/>

---

## 8 Acknowledgments
  
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
- [`MSXPen`](https://msxpen.com/) by [Rafael Jannone](https://twitter.com/jannone)
- fMSX emulator by Marat Fayzullin [`WEB`](https://fms.komkon.org/fMSX/)
- Meisei emulator by Hap >> `?`

---

<br/>
 
## 9 References

- [Run-Length Encoding](https://en.wikipedia.org/wiki/Run-length_encoding)
- [Wonder Boy RLE](https://www.smspower.org/Development/Compression#WonderBoyRLE) compression algorithm.

---