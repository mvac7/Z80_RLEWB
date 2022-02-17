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
void testRLEWB();


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
  print(4,10,"RLEWB to VRAM decompress"); 
  
  WAIT(220);
     
  testRLEWB();
  
  return;
}



/* =============================================================================
One character input (waiting)
============================================================================= */
byte inkey() __naked
{
__asm
  push IX   
  call _CHGET
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





// TEST functions ##############################################################
void testRLEWB()
{
  color(0,0,1);
  screen(2);
//  setSpritesSize(0);
//  setSpritesZoom(true);

  
  unRLEWBtoVRAM ((unsigned int) IMAGE_PAT, BASE12);
  unRLEWBtoVRAM ((unsigned int) IMAGE_COL, BASE11);

  
  while(1) //to infinity and beyond!
  {
    HALT;
  }
   
}
//########################################################### END TEST functions


