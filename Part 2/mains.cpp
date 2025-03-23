/*
*
*
* ARRAYS
*
*
main(VOID)
{
	// type variable[# of items] = {array items};
	INT classes[ARRAY_SIZE] = {670};

	printf("index 0: %i, index 1: %i, index 2: %i\n", classes[0], classes[1], classes[2]);

	classes[1] = 660;
	printf("index 0: %i, index 1: %i, index 2: %i\n", classes[0], classes[1], classes[2]);

	return ERROR_SUCCESS;
}
*/

/*
*
*
* INTEGERS AND FLAGS
*
*
main(VOID)
{
	INT min = -1;
	UINT max = 4294967295;

	UINT nextClass = 770;

	// format for operators
	printf("is min == max? %s\n", (min == max) ? "YES" : "NO");
	printf("is max > min? %s\n", (max > min) ? "YES" : "NO");


	printf("is 770 a multiple of 16? %s\n", FlagOn(nextClass, MULTIPLE_OF_16) ? "YES" : "NO");
	printf("is 770 an odd number? %s\n", FlagOn(nextClass, ODD_NUMBER) ? "YES" : "NO");

	// test for flags on
	SetFlag(nextClass, ODD_NUMBER);
	printf("is 771 an odd number? %s\n", FlagOn(nextClass, ODD_NUMBER) ? "YES" : "NO");

	// tests for cleared flags
	ClearFlag(nextClass, ODD_NUMBER);
	printf("ClearFlag(nextClass, ODD_NUMBER) = %i (0x%08x)\n", nextClass, nextClass);

	// test for memory allocation by bit position
	ULONG memory = MEM_COMMIT | MEM_RESERVE;
	printf("is memory being committed? %s\n", FlagOn(memory, MEM_COMMIT) ? "YES" : "NO");

	return ERROR_SUCCESS;
}
*/