#include <stdio.h>
#include "pure_radix.h"

int main(){
	const int N = 8;
	int example[] = {123, 456, 789, 102, 0, 5, 27, 54321};
	// test int array to matrix conversion
	char** converted = intArrayToCharMatrix(example, N, 5);
	printf("I've converted the int array to a matrix of chars\n");
	int i;
	for(i = 0; i < N; i++){
		printf("%s\n",converted[i]);
	}
	// test matrix to int array conversion
	int* reconverted = charMatrixToIntArray(converted, N, 5);
	printf("I've converted the matrix of chars to an int array\n");
	for(i = 0; i < N; i++){
		printf("%d\n",reconverted[i]);
	}
	return 0;
}
