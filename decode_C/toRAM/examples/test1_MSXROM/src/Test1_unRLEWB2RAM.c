/* =============================================================================
   test GFX (MSX TILES devtool)
   v1.0 (27 jun 2014)
   Description:
    test output data from MSX TILES devtool
    & test unWBRLE2RAM library (RLEWB to RAM uncompress)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/VDP_TMS9918.h"
#include "../include/unRLEWBtoRAM.h"
#include "../include/GFXdata.h"


#define  HALT __asm halt __endasm   //wait for the next interrupt




// Functions -------------------------------------------------------------------


//
void main(void)
{
    
  color(0,0,1);
  screen(2);
  
  unRLEWBtoRAM ((unsigned int) IMAGE_PAT, 0xD000);
  copyToVRAM(0xD000,BASE12,0x1800);
  
  unRLEWBtoRAM ((unsigned int) IMAGE_COL, 0xD000);
  copyToVRAM(0xD000,BASE11,0x1800);
  
  while(1) //to infinity and beyond!
  {
    HALT;
  }

}




