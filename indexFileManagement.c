#include "indexFileManagement.h"

int initIndexFile(FILE *indexFile, IndexFileRegister *indexFileRegister){
    IndexHeader h = {'0', 0};
    indexFileRegister->fileHeader = h;
    indexFileRegister->filePointer = indexFile;
    indexFileRegister->indexArray = createIndexRegisterArray(MAX_BUFFER_SIZE);

    if(indexFileRegister->indexArray == NULL ){
        return 0;
    }

    return 1;
}

IndexRegisterArray *createIndexRegisterArray( int size ){
    IndexRegisterArray *array = malloc(sizeof(IndexRegisterArray));
    
    if(array == NULL) return NULL;

    array->fileRegisters = malloc(sizeof(IndexRegister) * size);

    if(array ->fileRegisters == NULL) {
        free(array);
        return NULL;
    }

    array->actualSize = 0;
    array->totalSize = size;

    return array;
}

void reallocIndexRegisterArray(IndexFileRegister *indexFileRegister){
    IndexRegister *array = indexFileRegister->indexArray->fileRegisters;
    array = realloc(array, indexFileRegister->indexArray->actualSize * 5 * sizeof(IndexRegister));
    indexFileRegister->indexArray->fileRegisters = array;
    indexFileRegister->indexArray->totalSize *= 5;
}

void destroyIndexRegisterArray(IndexFileRegister *indexFileRegister){
    free(indexFileRegister->indexArray->fileRegisters);
    free(indexFileRegister->indexArray);
}

IndexHeader readIndexHeaderFromIndexFile(FILE *indexFile){
    IndexHeader h;
    fseek(indexFile, 0, SEEK_SET);
    fread(&h.status, sizeof(char), 1, indexFile);
    fread(&h.nroRegistros, sizeof(int), 1, indexFile);
    return h;
}

int readIndexRegisterFromIndexFile(FILE *file, IndexRegister *indexRegister){
    int tagSuccesfullRead =  0;
    
    tagSuccesfullRead += fread(&indexRegister->chaveBusca, sizeof(char), SIZE_CHAVE_BUSCA, file);
    tagSuccesfullRead += fread(&indexRegister->byteOffset, sizeof(long long int), 1, file);

    return tagSuccesfullRead;
}

//the index already exists in file not in ram
int readIndexesFromIndexFile(IndexFileRegister * indexFileRegister){
    IndexRegister ir;
    int arrayIt;
    FILE *file = indexFileRegister->filePointer;
    
    fseek(file, INDEX_PAGE_SIZE, SEEK_SET );
    while(!feof(file)){
        if(readIndexRegisterFromIndexFile(file, &ir) ){
            
            if(indexFileRegister->indexArray->actualSize == indexFileRegister->indexArray->totalSize){
                reallocIndexRegisterArray(indexFileRegister);
            }


            arrayIt = indexFileRegister->indexArray->actualSize;

            indexFileRegister->indexArray->fileRegisters[arrayIt] = ir;
            indexFileRegister->indexArray->fileRegisters[arrayIt].removed = '-';
            indexFileRegister->indexArray->actualSize++;
        }
    }

    return myCeil((double)ftell(file)/(double)INDEX_PAGE_SIZE) -1;
}

int readIndexFile(IndexFileRegister *indexFileRegister){
    indexFileRegister->fileHeader = readIndexHeaderFromIndexFile(indexFileRegister->filePointer);

    if(isIndexConsistent(&indexFileRegister->fileHeader) == 0){
        return -1;
    }

    return readIndexesFromIndexFile(indexFileRegister) + 1;
}

//the index only exists in RAM
void writeIndexFile(IndexFileRegister * indexFileRegister){
    IndexRegister ir;
    FILE *file = indexFileRegister->filePointer;
    writeIndexHeader(indexFileRegister);
    int numReg = 0;
    for (int i = 0; i < indexFileRegister->indexArray->actualSize; i++){
        ir = indexFileRegister->indexArray->fileRegisters[i];
        if(ir.removed != '*'){
            writeIndexRegisterInIndexFile(file, ir);
            numReg++;
        }
    }
    changeIndexFileNumReg(indexFileRegister, numReg);
}

int writeIndexRegisterInIndexFile(FILE *indexFile, IndexRegister ir){
    int flagSuccesful = 1;
    flagSuccesful &= fwrite(&ir.chaveBusca, sizeof(char), SIZE_CHAVE_BUSCA, indexFile );
    flagSuccesful &= fwrite(&ir.byteOffset, sizeof(long long int), 1, indexFile );
    return flagSuccesful;
}

void writeIndexHeader(IndexFileRegister *indexFileRegister){
    fseek(indexFileRegister->filePointer, 0, SEEK_SET);
    fwrite(&indexFileRegister->fileHeader.status, sizeof(char), 1, indexFileRegister->filePointer);
    fwrite(&indexFileRegister->fileHeader.nroRegistros, sizeof(int), 1, indexFileRegister->filePointer);
    putCharOnPage(indexFileRegister->filePointer, '@', INDEX_PAGE_SIZE - SIZE_NRO_REG - STATUS_TAG_SIZE);
}

//the index doesnt exist yet in any place
void makeIndexFromEmployeeFile(FILE *employeeFile, IndexFileRegister *indexFileRegister){
    EmployeeRegister *e = createEmployeeRegister();
    long long int employeeByteOffset;

    fseek(employeeFile, INDEX_PAGE_SIZE, SEEK_SET);
    while(!feof(employeeFile)){
        employeeByteOffset = ftell(employeeFile);
        if(readPerson(employeeFile,e)){
            if(e->indicadorTamanhoNome != 0 && e->removido != '*'){
                addEmployeeInIndexArrayEnd(indexFileRegister, e->nomeServidor, employeeByteOffset);
            }
        }
    }
    
    setNumberOfRegisters(indexFileRegister);

    sortIndexes(indexFileRegister);

    destroyEmployeeRegister(e);
    
}

int compareIndexRegisters(const void *a, const void *b){
    char *aString = (char *)a;
    char *bString = (char *)b;

    return strcmp(aString,bString) <= 0 ? -1 : 1;
}

void sortIndexes(IndexFileRegister *indexFileRegister){
    IndexRegister *array = indexFileRegister->indexArray->fileRegisters;
    int arraySize = indexFileRegister->indexArray->actualSize;
    qsort(array, arraySize, sizeof(IndexRegister), compareIndexRegisters );
}

int changeIndexFileStatus(IndexFileRegister *indexFileRegister, char newStatus){
    int flagSuccesful = 1;

    fseek(indexFileRegister->filePointer, 0 , SEEK_SET);
    flagSuccesful &= fwrite(&newStatus, sizeof(char), 1, indexFileRegister->filePointer);

    return flagSuccesful;
}

void setNumberOfRegisters(IndexFileRegister *indexFileRegister){
    indexFileRegister->fileHeader.nroRegistros = indexFileRegister->indexArray->actualSize;
}

int indexBinarySearch(IndexRegister *array, int inf, int sup , char *key){
    if(inf > sup) return -1;

    int mid = ( inf + sup ) /2;

    int res = strcmp(array[mid].chaveBusca, key);
    
    if( res == 0 ) {
        return mid;   
    }else if( res < 0 ){
        return indexBinarySearch(array, mid + 1, sup, key);
    }else{
        return indexBinarySearch(array, inf, mid - 1, key);
    }
}

int isIndexConsistent(IndexHeader *header ){
    return header->status == '1' ? 1 : 0; 
}

int findRepeatedNamesBlockBegin(IndexRegister *array,int initialPos, char *employeeName){
    
    //find the beginning of repeated names block
	while(initialPos > 0 && strcmp(array[initialPos - 1].chaveBusca, employeeName) == 0 ){
		initialPos --;
	}

    return initialPos;
}

void removeFromIndexFile(IndexFileRegister *indexFileRegister, char *employeeName ){
    IndexRegister *array = indexFileRegister->indexArray->fileRegisters;
    int arraySize = indexFileRegister->indexArray->actualSize;
    int pos = indexBinarySearch(array, 0, arraySize, employeeName);
    
    if(pos != -1){
        pos = findRepeatedNamesBlockBegin(array, pos, employeeName);
    }

    while(pos < arraySize && strcmp(array[pos].chaveBusca, employeeName) == 0){
        array[pos].removed = '*';			
        pos++;					
    }
}

void changeIndexFileNumReg(IndexFileRegister *indexFileRegister, int numReg){
    fseek(indexFileRegister->filePointer, NUM_REG_OFFSET, SEEK_SET );
    fwrite(&numReg, sizeof(int), 1, indexFileRegister->filePointer);
}

void addEmployeeInIndexArrayEnd(IndexFileRegister *indexFileRegister, char *employeeName, long long int employeeByteOffset){
    int arrayIt = 0,
    bufferSize = 0;
    char auxBuffer[MAX_BUFFER_SIZE] = {};
    
    //preparing the key
    strcpy(auxBuffer, employeeName);
    bufferSize = strlen(auxBuffer) + END_STRING_TOKEN;
    //filling it with '@'
    fillWithChar(auxBuffer + bufferSize, '@', SIZE_CHAVE_BUSCA - bufferSize);

    if(indexFileRegister->indexArray->actualSize == indexFileRegister->indexArray->totalSize){
        reallocIndexRegisterArray(indexFileRegister);
    }
    
    arrayIt = indexFileRegister->indexArray->actualSize;
    indexFileRegister->indexArray->fileRegisters[arrayIt].byteOffset = employeeByteOffset;
    indexFileRegister->indexArray->fileRegisters[arrayIt].removed = '-';
    memcpy(indexFileRegister->indexArray->fileRegisters[arrayIt].chaveBusca, auxBuffer, SIZE_CHAVE_BUSCA*sizeof(char));
    
    indexFileRegister->indexArray->actualSize++;
}
