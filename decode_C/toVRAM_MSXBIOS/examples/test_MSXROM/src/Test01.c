/* =============================================================================
   test01
   v1.1 (09 April 2022)
   
   Description:
    test output data from MSX TILES devtool v1
    and test unRLEWBtoVRAM library (RLEWB to VRAM decompress)

  Historic of versions:
  - v1.1 (09 April 2022) update libraries
  - v1.0 (27 jun 2014) first version
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxBIOS.h"

#include "../include/VDP_TMS9918A_MSXROM.h"
#include "../include/unRLEWBtoVRAM.h"

#include "../include/GFXdata.h"


#define  HALT __asm halt __endasm   //wait for the next interrupt



//------------------------------------------------------ definition of functions
void WAIT(uint cicles);
char INKEY();
char Random();

void print0(char column, char line, char* text); //print in screen 0
void print(char column, char line, char* text);  //print in screen 1 or 2
void vprint(uint vaddr, char* text);

void test_decompress();



//-------------------------------------------------------------------- variables

char RNDSEED;


//-------------------------------------------------------------------- Functions


//
void main(void)
{
  SCREEN(1);
  COLOR(15,4,4);

  HALT;
  print(1, 9,"Test MSXTILESdevtool v1 and");
  print(1,10,"SDCC unRLEWBtoVRAM Lib");  
  
  WAIT(220);
     
  test_decompress();
  
  return;
}



/* =============================================================================
One character input (waiting)
============================================================================= */
char INKEY() __naked
{
__asm
  push IX   
  call CHGET
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



/* =============================================================================
 Random
 Description: Gives you a random number
 Input:       -
 Output:     [char]
============================================================================= */  
char Random() __naked
{
__asm

  ld   A,(_RNDSEED)
  
  SRA  A  

  add	 A,A
  add	 A,A
	
  ld   B,A	
  ld   A,R
  add  A,B

  inc  A 
      
  ld   (_RNDSEED),A
   
  ld   L,A
 
  ret  
__endasm;
}



//print in screen 0
void print0(char column, char line, char* text)
{
  uint vaddr = BASE0 + (line*40)+column; // calcula la posicion en la VRAM
  vprint(vaddr, text);
}



//print in screen 1 or 2
void print(char column, char line, char* text)
{
  uint vaddr = BASE10 + (line*32)+column; // calcula la posicion en la VRAM
  vprint(vaddr, text);
}



void vprint(uint vaddr, char* text)
{
  while(*(text)) VPOKE(vaddr++,*(text++));
}





// TEST functions ##############################################################
void test_decompress()
{
  char color;

  COLOR(0,0,1);
  SCREEN(2);
  
  unRLEWBtoVRAM((unsigned int) IMAGE_PAT, BASE12);
  unRLEWBtoVRAM((unsigned int) IMAGE_COL, BASE11);

  while(1) //to infinity and beyond!
  {
    //HALT;
    color = Random()>>4;
    COLOR(0,0,color);    
  }
   
}
//########################################################### END TEST functions


