// main.c : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <stdio.h>
//
// TODO #1: create the main function for the program
// this includes, the following:

// function return type
// function name
// function parameters, if any (not needed for this version)
// function body
INT main()
{
	INT hexOne = 0x7FFFFFFFF;
	UINT hexTwo = 0x1;
	DWORD pemdas = 1 << 12;

	printf("INT hexOne (hex) [%x] (dec) [%d] \n", hexOne);
	printf("UINT hexTwo (hex) [%x] (dec) [%d] \n", hexTwo);
	printf("DWORD pemdas (hex) [%x] (dec) [%d] \n", pemdas);

	printf("INT hexOne + UINT hexTwo (hex) [%x] (signed) [%d] (unsigned) [%u]\n", hexOne += hexTwo);

	ULONGLONG beef = 0xDeadBeefCafeBabe;
	
	// acceptable ipv4 representations
	INT ip = 2130706433;
	ULONG localhost = 0x7F000001;

	printf("beef %llu\n", beef);
	printf("integer %d\n", ip);
	printf("hex %ul\n", localhost);

	INT mix = 0x7FFFFFFF;
	UINT match = 1U;
	mix += match;
	match -= 2;

	printf("mix:	(hex) [0x%x] (signed) [%i]	(unsigned) [%u]\n", mix, mix, mix);
	printf("match:	(hex) [0x%x] (signed) [%i]		(unsigned) [%u]\n", match, match, match);

	return ERROR_SUCCESS;
}
//
// TODO #2: inside main's function body
//
// create 3 variables (name the variables whatever you want) of the following types and
// intialize them with their respective values:
//
//INT 0x7FFFFFFF;
//UINT 0x1;
//DWORD 1 << 12;
//
// complete the following printf statements with their proper format string argument types
//
// your argument type should be placed in the []
//
// printf("INT var (hex)[%x] (dec)[%d] \n", );
// printf("UINT var (hex)[%x] (dec)[%d] \n", );
// printf("DWORD var (hex)[%x] (dec)[%d] \n", );
//
// TODO #3: inside main's function body
//
// add and update your INT variable with your UINT variable
//
// fill in the following printf statement with the newly updated INT variable
//
// printf("INT var + UINT var (hex)[%x] (signed)[%d] (unsigned)[%u] \n", );
//
// TODO #4: inside main's function body
//
// for this part, you must choose the proper data type to hold the value
//
// create 3 variables to hold the following values:
//
//0xDeadBeefCafeBabe;
//2130706433;
//0x7F000001;