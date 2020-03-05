#include <stdio.h>
#include <stdlib.h>

#define ASCII_OFFSET 48

// https://stackoverflow.com/questions/3911400/how-to-pass-2d-array-matrix-in-a-function-in-c
// https://www.geeksforgeeks.org/pass-2d-array-parameter-c/

void initialize(unsigned int rows, unsigned int columns, char matrix[rows][columns]){
	int i,j,count = 1;
	for(i = 0; i < rows; i++){
		for(j = 0; j < columns; j++, count++){
			matrix[i][j] = count + ASCII_OFFSET;
		}
	}
}

void printMatrix(unsigned int rows, unsigned int columns, char matrix[rows][columns]){
	printf("-> This is the matrix\n");
	int i,j;
	for(i = 0; i < rows; i++){
		for(j = 0; j < columns; j++){
			printf("%c\t", matrix[i][j]);
		}
		printf("\n");
	}
}

void iterateAsArray(unsigned int lenght, char matrix[lenght]){
	printf("-> This is the array\n");
	int i;
	for(i = 0; i < lenght; i++){
		printf("%c\t",matrix[i]);
	}
	printf("\n");
}

int main(){
	
	// First with a C matrix
	
	unsigned int rows = 3, columns = 2;
	char matrix[3][2];
	
	initialize(rows,columns,matrix);
	
	printMatrix(rows,columns,matrix);
	
	iterateAsArray(rows*columns,matrix);
	
	// Now with dynamically allocated memory
	
	char** arrayDin = (char**) malloc(sizeof(char)*rows*columns);
	
	initialize(rows,columns,arrayDin);
	
	printMatrix(rows,columns,arrayDin);
	
	iterateAsArray(rows*columns,arrayDin);
	
	// Now with used implementation (array of arrays)
	
	char** result = (char**) malloc(sizeof(char *) * rows + sizeof(char) * columns * rows);
    char* ptr = (char*)(result + rows);
    int i;
    for(i = 0; i < rows; i++){
        result[i] = (ptr + columns * i); 
    }
    
    initialize(rows,columns,result);
	
	printMatrix(rows,columns,result);
	
	iterateAsArray(rows*columns,result);

    return 0;
}
