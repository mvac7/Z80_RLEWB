# Z80 RLEWB for SDCC & Assembler

```
Version: 1.3
Date: 24 March 2020
Author: mvac7/303bcn
Architecture: MSX
Programming language: C & Assembler
mail: mvac7303b@gmail.com
```


## Sorry! This text is pending correction of the English translation


### History of versions:
- v1.1 (26 jun 2014) First version 
- v1.2 (20 April 2019) optimized and first version on github
- v1.3 (24 March 2020) SDsnatcher Issue #1 - Enhanced to pass the Acid2Test


## 1. Introduction

This project is an Open Source library.

Object functions for C (SDCC), and Z80 Assembler source for decompress RLE-WB 
data encoding.  

RLEWB is a compressor of the RLE type, with the advantage that it improves the 
results in data sequences where there are not many series of repeated values, 
because it does not penalize for the simple values.

It is designed for graphic data of the TMS9918A.
  
Includes applications for testing and learning purposes.



## 2. Acknowledgments
  
Thanks for Info & help, to:

* SMS Power > http://www.smspower.org/Development/Index
* SDsnatcher > https://github.com/sdsnatcher
* Avelino Herrera > http://msx.atlantes.org/index_es.html
* Nerlaska > http://albertodehoyonebot.blogspot.com.es
* Fubu > http://www.gamerachan.org/fubu/
* Marq/Lieves!Tuore > http://www.kameli.net/lt/
* Sapphire/Z80ST > http://z80st.auic.es/
* Pentacour > http://pentacour.com/
* JamQue/TPM > http://www.thepetsmode.com/
* Andrear > http://andrear.altervista.org/home/msxsoftware.php
* Konamiman > https://www.konamiman.com
* MSX Assembly Page > http://map.grauw.nl/resources/msxbios.php
* Portar MSX Tech Doc > http://nocash.emubase.de/portar.htm
* MSX Resource Center > http://www.msx.org/
* Karoshi MSX Community > http://karoshi.auic.es/
* BlueMSX >> http://www.bluemsx.com/
* OpenMSX >> http://openmsx.sourceforge.net/
* Meisei  >> ?



## 3. Requirements

For C:

 * Small Device C Compiler (SDCC) v3.6 http://sdcc.sourceforge.net/
     
 * Hex2bin v2.2 http://hex2bin.sourceforge.net/


For Assembler:

 * asMSX https://github.com/Fubukimaru/asMSX/releases/

Or:

 * Sjasm (by XL2S Entertainment) http://www.xl2s.tk/



## 4. Encoder description

```
 CD = Control Digit = $80

 CD + $0         --> for one $80 value
 CD + $FF        --> end of data block
 CD + nn + dd    --> repeat nn ($1-$FE)+1 dd value
 dd (!= CD)      --> raw data
```


## 5. Functions

* (SDCC) unRLEWBtoRAM (unsigned int sourceADDR, unsigned int targetADDR) Decompress RLEWB data to RAM
* (SDCC Assembler inline) unRLEWBRAM  <--- HL = data address ; DE = RAM address - Decompress RLEWB data to RAM
* (SDCC) unRLEWBtoVRAM (unsigned int RAM, unsigned int VRAM) Decompress RLEWB data to VRAM
* (SDCC Assembler inline) unRLEWBVRAM  <--- HL = data address ; DE = VRAM address - Decompress RLEWB data to VRAM
* (Assembler) unRLEWBtoRAM <--- HL = data address ; DE = RAM address - Decompress RLEWB data to RAM
* (Assembler) unRLEWBtoVAM <--- HL = data address ; DE = VRAM address - Decompress RLEWB data to VRAM
 