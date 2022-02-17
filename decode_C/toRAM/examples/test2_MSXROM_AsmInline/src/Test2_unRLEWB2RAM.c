/* =============================================================================
   test GFX (MSX TILES devtool)
   v1.0 (27 jun 2014)
   Description:
    test output data from MSX TILES devtool
    & test unWBRLE2RAM library (RLEWB to RAM decompress)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/VDP_TMS9918.h"
#include "../include/unRLEWBtoRAM.h"
#include "../include/GFXdata.h"


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
  push IX
  
  ;decompress to RAM
  ld   HL,#_IMAGE_PAT
  ld   DE,#0xD000 
  call unRLEWBRAM
  
  ;copy to VRAM pattern table
  ld   HL,#0xD000 ;RAM addr
  ld   DE,#BASE12 ;VRAM addr
  ld   BC,#0x1800 ;length
  call 0x005C     ;LDIRVM

  ;decompress to RAM
  ld   HL,#_IMAGE_COL
  ld   DE,#0xD000 
  call unRLEWBRAM
  
  ;copy to VRAM color table
  ld   HL,#0xD000 ;RAM addr
  ld   DE,#BASE11 ;VRAM addr
  ld   BC,#0x1800 ;length
  call 0x005C     ;LDIRVM
  
  pop  IX
  ret
__endasm;
}