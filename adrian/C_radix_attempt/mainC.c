#include <stdio.h>
#include "pure_radix.h"

void printMatrix(char** matrix, int n){
	printf("Array converted to matrix of chars\n");
	int i;
	for(i = 0; i < n; i++){
		printf("%s\n",matrix[i]);
	}
}

void printArray(int* array, int n){
	printf("Matrix of chars converted to an int array\n");
	int i;
	for(i = 0; i < n; i++){
		printf("%d\n",array[i]);
	}
}

int main(){
	const int N = 8;
	int example[] = {123, 456, 789, 102, 0, 5, 27, 54321};
	// test int array to matrix conversion
	char** converted = intArrayToCharMatrix(example, N, 5);
	printMatrix(converted,N);
	// test matrix to int array conversion
	int* reconverted = charMatrixToIntArray(converted, N, 5);
	printArray(reconverted,N);
	// test sorting
	radixSort(converted, N, 5);
	
	return 0;
}
