/*
File made to simplify the main function visualization

@Authors: João Pedro Almeida Santos Secundino 10692054
		  Giovani Decico Lucafó 10288779
*/
#include <stdio.h>
#include <stdlib.h>
#include "fileManagement.h"
#include "indexFileManagement.h"
#ifndef IO_PROG_TRAB2
#define IO_PROG_TRAB2
#define MERGE 0
#define MATCH 1
#define PRINT 1
#define NOT_PRINT 0

/**
Reads a specific register field from stdin
@Arguments:
    - fieldName: string cointaining the name of the field to be read
    - o: union that will receive the content read by stdin
@Return:
    none
*/
int readSpecificField(char *fieldName, Options *o);

/**
this function was made to encapsulate the program's first functionality
It reads the csv file, builds the Header page, and creates the binary file acording to what it read
@Arguments:
	- csv: pointer to a CSV file
	- binFile: pointer to an output binary file
@Return:
	Number of pages read
*/
int firstFunctionality(FILE *csv, FILE *binFile);

/**
This function was made to encapsulate the program's second functionality
@Arguments:
	- binFile - pointer to a binary file
@Return:
	none
*/
void secondFuncionality( FILE *binFile );

/**
This function was made to encapsulate the program's third functionality
@Arguments:
	- binFile: pointer to a binary file
	- o: union containing the specified field content
	- option: indicates the required field to be searched
@Return:
	if an error occurred, -1
	if the remployee doesn't exists, -2
	otherwise, the number of pages accessed
*/
int thirdFunctionality(FILE *binFile, Options o, int option);

/**
This function was made to encapsulate the program's fourth functionality
@Arguments:
	- fileRegister: pointer to a fileRegister
	- removedList: pointer to a list containing all the removed register from the file
    - data: array containing all queries to be made
    - dataToRemoveSize: size of data (number of queries)
@Return:
	none
*/
void fourthFunctionality(FileRegister *fileRegister, IndexFileRegister *indexFileRegister,LIST *removedList, DataToRemove *data,int dataToRemoveSize);

/**
This function was made to encapsulate the program's fifth functionality
@Arguments:
	- fileRegister: pointer to a fileRegister
    - removedList: pointer to a list containing all the removed register from the file
	- pointer to a index file register if it exists
@Return:
	none
*/
void fifthFunctionality(FileRegister *fileRegister, IndexFileRegister *IndexFileRegister,LIST *removedList);

/**
This function was made to encapsulate the program's sixth functionality
@Arguments:
	- fileRegister: pointer to a fileRegister
    - removedList: pointer to a list containing all the removed register from the file
@Return:
	none
*/
void sixthFunctionality(FileRegister *fileRegister, LIST *removedList);

/**
This function was made to encapsulate the program's eighth functionality: merge registers in two files by its ids
@Arguments:
	- firstFileRegister: pointer to the first file to be merged
	- secondFileRegister: pointer to the second file to be merged
	- outFile: pointer to a new created file that will contain the result of the two merged files
@Return:
	none
*/
void eighthFunctionality(FileRegister *firstFileRegister, FileRegister *secondFileRegister, FileRegister *outFileRegister);

/**
This function was made to encapsulate the program's ninth functionality: match registers in the two files by its ids
@Arguments:
	- firstFileRegister: pointer to the first file to be matched
	- secondFileRegister: pointer to the second file to be matched
	- outFile: pointer to a new created file that will contain the result of the two matched files
@Return:
	none
*/
void ninthFunctionality(FileRegister *firstFileRegister, FileRegister *secondFileRegister, FileRegister *outFileRegister);


/**
This function was made to prepare and clean all the needs of first functionality
@Arguments:
	- csv: pointer to a csv file
@Return:
	none
*/
void setUnsetFunc1(FILE *csv);

/**
This function was made to prepare and clean all the needs of third functionality
@Arguments:
	- binFile: pointer to a bin file
@Return:
	none
*/
void setUnsetFunc3(FILE *binFile);


/**
This function was made to prepare and clean all the needs of fourth functionality
@Arguments:
	- binFile: pointer to a bin file
	- indexFileRegister: pointer to an index file register if it exists (can be NULL)
	- printHexFile: set PRINT if you want to print binFile HEX or NOT_PRINT othwerwise
@Return:
	1 if success
	-1 otherwise
*/
int setUnsetFunc4(FILE * binFile, IndexFileRegister *indexFileRegister, int printHexFile);

/**
This function was made to prepare and clean all the needs of fifth functionality
@Arguments:
	- binFile: pointer to a bin file
	- indexFileRegister: pointer to an index file register if it exists (can be NULL)
	- printHexFile: set PRINT if you want to print binFile HEX or NOT_PRINT othwerwise
@Return:
	1 if success
	-1 otherwise
*/
int setUnsetFunc5(FILE *binFile, IndexFileRegister *indexFileRegister, int printHexFile);

/**
This function was made to prepare and clean all the needs of sixth functionality
@Arguments:
	- binFile: pointer to a bin file
@Return:
	none
*/
void setUnsetFunc6(FILE *binFile);

/**
This function was made to prepare and clean all the needs of seventh functionality
@Arguments:
	- binFile: pointer to a bin file
@Return:
	none
*/
void setUnsetFunc7(FILE *binFile);

/**
This function was made to prepare and clean all the needs of eighth and ninth functionalities
@Arguments:
	- binFile: pointer to a bin file
	- mode: indicates what functionality to use
		MERGE: will merge the files(will cal the eight functionality)
		MATCH: will match the registers in file (will call the ninth functionality)
@Return:
	none
*/
void setUnsetFuncs8And9(FILE *firstBinFile, const int mode);

void setUnsetFunc10(FILE *binFile);

int setUnsetFunc11(FILE *binFile);

void setUnsetFunc12(FILE *binFile);

void setUnsetFunc13(FILE *binFile);

void setUnsetFunc14(FILE *binFile);

void tenthFunctionality(FileRegister *inFileRegister, IndexFileRegister *indexFileRegister);

int eleventhFunctionality(FileRegister *indexFileRegister, IndexFileRegister *IndexFileRegister, char *employeeName, FILE* outStream);

#endif 
