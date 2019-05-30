#include "indexFileManagement.h"
IndexFileRegisterArray *createIndexFileRegisterArray( int size ){
    IndexFileRegisterArray *array = malloc(sizeof(IndexFileRegisterArray));
    
    if(array == NULL) return NULL;

    array->fileRegisters = malloc(sizeof(IndexFileRegister) * size);

    if(array ->fileRegisters == NULL) {
        free(array);
        return NULL;
    }

    array->actualSize = 0;
    array->totalSize = size;

    return array;
}
void reallocIndexFileRegisterArray(IndexFile *indexFile){
    IndexFileRegister *array = indexFile->indexArray.fileRegisters;
    array = realloc(array, indexFile->indexArray.actualSize * 5);
    indexFile->indexArray.totalSize *= 5;
}
void destroyIndexFileRegisterArray(IndexFile *indexFile){
    free(indexFile->indexArray.fileRegisters);
}
int readIndexFileRegisterFromFile(IndexFileRegister *indexFileRegister){
    
}