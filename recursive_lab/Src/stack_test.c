#include "stack_test.h"



void simpleRecursionTest(int level)
{
    PrintString("\nRecursion level = ");
    Print_uint32(level);
    PrintString("\n");
    level++;
    simpleRecursionTest(level);
}