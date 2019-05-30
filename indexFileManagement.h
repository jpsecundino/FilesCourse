#ifndef INDEX_FILE_MANAGEMENT
#define INDEX_FILE_MANAGEMENT
#include<stdio.h>
#include "fileManagement.h"
#define SIZE_CHAVE_BUSCA 120
#define SIZE_NRO_REG 4
#define NUM_REG_OFFSET 1

typedef struct _indexFileHeader{
    char status;
    int nroRegistros;
}IndexHeader;

typedef struct _indexRegister{
    char chaveBusca[SIZE_CHAVE_BUSCA];
    long long int byteOffset;
    char removed;
}IndexRegister;

typedef struct _indexRegisterArray{
    IndexRegister *fileRegisters;
    int actualSize;
    int totalSize;
}IndexRegisterArray;

typedef struct _indexFileRegister{
   IndexHeader fileHeader;
   IndexRegisterArray *indexArray;
   FILE *filePointer;
}IndexFileRegister;

IndexRegisterArray *createIndexRegisterArray( int size );
void reallocIndexRegisterArray(IndexFileRegister *indexFile);
void destroyIndexRegisterArray(IndexFileRegister *indexFile);
IndexHeader readIndexHeaderFromIndexFile(FILE *indexFile);
void writeIndexHeader(IndexFileRegister *indexFile);
int readIndexRegisterFromIndexFile(FILE *file, IndexRegister *indexRegister);
int readIndexesFromIndexFile(IndexFileRegister * indexFile);
void writeIndexFile(IndexFileRegister * indexFile);
int writeIndexRegisterInIndexFile(FILE *indexFile, IndexRegister ir);
void makeIndexFromEmployeeFile(FILE *employeeFile, IndexFileRegister *indexFile);
int compareIndexRegisters(const void *a, const void *b);
void sortIndexes(IndexFileRegister *indexFile);
int initIndexFile(FILE *file, IndexFileRegister *indexFile);
int changeIndexFileStatus(IndexFileRegister *indexFileRegister, char newStatus);
void setNumberOfRegisters(IndexFileRegister *indexFileRegister);
int readIndexFile(IndexFileRegister *indexFileRegister);
int indexBinarySearch(IndexRegister *array, int beg, int size, char *key);
int isIndexConsistent(IndexHeader *header );
int findRepeatedNamesBlockBegin(IndexRegister *array,int initialPos, char *employeeName);
void removeFromIndexFile(IndexFileRegister *indexFileRegister, char *employeeName );
void changeIndexFileNumReg(IndexFileRegister *indexFileRegister, int numReg);
#endif