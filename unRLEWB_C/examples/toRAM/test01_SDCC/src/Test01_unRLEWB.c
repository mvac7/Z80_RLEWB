/* =============================================================================
   test GFX (MSX TILES devtool)
   v1.0 (27 jun 2014)
   Description:
    test output data from MSX TILES devtool
    & test unWBRLE2VRAM library (RLE WB to VRAM unpacker)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/VDP_TMS9918.h"
#include "../include/unRLEWBtoRAM.h"



#define  HALT __asm halt __endasm   //wait for the next interrupt







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




// Functions -------------------------------------------------------------------


//
void main(void)
{
    
  color(0,0,1);
  screen(2);
  
  unRLEWBtoRAM ((unsigned int) DATA_COL, 0xD000);

  copyToVRAM(0xD000,0x2000,0x800);
  
  while(1) //to infinity and beyond!
  {
    HALT;
  }

}




