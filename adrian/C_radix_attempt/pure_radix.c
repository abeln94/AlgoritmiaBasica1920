#include <stdlib.h>
#include <stdio.h> // DELETE AFTER DEBUG THIS LIB
#include "pure_radix.h"

// constant used that represents number's BASE
#define BASE 10
// char to int and int to ascii constant value for conversion
#define ASCII_OFFSET 48

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
	// Array's dinamic allocation
	int* result = (int*) malloc(n*4);
	int i;
	// fill the array with the matrix's representated numbers
	for(i = 0; i < n; i++){
		result[i] = 0; // initially assign 0 because malloc doesn't initiallizes the memory allocated
		int weight = 1; // weight of the current digit in the defined base
		int j;
		for(j = 0; j < digits; j++){
			result[i] += (weight * (int) (matrix[i][j] - ASCII_OFFSET));
			weight *= BASE;
		}
	}
	// return de pointer to the filled array
	return result;
}

// Given an array of N positions of digits size integers, returns a 
// matrix of N rows and digits columns with equivalent values to given 
// according to the explained representation of integers as char 
// sequence
// Coms: helpful link
// 		 https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
char** intArrayToCharMatrix(int* array, int n, unsigned int digits){
	// Matrix's dinamic allocation
	char** result = (char**) malloc(sizeof(char *) * n + sizeof(char) * (digits + 1) * n);
	// ptr is now pointing to the first element in of matrix
    char* ptr = (char*)(result + n);
    // for loop to point rows pointer to appropriate location in matrix
    int i;
    for(i = 0; i < n; i++){
        result[i] = (ptr + (digits + 1) * i); 
    }
    // fill the matrix with the array's numbers
	for(i = 0; i < n; i++){
		int number = array[i];
		result[i][digits] = '\0';
		int j;
		for(j = 0; j < digits; j++){
			result[i][j] = (number % BASE) + (int) ASCII_OFFSET;
			number /= BASE;
		}
	}
	// return de pointer to the filled matrix
	return result;
}
