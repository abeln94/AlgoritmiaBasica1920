#include <stdio.h>
#include "pure_radix.h"

/* Now it's not available with this version of matrix representation
void printMatrix(char* matrix, int n, int digits){
	printf("Array converted to matrix of chars\n");
	int i;
	for(i = 0; i < n; i++){
		printf("%s\n",(matrix + i * (digits + 1)));
	}
}
*/

void printArray(int* array, int n){
	printf("Matrix of chars converted to an int array\n");
	int i;
	for(i = 0; i < n; i++){
		printf("%d\n",array[i]);
	}
}

int main(){
	const int N = 15;
	int example[] = {54321, 123, 4837, 210, 333, 2516, 8138, 3, 47, 456, 789, 102, 0, 5, 27};
	// test int array to matrix conversion
	char* converted = intArrayToCharMatrix(example, N, 5);
	//printMatrix(converted, N, 5);
	// test matrix to int array conversion
	int* reconverted = charMatrixToIntArray(converted, N, 5);
	printArray(reconverted,N);
	// test sorting
	printf("-> Attempt of sorting\n");
	radixSort(converted, N, 5);
	//printMatrix(converted, N, 5);
	reconverted = charMatrixToIntArray(converted, N, 5);
	printArray(reconverted,N);
	
	return 0;
}
