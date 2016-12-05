/*******************************************************************************
File name       : bsort.s
Description     : Assembly language function to bubble sort an array.
Author          : Patrick Laban
Created         : 11/2/2016


*******************************************************************************/

  MODULE        BUBBLESORT    /* The module name */

  PUBLIC BubbleSort         /* Make function name visible to linker           */

  /* Tells the linker the section name : memory type : fragment (align)       */
  SECTION `.text`:CODE:NOROOT(1) 
  THUMB                 /* Mode control directive                             */

/*******************************************************************************
Function Name   : BubbleSort
Description     : Sorts an array passed using a bubble sort
C Prototype     : void BubbleSort(int A[], int cnt)
                : Where int A[] is the array to sort
                : cnt is the size of A
Arguments       : R0: Address of A
                : R1: cnt, number of elements
                : R2: sort order, 0 for s->b, 1 for b->s
Variables       : R3: Last address
                : R4: swap occured flag
                : R5: Indexed address for operation
                : R6: Current element value
                : R7: Next element value
                
Return value    : None
*******************************************************************************/
BubbleSort:
  STMFD SP!,{R3-R12,LR}         ; Save current context
  CMP   R1,#2                   ; Ensure there are at least two items
  BLT   BubbleSort_exit         ; If not goto exit
  MOV   R4,#0                   ; set swap flag to 0 (False)
  MOV   R5,R0                   ; Start at the begining of the array
  ADD   R3,R0,R1,LSL #2         ; Set R3 to the end of the array
  SUB   R3,R3,#4                ; Subtract one index to be at actual end

BubbleSort_loop:
  CMP   R5,R3                   ; Check to see if we are at the last element
  BEQ   BubbleSort_fullsortcheck; If so check for full sort
  LDR   R6,[R5],#4              ; Load value of current addr, then inc R4
  LDR   R7,[R5]                 ; Load value of next address into R6
  CMP   R2,#0                   ; Check which sort to use
  BEQ   BubbleSort_lt           ; Use small to big (LT)
  B     BubbleSort_gt           ; Use big to small (GT)
  

BubbleSort_lt:
  CMP   R7,R6                   ; Compare the values for sorting
  ITTT  LT                      ; If next elem is less then current
  STRLT R6,[R5]                 ; Store the swapped values
  STRLT R7,[R5,#-4]
  MOVLT R4,#1                   ; Set swap flag to 1 (True)
  B     BubbleSort_loop         ; Repeat loop

BubbleSort_gt:
  CMP   R7,R6                   ; Compare the values for sorting
  ITTT  GT                      ; If next elem is less then current
  STRGT R6,[R5]                 ; Store the swapped values
  STRGT R7,[R5,#-4]
  MOVGT R4,#1                   ; Set swap flag to 1 (True)
  B     BubbleSort_loop         ; Repeat loop

BubbleSort_fullsortcheck:
  CMP   R4,#0                   ; See if we did any swaps
  BEQ   BubbleSort_exit         ; If no swaps then sorted, exit
  MOV   R5,R0                   ; reset the current address to start of array
  MOV   R4,#0                   ; reset swap flag
  B     BubbleSort_loop         ; loop through again

BubbleSort_exit:
  LDMFD SP!,{R3-R12,LR}         ; Restore context and update PC with LR
  END