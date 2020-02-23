#include <stdlib.h>
#include <stdio.h> // DELETE AFTER DEBUG THIS LIB
#include "pure_radix.h"

// variable which value is used as number's base
static unsigned int base = 10;
// char to int and int to ascii constant values for conversion
static const char charOfInt[] = {'0','1','2','3','4','5','6','7','8','9'};
static const char ASCII_NUMBER_MASK = 0x0F; // ASCII code of numbers goes from 0x30 to 0x39

// Sets the value of the number's base
void setBase(unsigned int b){
	if(b){ // b != 0
		base = b;
	}
}

// Sorts array using radix sort
/*
 * TIPS OF IMPLEMENTATION:
 * -> dynamic allocation's system calls are expensive in time, so them 
 *    are executed once, reserving space for array location at the 
 *    begining (this array is reused for each iteration of sorting) and 
 *    freeing it at the end, when the step explained in the following 
 *    paragraph concludes
 * -> function's calls are expensive in time, so implementation doesn't 
 *    use auxiliar functions (only standard library's one when it's 
 *    strictly necessary, and if them manage better than directly a 
 *    system calls, without adding unnecessary processing). These branch 
 *    instruction (branch/jump and link) and stack's management is 
 *    avoided (taking into consideration an optimization of compiler 
 *    could be making it)
 * -> array is copied (at most) once;  RAM's position of array given as 
 *    parameter is stored in an aditional variable, and array used to 
 * 	  keep sorted values is switched between the original and the 
 * 	  dynamic one, so only if result isn't stored in the array given 
 *    as parameter when the sorting ends, a copy is needed
 */
void radixSort(char** v, int n, unsigned int digits){
		
}

// Given a matrix of N rows and digits columns, returns an array of 
// integers of N positions with equivalent values to given according 
// to the explained representation of integers as char sequence
int* charMatrixToIntArray(char** matrix, int n, unsigned int digits){
	int* result = (int*) malloc(n*4);
	int i;
	for(i = 0; i < n; i++){
		result[i] = 0;
		int weight = 1;
		int j;
		for(j = digits-1; j >= 0; j--){
			result[i] += (weight * (int) (matrix[i][j] & ASCII_NUMBER_MASK));
			weight *= base;
		}
	}
	return result;
}

// Given an array of N positions of digits size integers, returns a 
// matrix of N rows and digits columns with equivalent values to given 
// according to the explained representation of integers as char 
// sequence
char** intArrayToCharMatrix(int* array, int n, unsigned int digits){
	char** result = (char**) malloc(n*(digits+1));
	int i;
	for(i = 0; i < n; i++){
		int number = array[i];
		result[i][digits] = '\0';
		int j;
		for(j = digits-1; j >= 0; j--){
			result[i][j] = charOfInt[number % base];
			number /= base;
		}
	}
	return result;
}
