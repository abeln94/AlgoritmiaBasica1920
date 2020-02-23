#include <stdio.h>
#include "pure_radix.h"

int main(){
	printf("I'm here!");
	const int N = 8;
	int example[] = {123, 456, 789, 102, 0, 5, 27, 54321};
	// test int array to matrix conversion
	char** converted = intArrayToCharMatrix(example, N, 6);
	int i;
	for(i = 0; i < N; i++){
		printf("%s\n",converted[i]);
	}
	return 0;
}
