/* =============================================================================
   test1
   v1.1 (11 April 2022)
   
   Description:
    test output data from MSX TILES devtool
    & test unRLEWBtoRAM library (RLEWB to RAM decompress)

  Historic of versions:
  - v1.1 (11 April 2022) update libraries
  - v1.0 (27 June 2014) first version
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxBIOS.h"

#include "../include/VDP_TMS9918A_MSXROM.h"
#include "../include/unRLEWBtoRAM.h"

#include "../include/GFXdata.h"


#define  HALT __asm halt __endasm   //wait for the next interrupt




// Functions -------------------------------------------------------------------


//
void main(void)
{
    
  COLOR(0,0,4);
  SCREEN(2);
  
  unRLEWBtoRAM((unsigned int) IMAGE_PAT, 0xD000);
  CopyToVRAM(0xD000,BASE12,0x1800);
  
  unRLEWBtoRAM((unsigned int) IMAGE_COL, 0xD000);
  CopyToVRAM(0xD000,BASE11,0x1800);
  
  while(1) //to infinity and beyond!
  {
    HALT;
  }

}




