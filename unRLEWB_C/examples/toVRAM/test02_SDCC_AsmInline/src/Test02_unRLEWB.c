/* =============================================================================
   test RLEWB & MSX TILES devtool
   v1.0 (27 jun 2014)
   Description:
    test output data from MSX TILES devtool or tMSgfX
    & test unRLEWB2VRAM library (RLE WB to VRAM decompress)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/VDP_TMS9918.h"
#include "../include/unRLEWBtoVRAM.h"



#define  HALT __asm halt __endasm   //wait for the next interrupt



// Definitions of Labels -------------------------------------------------------

//BIOS addresses
#define _CHKRAM 0x0000
#define _CALSLT 0x001C
#define _CHGET  0x009F
#define _GTSTCK 0x00D5
#define _GTTRIG 0x00D8
#define _SNSMAT 0x0141
#define _KILBUF 0x0156



//  definition of functions  ---------------------------------------------------
void testRLEWB();

void showPicture();


void vpoke_block(uint address, char* block, uint size);
void WAIT(uint cicles);
byte inkey();

void print0(byte column, byte line, char* text); //print in screen 0
void print(byte column, byte line, char* text);  //print in screen 1 or 2
void vprint(uint vaddr, char* text);






// Functions -------------------------------------------------------------------


//
void main(void)
{

  screen(1);
  color(15,4,4);

  HALT;
  print(4, 9,"Test MSX TILES devtool &");
  print(4,10,"RLE WB to VRAM decompress"); 
  
  WAIT(220);
     
  testRLEWB();


  return;
}



/* =============================================================================
One character input (waiting)
============================================================================= */
byte inkey(){
__asm   
   call    _CHGET
   ld l,a
__endasm;
}



// Generates a pause in the execution of n interruptions.
// PAL: 50=1second. ; NTSC: 60=1second. 
void WAIT(uint cicles)
{
  uint i;
  for(i=0;i<cicles;i++) HALT;
  return;
}



//print in screen 0
void print0(byte column, byte line, char* text)
{
  uint vaddr = BASE0 + (line*40)+column; // calcula la posicion en la VRAM
  vprint(vaddr, text);
}



//print in screen 1 or 2
void print(byte column, byte line, char* text)
{
  uint vaddr = BASE10 + (line*32)+column; // calcula la posicion en la VRAM
  vprint(vaddr, text);
}



void vprint(uint vaddr, char* text)
{
  while(*(text)) vpoke(vaddr++,*(text++));
}



void vpoke_block(uint address, char* block, uint size)
{    
  while (size-->0){ vpoke(address++,*(block++));}
}



void testRLEWB()
{
  color(0,0,1);
  screen(2);
//  setSpritesSize(0);
//  setSpritesZoom(true);
  
  showPicture();

  
  while(1) //to infinity and beyond!
  {
    HALT;
  }
   
}




void showPicture() __naked
{
__asm 

  halt
  ld   HL,#TILESET_pat
  ld   DE,#BASE12
  call unRLEWBVRAM
  
  ld   HL,#TILESET_col
  ld   DE,#BASE11
  call unRLEWBVRAM

  ret
  

; tileset EGYPTIAN
; pattern data
; RLE WB compressed - Original size= 6144 - Final size= 1677
TILESET_pat:
.db 0x80,0xFE,0x00,0x80,0xA5,0x00,0xE0,0x8F,0x3F,0x80,0x04,0x00,0x07,0x0E,0x03,0x80
.db 0xE8,0x00,0x07,0xFF,0xFF,0x80,0x03,0x00,0x07,0x80,0x03,0x00,0xE0,0x80,0x02,0x03
.db 0x1F,0x80,0x02,0xFF,0x03,0xC0,0xC0,0x00,0xE0,0xFF,0xFF,0x80,0xEB,0x00,0x03,0x80
.db 0x06,0x00,0xC0,0x80,0xFE,0x00,0x80,0xFE,0x00,0x80,0x32,0x00,0x01,0x03,0x07,0x0F
.db 0x1F,0x3F,0x00,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0x80,0x36,0x00,0x03,0x80,0x0E
.db 0x00,0xC0,0x00,0x1C,0xE1,0xC1,0xC1,0x81,0x80,0x00,0x90,0xFF,0x01,0x80,0x02,0xF0
.db 0xE0,0xC0,0xC1,0xFF,0x03,0x80,0x03,0x00,0x80,0x00,0x80,0x03,0x00,0x80,0x00,0xC0
.db 0xE0,0xE0,0xF0,0x80,0x81,0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0x80,0x0E,0xFF
.db 0x80,0x00,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0x80,0x2D,0x00,0x07,0x1F,0x1F,0x07
.db 0x80,0x03,0x00,0xFF,0xFF,0xF0,0xFE,0xFE,0x7F,0x1F,0x07,0x83,0xE1,0x67,0x07,0x07
.db 0x87,0xE7,0xF7,0xFF,0xFF,0x7C,0x7C,0xF8,0xF8,0x80,0x02,0xF7,0xFF,0x80,0x02,0x00
.db 0xFE,0x80,0x02,0xFF,0xC7,0x30,0x80,0x03,0x00,0x80,0x00,0xE0,0xF0,0x80,0x79,0x00
.db 0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0x80,0x1E,0xFF,0x80,0x00,0xC0,0xE0,0xF0,0xF8
.db 0xFC,0xFE,0xFF,0x80,0x19,0x00,0x01,0x03,0x03,0x80,0x02,0x00,0x7F,0xFF,0xFF,0xF0
.db 0x80,0x03,0x00,0xFF,0xFF,0x01,0x00,0x00,0x03,0x07,0x3F,0xF8,0xF8,0xFF,0x3F,0x0F
.db 0x80,0x02,0xFF,0x7F,0x7F,0x80,0x06,0xFF,0xFE,0xFC,0x80,0x02,0xFF,0xFE,0xFC,0x01
.db 0x00,0x00,0xC0,0xE0,0xF0,0xF8,0x7C,0xF0,0xF8,0x80,0x02,0x38,0x18,0x18,0x08,0x80
.db 0x21,0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x00,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF
.db 0x80,0x41,0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0x80,0x2E,0xFF,0x80,0x00,0xC0
.db 0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0x80,0x19,0x00,0x01,0x01,0x07,0x0F,0x0F,0x1F,0x7F
.db 0xFF,0xFC,0xFC,0xE0,0xC0,0x80,0x00,0x00,0xFF,0xFF,0x1F,0x1F,0x3E,0x3C,0x38,0x78
.db 0xFF,0xF7,0xCF,0x80,0x04,0x0F,0xF8,0xFC,0xFE,0xE0,0x80,0x03,0xF0,0xC3,0xE1,0xE1
.db 0xF1,0x70,0xF0,0xF8,0xF9,0x80,0x21,0xFF,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0x80
.db 0x0E,0xFF,0x80,0x00,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0x80,0x31,0x00,0x01,0x03
.db 0x07,0x0F,0x1F,0x3F,0x7F,0x80,0x3E,0xFF,0x80,0x17,0x00,0xE1,0xC3,0xC3,0xC7,0xCF
.db 0xCF,0x80,0x09,0xFF,0x87,0x80,0x05,0x8F,0xCF,0xF0,0xF0,0x80,0x02,0xF8,0x80,0x02
.db 0xFC,0x07,0x07,0x87,0x83,0x83,0x80,0x02,0xC3,0x06,0x06,0x04,0x80,0x1E,0x00,0x01
.db 0x03,0x07,0x0F,0x1F,0x3F,0x7F,0x80,0x1E,0xFF,0x80,0x00,0xC0,0xE0,0xF0,0xF8,0xFC
.db 0xFE,0xFF,0x80,0x21,0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0x80,0x46,0xFF,0x80
.db 0x27,0x00,0x30,0x30,0x80,0x05,0x00,0x01,0x80,0x06,0x00,0xC1,0xC1,0xE0,0xE0,0x80
.db 0x02,0xF0,0xF8,0x80,0x04,0xFF,0x80,0x02,0x7F,0x80,0x11,0xFF,0x01,0x03,0x07,0x0F
.db 0x1F,0x3F,0x7F,0x80,0x2E,0xFF,0x80,0x00,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0x80
.db 0x11,0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0x80,0x4E,0xFF,0x80,0x37,0x00,0x80
.db 0x05,0x07,0x03,0x03,0x80,0x00,0x80,0x00,0x80,0x02,0xC0,0x80,0x02,0xE0,0x80,0x09
.db 0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0x80,0x3E,0xFF,0x80,0x00,0xC0,0xE0,0xF0
.db 0xF8,0xFC,0xFE,0xFF,0x00,0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0x80,0x56,0xFF
.db 0x80,0x37,0x00,0x80,0x02,0x03,0x80,0x03,0x01,0x00,0xE0,0xF0,0xF0,0x80,0x02,0xF8
.db 0xFC,0xFC,0x80,0x08,0x00,0xF0,0x80,0x06,0xFF,0x00,0xFF,0x00,0xFF,0xF0,0x80,0x02
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x03,0xFF,0x0F,0xFF,0x3F
.db 0xFF,0xF0,0x80,0x06,0xFF,0x00,0xFF,0x00,0xFF,0xF0,0x80,0x02,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0x80,0x3F,0xFF,0x03
.db 0x03,0x01,0x80,0x03,0x81,0xC0,0x80,0x15,0xFF,0x3F,0x3F,0xFF,0xF0,0x80,0x06,0xFF
.db 0x00,0xFF,0x00,0xFF,0xF0,0x80,0x02,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF
.db 0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF
.db 0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x03,0xFF,0x0F,0xFF,0x3F,0x80
.db 0x18,0xFF,0xF0,0x80,0x06,0xFF,0x00,0xFF,0x00,0xFF,0xF0,0x80,0x02,0xFF,0x00,0xFF
.db 0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF
.db 0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF
.db 0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF
.db 0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0x80,0x3F,0xFF
.db 0x80,0x03,0xC0,0x80,0x03,0xE0,0x80,0x02,0xFF,0x80,0x04,0x7F,0xF0,0x01,0x00,0x00
.db 0x10,0x70,0x0C,0x0E,0x80,0x07,0xE0,0x80,0x02,0x00,0x30,0x30,0x78,0x7F,0x80,0x00
.db 0x80,0x02,0x00,0x18,0x18,0x38,0xFC,0xFC,0x00,0xF0,0x80,0x06,0xFF,0x00,0xFF,0x00
.db 0xFF,0xF0,0x80,0x02,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x03
.db 0xFF,0x0F,0xFF,0x3F,0x80,0x30,0xFF,0xF0,0x80,0x06,0xFF,0x00,0xFF,0x00,0xFF,0xF0
.db 0x80,0x02,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0x80,0x3F,0xFF,0x80,0x07,0xE0,0x80,0x02,0x7F,0x80,0x04,0x3F,0xF0,0xF3
.db 0xF7,0x80,0x04,0xFF,0xE0,0x00,0xE1,0xE3,0xE7,0x80,0x02,0xFF,0x80,0x00,0x00,0x3F
.db 0x7F,0xFF,0x80,0x02,0x00,0x01,0x01,0x07,0x80,0x04,0x03,0x80,0x03,0x00,0x80,0x02
.db 0x80,0xC0,0x80,0x08,0x00,0xF0,0x80,0x4E,0xFF,0xF0,0x80,0x06,0xFF,0x00,0xFF,0x00
.db 0xFF,0xF0,0x80,0x02,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00
.db 0xFF,0x00,0xFF,0x00,0x80,0x13,0xFF,0x07,0x1F,0x1F,0xFF,0x00,0x00,0x03,0xFF,0xFF
.db 0xF0,0x80,0x1B,0x00,0xE0,0xE0,0x80,0x03,0xFF,0x80,0x07,0xE0,0x80,0x07,0xC0,0x80
.db 0x07,0x00,0x80,0x00,0xC0,0xF8,0xF8,0x80,0x03,0xF9,0x80,0x04,0x00,0x80,0x02,0xC0
.db 0x80,0x04,0x03,0x80,0x02,0x06,0x80,0x07,0xC0,0x80,0x21,0x00,0x83,0x01,0x06,0x18
.db 0xE1,0x06,0x80,0x06,0x00,0xC3,0x80,0x38,0xFF,0xF0,0x80,0x06,0xFF,0x00,0xFF,0x00
.db 0xFF,0xF0,0x80,0x02,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x03
.db 0xFF,0x0F,0xFF,0x3F,0x80,0x0F,0xFF,0xC0,0x80,0x03,0xFF,0xE0,0xE0,0xFE,0x80,0x02
.db 0x00,0xE1,0xFF,0xFF,0xF8,0xFF,0x00,0x80,0x04,0xFF,0x7F,0xFF,0x4B,0x80,0x16,0xFF
.db 0x80,0x07,0xE0,0x80,0x07,0xC0,0x80,0x07,0x00,0x80,0x07,0x06,0x80,0x02,0xC0,0xE0
.db 0x80,0x03,0x60,0x80,0x07,0x06,0x80,0x07,0xC0,0x80,0x1D,0x00,0xE0,0xE0,0x00,0xC4
.db 0xC0,0x86,0x80,0x00,0x80,0x00,0x81,0xC3,0xC3,0x80,0x04,0x30,0xF0,0xF0,0x80,0x48
.db 0x00,0xF0,0x80,0x1D,0xFF,0x01,0x80,0x06,0x00,0xFF,0x1F,0x07,0x80,0x04,0x00,0xFF
.db 0xE0,0x80,0x15,0x00,0x01,0x01,0x1F,0x80,0x04,0xFF,0x80,0x04,0xE0,0x80,0x02,0xC0
.db 0x80,0x07,0x3F,0x80,0x07,0xFF,0x80,0x06,0x06,0x07,0x80,0x06,0x60,0xFF,0x80,0x06
.db 0x06,0xFF,0x80,0x07,0xC0,0x80,0x17,0x00,0x80,0x04,0x18,0x87,0x87,0xFF,0x1C,0x18
.db 0x08,0x08,0x80,0x02,0x0C,0x0E,0x80,0x83,0x00,0x80,0x02,0x01,0x80,0x04,0x00,0x80
.db 0x02,0xF0,0x7C,0x00,0x80,0x06,0x1F,0x80,0x02,0x00,0x80,0x02,0x01,0xFF,0xFF,0x80
.db 0x02,0x3F,0x80,0x03,0xFF,0xFC,0x80,0x04,0x00,0x80,0x02,0x07,0x80,0x07,0x00,0x07
.db 0x07,0x80,0x02,0x7F,0x80,0x02,0x78,0x80,0x06,0xFF,0x07,0x80,0x07,0xFF,0x80,0x04
.db 0xC0,0x80,0x1B,0x00,0x80,0x02,0x01,0x80,0x03,0x03,0x0E,0x0F,0x1F,0x1F,0x3F,0x7F
.db 0xFF,0x07,0x80,0x02,0x80,0x80,0x03,0xC0,0xE0,0x80,0x7F,0x00,0x83,0x80,0x00,0xF0
.db 0xF0,0x80,0x02,0xFC,0xFF,0xFC,0x3C,0x3C,0x80,0x03,0x00,0xFF,0x80,0x02,0x00,0xFC
.db 0xFC,0x80,0x02,0x00,0x80,0x02,0x03,0x80,0x04,0x00,0xF8,0xF8,0xF0,0x10,0x10,0x80
.db 0x02,0x00,0x80,0x03,0xFF,0x70,0x70,0x7E,0xFF,0x80,0x03,0x87,0x80,0x02,0x07,0x87
.db 0xF8,0xF8,0x80,0x05,0xFF,0x03,0x03,0x80,0x25,0xFF,0x03,0x03,0x07,0x06,0x80,0x03
.db 0x07,0xE0,0x80,0x00,0x80,0x05,0x00,0x60,0x34,0x3C,0x80,0x03,0x38,0xF8,0x80,0x8F
.db 0x00,0xFF,0x07,0x80,0x05,0x00,0xFF,0x7F,0x3F,0x3F,0x80,0x03,0x00,0x80,0x03,0xFF
.db 0x80,0x03,0x00,0x80,0x02,0xFC,0xE0,0x80,0x3B,0x00,0x07,0x07,0x80,0x05,0x00,0xFF
.db 0xFF,0x80,0x05,0x0F,0x80,0x07,0xF8,0x80,0xE7,0x00,0x01,0x01,0x80,0x05,0x00,0xFF
.db 0xFF,0x03,0x03,0x80,0x03,0x00,0xF8,0xFF,0xFF,0xF8,0x80,0x02,0x78,0x00,0x00,0x80
.db 0x05,0xC0,0x80,0x1A,0x00,0x87,0xFA,0xC2,0xBA,0x83,0x80,0x02,0xFF,0x1A,0xE9,0xEB
.db 0xEB,0x1B,0x80,0x02,0xFF,0x61,0xBE,0xF0,0xEE,0xE0,0x80,0x02,0xFF,0xA6,0x9B,0x80
.db 0x02,0xBB,0xFF,0xFF,0x7F,0x0C,0x7B,0x78,0x7B,0x9C,0xFF,0xFF,0xF7,0x76,0xAC,0x3E
.db 0xFE,0x7C,0x80,0x02,0xFF,0xE3,0xD9,0xD5,0xCD,0x63,0xFF,0x80,0xFF

; color data
; RLE WB compressed - Original size= 6144 - Final size= 1385
TILESET_col:
.db 0x80,0xFE,0x55,0x55,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80
.db 0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05
.db 0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55
.db 0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44
.db 0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44
.db 0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80
.db 0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x03,0x55,0x80,0x02,0x5F,0x44
.db 0x80,0x03,0x55,0x5F,0xE5,0xE4,0xE4,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44
.db 0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44
.db 0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80
.db 0x05,0x55,0x44,0x44,0x80,0x05,0x55,0x44,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80
.db 0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02
.db 0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55
.db 0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80
.db 0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02
.db 0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55
.db 0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80
.db 0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0xE5,0x44,0x44
.db 0x80,0x03,0x4E,0x00,0x50,0x80,0x06,0x04,0x50,0x80,0x02,0x04,0x80,0x04,0x05,0x80
.db 0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04
.db 0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44
.db 0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x04,0x55,0x80
.db 0x02,0x44,0x80,0x04,0x55,0x80,0x02,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03
.db 0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44
.db 0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80
.db 0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03
.db 0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44
.db 0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80
.db 0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03
.db 0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x02,0x44
.db 0xF4,0x80,0x03,0x55,0x80,0x02,0x44,0xF4,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03
.db 0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44
.db 0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80
.db 0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x03
.db 0x55,0x80,0x03,0x44,0x80,0x03,0x55,0x80,0x03,0x44,0x80,0x02,0x55,0x80,0x04,0x44
.db 0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80
.db 0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02
.db 0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44
.db 0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80
.db 0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02
.db 0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44
.db 0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80
.db 0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02
.db 0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44
.db 0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80
.db 0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x80,0x02
.db 0x55,0x80,0x04,0x44,0x80,0x02,0x55,0x80,0x04,0x44,0x55,0x55,0x80,0x05,0x44,0x55
.db 0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55
.db 0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80
.db 0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05
.db 0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44
.db 0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55
.db 0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55
.db 0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80
.db 0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05
.db 0x44,0x55,0xA5,0x80,0x05,0xA4,0xA5,0xA5,0x80,0x05,0xA4,0x55,0x55,0x80,0x05,0x44
.db 0x55,0x55,0x80,0x05,0x44,0x55,0x55,0x80,0x05,0x44,0x55,0x80,0x06,0x44,0x55,0x80
.db 0x06,0x44,0x55,0x80,0x06,0x44,0x55,0x80,0x04,0x44,0xC4,0xC4,0x55,0x80,0x03,0x44
.db 0xCC,0xCC,0x44,0x55,0x80,0x04,0x44,0xC4,0xC4,0xC5,0x80,0x06,0x4C,0xC5,0x80,0x06
.db 0x4C,0x80,0x04,0x5C,0x80,0x02,0xC4,0x55,0x44,0x80,0x05,0xC4,0x55,0x80,0x06,0x44
.db 0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44
.db 0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44
.db 0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44
.db 0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44,0x55,0x80,0x06,0x44,0x55,0x80,0x16,0xA4
.db 0xA5,0x80,0x06,0xA4,0x55,0x80,0x06,0x44,0x55,0x80,0x23,0x44,0x80,0xAB,0xC4,0x80
.db 0x47,0xA4,0x80,0x5F,0xC4,0x80,0xA7,0xA4,0x80,0x20,0xC4,0x6C,0x80,0x03,0x64,0x80
.db 0x29,0x4C,0x80,0xAD,0xA4,0x80,0x1F,0x4C,0x80,0x07,0x46,0x80,0x21,0xC4,0x80,0xC5
.db 0xA4,0x80,0x0F,0xC4,0x80,0x21,0x46,0x80,0xDD,0xA4,0x80,0x19,0x64,0x80,0xAD,0xA4
.db 0x80,0x37,0xBB,0x80,0x18,0x6B,0x80,0xEE,0xBA,0x80,0x20,0xB6,0x80,0xDE,0xBA,0x80
.db 0x10,0xB6,0x80,0x03,0x06,0x6B,0xCB,0xCB,0x80,0x0A,0x6B,0x80,0x03,0x0B,0xC0,0x80
.db 0x02,0xBB,0x80,0x05,0x0B,0x80,0xCE,0xBA,0x80,0x0F,0xB6,0x80,0x07,0xBC,0x6B,0xCC
.db 0x80,0x05,0x6B,0x80,0x04,0xC0,0x22,0xCC,0x22,0xB0,0xB0,0x0C,0x0C,0x6C,0x62,0x6C
.db 0x62,0x80,0x03,0xBB,0x80,0x0C,0x6B,0x80,0x92,0xBA,0x80,0x08,0x5B,0x80,0x02,0x75
.db 0xBB,0xBB,0x80,0x05,0x55,0xBB,0xBB,0x80,0x05,0x55,0xBB,0xBB,0x80,0x05,0x55,0xBB
.db 0xBB,0x80,0x05,0x5B,0xB6,0xB6,0xBA,0xBA,0x5A,0x5A,0x54,0x54,0x6B,0x6B,0x80,0x0D
.db 0xAB,0x80,0x07,0xB6,0xCC,0x22,0xCC,0x22,0x22,0x80,0x02,0x0B,0x6C,0x62,0x6C,0x62
.db 0x62,0x80,0x06,0x6B,0x80,0x25,0x0B,0xBC,0xBC,0x80,0x03,0x3C,0x80,0x06,0xBB,0x80
.db 0x39,0xB9,0x80,0x2E,0xBA,0x80,0x04,0x57,0x80,0x02,0xB5,0x77,0x55,0x55,0x80,0x24
.db 0x57,0x80,0x07,0x54,0x80,0x05,0x4B,0x80,0x09,0xAB,0x80,0x07,0x6B,0x80,0x07,0x0B
.db 0x80,0x07,0x6B,0x80,0x25,0x0B,0xBD,0xBD,0x00,0x90,0x80,0x02,0x9E,0xBE,0xDE,0xDE
.db 0xB9,0x80,0x4F,0xDB,0x80,0x1E,0xBA,0x80,0x10,0x5B,0x75,0x75,0x80,0x04,0xBB,0x80
.db 0x02,0x55,0x80,0x04,0xBB,0x80,0x02,0x55,0x80,0x04,0xBB,0x80,0x07,0xB5,0x80,0x17
.db 0xBA,0x6B,0x6B,0x80,0x0D,0xAB,0x6B,0x6B,0x80,0x25,0xAB,0x80,0x04,0xDB,0x80,0x02
.db 0xB9,0xED,0xED,0x80,0x05,0x4D,0x80,0x83,0xBB,0x80,0x6C,0xAB,0x80,0x06,0xDB,0x80
.db 0x06,0x4D,0xB4,0x80,0x05,0xDB,0x80,0x81,0x6B,0x80,0x67,0xBA,0xDB,0x80,0x03,0x6B
.db 0x80,0x02,0xAB,0x4B,0x80,0x05,0x6B,0xAA,0x80,0x03,0x6B,0x80,0xFE,0xAB,0x80,0xFE
.db 0xAB,0x80,0x27,0xAB,0x80,0x35,0xBF,0x80,0xFF

__endasm;
}