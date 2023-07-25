/* =============================================================================
  test RLEWB
  
  Version: 1.0 (23/04/2019)
  Architecture: MSX
  Format: .COM (MSXDOS)
  Programming language: C
   
  Description:
    test output data from MSX TILES devtool or tMSgfX
    & test unRLEWB2VRAM library (RLEWB to VRAM decompress)
    
  History of versions:
  - v1.0 (23/04/2019)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVars.h"
#include "../include/msxBIOS.h"
#include "../include/msxDOS.h"

#include "../include/VDP_TMS9918A.h"
#include "../include/unRLEWBtoVRAM.h"

#include "../include/GFXdata.h"



#define  HALT __asm halt __endasm   //wait for the next interrupt




// MSX-DOS Calls ---------------------------------------------------------------
#define  SYSTEM 0x0005  // MSX-DOS entry





// Function Declarations -------------------------------------------------------
void System(char code);

char PEEK(uint address);
void POKE(uint address, char value);

char INKEY();

void WAIT(uint cicles);

void testRLEWB();


// global variable definition --------------------------------------------------



// Functions -------------------------------------------------------------------


//
void main(void)
{
  char colorInk=0;
  char colorBG=0;
  char colorBDR=0;
  char scrcolumns=0;
  //char MSXsion=0;
  
  colorInk=PEEK(FORCLR);
  colorBG=PEEK(BAKCLR);
  colorBDR=PEEK(BDRCLR);
  scrcolumns=PEEK(LINLEN);
  
  
  testRLEWB();

//EXIT MSXDOS ------------------------------------------------------------------
  //put the screen as it was.
  COLOR(colorInk,colorBG,colorBDR);
  //WIDTH(scrcolumns);

  if(scrcolumns<33) SCREEN(1);
  else SCREEN(0);
  //
    
  System(_TERM0); 
//--------------------------------------------------------------------- end EXIT

}



// call system functions 
// see MSX Assembly Page > MSX-DOS 2 function calls
// http://map.grauw.nl/resources/dos2_functioncalls.php
void System(char code) __naked
{
code;
__asm
	push IX
	ld   IX,#0
	add  IX,SP

	ld   C,4(IX)
	call SYSTEM

	pop  IX
    ret
__endasm; 
}



char PEEK(uint address) __naked
{
address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   L,4(IX)
  ld   H,5(IX)
  ld   L,(HL)

  pop  IX
  ret
__endasm;
}



void POKE(uint address, char value) __naked
{
address;value;
__asm
  push IX
  ld   IX,#0
  add  IX,SP 
    
  ld   L,4(IX)
  ld   H,5(IX)
  ld   A,6(IX)
  ld   (HL),A

  pop  IX
  ret  
__endasm;
}







/* =============================================================================
One character input (waiting)
============================================================================= */
char INKEY() __naked
{
__asm   
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   IX,#CHGET
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
  
  ld   L,A
  pop  IX
  ret
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




// TEST functions ##############################################################
void testRLEWB()
{
  COLOR(0,0,1);
  SCREEN(2);
//  setSpritesSize(0);
//  setSpritesZoom(true);
  
  unRLEWBtoVRAM ((unsigned int) IMAGE_PAT, BASE12);
  unRLEWBtoVRAM ((unsigned int) IMAGE_COL, BASE11);
  
  INKEY();
   
}
//########################################################### END TEST functions
