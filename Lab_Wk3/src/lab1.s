/*******************************************************************************
File name       : lab1.s
Description     : Assembly language function to initialize an array.
Author          :     
Created         : 10/27/2014 By Jeff Bosch
Revision History
  10/26/2015 - Updated by Dave Allegre for the IAR compiler


*******************************************************************************/
  
  PUBLIC InitXa                 /* Make function name visible to linker */
  SECTION `.text`:CODE:NOROOT(1)
  THUMB

/*******************************************************************************
Function Name   : InitXa
Description     : Initializes an array passed using indexes
C Prototype     : void InitXa(int A[], int cnt, int val)
                : Where int A[] is the array to initialize
                : cnt is the size of A, and val is the value to initialize to
Parameters      : R0: Address of A
                : R1: cnt, number of elements
                : R2: val, value to set the elements to
Return value    : None
*******************************************************************************/
InitXa:
  MOV   R3,#+0                  ;; initalize the loop counter

InitXa_loop:
  STR   R2,[R0, R3, LSL #+2]    ;; val -> @(r0 [r3 * 4] on word boundary
  ADD   R3,R3,#+1               ;; Next element number
  CMP   R3,R1                   ;; check for more elements
  BNE     InitXa_loop             ;; repeat if more elements
  MOV   pc,lr                   ;; return
 
  END
