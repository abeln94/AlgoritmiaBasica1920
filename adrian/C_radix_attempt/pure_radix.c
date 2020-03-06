#include <stdlib.h>
#include <stdio.h> // DELETE AFTER DEBUG THIS LIB
#include "pure_radix.h"

// Comment the following line to implement auxiliar array of radix sort using dynamic allocation instead of call stack one
#define USE_STACK

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
 * -> auxiliar array needed to store sorted elements into each iteration 
 *    can be reserved into call stack instead of being dynamically 
 *    reserved using a system call, so execution time will be reduced 
 *    significantly
 * -> don't worry about auxiliar array's strings state; if its strings 
 *    don't end with '\0' nothing will happen, because in all cases the 
 *    array returned will be the given as parameter, which strings are 
 *    correctly ended with '\0', so many memory can be saved without 
 *    allocating the byte destinated to store the '\0' character that 
 *    won't be used for sorting
 * -> characters in the matrix don't have to match the ASCII 
 *    representation of the digits; this way, its value as decimal 
 *    (although it suposes being unreadable) can be used as ASCII value, 
 *    so that all these additions and subtractions of the offsets can be 
 *    avoided
 */
void radixSort(char* v, int n, unsigned int digits){
	int columns = digits + 1;
	// Allocation of auxiliar "v", the array to sort in each iteration
#ifdef USE_STACK
	char axlr[n * columns]; // auxiliar array is reserved into call stack, not with dynamic allocation system call
	char* auxiliar = axlr; // https://www.tutorialspoint.com/cprogramming/c_pointer_to_an_array.htm -> using array directly causes types incompatibility
#else
	// system call for dynamic allocation
	char* auxiliar = (char*) malloc(sizeof(char) * columns * n);
#endif
	// Defining pointer to original array, that lets use its reference swapping it while sorting
	char* original = v;
	// Defining variables to be used while sorting
	unsigned int count[BASE] = {0};
	// Loop to sort per each digit
	int i,j,k;
	for(j = 0; j < digits; j++){
		// counts the number of occurrences of each digit
		for(i = 0; i < n; i++) {
			count[(unsigned char) (*(v + i * columns + j) - ASCII_OFFSET)]++;
		}
		// prepares base position after count
		for(i = 1; i < BASE; i++) {
			count[i] += count[i - 1];
		}
		// sort v into auxiliar
		for(i = n - 1; i >= 0; i--) {
			int row = --count[(unsigned char) (*(v + i * columns + j) - ASCII_OFFSET)];
			for(k = 0; k < columns; k++){ 
				*(auxiliar + row * columns + k) = *(v + i * columns + k);
			}
		}
		// swap array's pointers		
		char* aux = v;
		v = auxiliar;
		auxiliar = aux;
		// DEBUG
		for(i = 0; i < n; i++){
			*(auxiliar + i * columns + digits) = '\0';
		}
		printf("-> This is v into %d iteration\n",j);
		for(i = 0; i < n; i++){
			printf("v[%d] = %s\n",i,(v+i*columns));
		}
		printf("-> This is auxiliar into %d iteration\n",j);
		for(i = 0; i < n; i++){
			printf("auxiliar[%d] = %s\n",i,(auxiliar+i*columns));
		}
		// /DEBUG
	}
	// Check if last array used as sorted isn't the given one and all elements have to be copied before return
	if(original != v){
		for(i = 0; i < n; i++){
			for(j = 0; j < digits; j++){
				*(v + i * columns + j) = *(auxiliar + i * columns + j);
			}
		}
	}
}

// Given a matrix of N rows and digits columns, returns an array of 
// integers of N positions with equivalent values to given according 
// to the explained representation of integers as char sequence
int* charMatrixToIntArray(char* matrix, int n, unsigned int digits){
	// Array's dinamic allocation
	int* result = (int*) malloc(sizeof(int) * n);
	int i;
	// fill the array with the matrix's representated numbers
	for(i = 0; i < n; i++){
		result[i] = 0; // initially assign 0 because malloc doesn't initiallizes the memory allocated
		int weight = 1; // weight of the current digit in the defined base
		int j;
		for(j = 0; j < digits; j++){
			result[i] += (weight * (int) (*(matrix + i * (digits + 1) + j) - ASCII_OFFSET));
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
//       ...alternative representation emulating matrix
//       https://stackoverflow.com/questions/2128728/allocate-matrix-in-c
//       ...this last representation with only pointer operations (not 
//       indexes)
//       https://www.tutorialspoint.com/how-to-dynamically-allocate-a-2d-array-in-c
char* intArrayToCharMatrix(int* array, int n, unsigned int digits){
	int columns = digits + 1;
	// Matrix's dinamic allocation
	char* result = (char*) malloc(sizeof(char) * columns * n);
    // fill the matrix with the array's numbers
    int i;
	for(i = 0; i < n; i++){
		int number = array[i];
		*(result + i * columns + digits) = '\0';
		int j;
		for(j = 0; j < digits; j++){
			*(result + i * columns + j) = (number % BASE) + (int) ASCII_OFFSET;
			number /= BASE;
		}
	}
	// return de pointer to the filled matrix
	return result;
}
