#ifndef INDEX_FILE_MANAGEMENT
#define INDEX_FILE_MANAGEMENT
#include<stdio.h>
#define SIZE_CHAVE_BUSCA 120

typedef struct _indexFileHeader{
    char status;
    int nroRegistros;
}IndexFileHeader;

typedef struct _indexFileRegister{
    char chaveBusca[SIZE_CHAVE_BUSCA];
    long long int byteOffset;
}IndexFileRegister;

typedef struct _indexFileRegisterArray{
    IndexFileRegister *fileRegisters;
    int actualSize;
    int totalSize;
}IndexFileRegisterArray;

typedef struct _indexFile{
   IndexFileHeader fileHeader;
   IndexFileRegisterArray indexArray;
   FILE *indexFile;
}IndexFile;

IndexFileRegisterArray *createIndexFileRegisterArray( int size );
void reallocIndexFileRegisterArray(IndexFileRegisterArray *array);
void destroyIndexFileRegisterArray(IndexFile *indexFile);
int readIndexFileRegisterFromFile(IndexFileRegister *indexFileRegister);

#endif