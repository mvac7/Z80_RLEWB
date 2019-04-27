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
void showPicture();








// Functions -------------------------------------------------------------------


//
void main(void)
{

  color(0,0,1);
  screen(2);
  
  showPicture();

  
  while(1) //to infinity and beyond!
  {
    HALT;
  }
  
   

}




void showPicture() __naked
{
__asm 

  
  ld   HL,#DATA_COL
  ld   DE,#BASE11
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