#include <stdlib.h>
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
 * Starting on basis of the given representation of radix sort...
 * => SEQUENTIAL OPTIMIZATIONS
 * -> instead of using queues for each base's value counting of digits, 
 *    an array with base's number of values dimension is used
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
 * -> as consequence of the two previous points, the representation of 
 *    the numbers into the matrix's rows can be optimized making each 
 *    digit representation turns into its decimal value as char and not 
 *    the ASCII one. At that point, as no characters are representated, 
 *    string compatibility can be avoided, so '\0' at the end of each 
 *    row can be omited, turning the (n x (digits + 1)) matrix into a 
 *    (n x digits) one
 * -> instead of using both integers and pointers to index the matrices, 
 *    only pointers would be used, because it's the way the effective 
 *    access to its elements is done, so that the calculus binded to the 
 *    logical representation asociated to integers could be 
 *    precalculated, avoiding it for each iteration and reducing 
 *    indexation time (taking into consideration a compiler's optimizer 
 *    smart enough should be doing it whithout programming it 
 *    explicitly, it's done due to the age of the compiler's version)
 * => PARALLEL OPTIMIZATIONS
 *    -Taking into consideration radix sort's three phases for each digit
 *    1) Counting the number of apearences of each value of the basis 
 *       into observed digit's position of the array's integers
 *       observated of the array
 *    2) Acumulating for each value of the basis the count of the 
 *       previous one to establish the array's top positions when 
 *       sorting
 *    3) Sort the array's numbers copying each one into an auxiliar one, 
 *       with index the number given by the accumulation array
 *    -While third phase must be sequential, the first two might not, so 
 *    some optimizations based on parallelize its calculus can be done. 
 *    These optimizations will work properly for large size of the array 
 *    given to be sorted, because time inverted into system calls to 
 *    create and throw the threads should not be ignored. Remember a 
 *    successful multithread execution is only achieved when the number 
 *    of threads created is equal to the number of cores of the CPU used
 * ~> calculate the two first phases dividing the N numbers array given 
 *    per number of cores of the processor. This will work specially 
 *    better than other divisions when the number of values of the basis 
 *    is not a multiple of the number of cores
 * ~> calculate the two first phases each iteration calculating such 
 *    many digits as the number of cores of the processor has, 
 *    simultaneously, so that the third phase is executed that number of 
 *    times when the first and the second one end
 * ~> calculate the two first phases giving each thread such many values 
 *    of the basis as the number of values the basis has divided per the 
 *    number of cores
 */
void radixSort(char* v, int n, unsigned int digits){
	// Allocation of auxiliar "v", the array to sort in each iteration
#ifdef USE_STACK
	char axlr[n * digits]; // auxiliar array is reserved into call stack, not with dynamic allocation system call
	char* auxiliar = axlr; // https://www.tutorialspoint.com/cprogramming/c_pointer_to_an_array.htm -> using array directly causes types incompatibility
#else
	// system call for dynamic allocation
	char* axlr = (char*) malloc(sizeof(char) * digits * n);
	char* auxiliar = axlr;
#endif
	// Defining pointer to original array, that lets use its reference swapping it while sorting
	char* original = v;
	// Loop to sort per each digit
	int i,j,k;
	for(j = 0; j < digits; j++){
		// Defining array used to count number of digits
		unsigned int count[BASE] = {0};
		// counts the number of occurrences of each digit
		for(i = 0; i < n; i++) {
			count[(unsigned char) *(original + i * digits + j)]++;
		}
		// prepares base position after count
		for(i = 1; i < BASE; i++) {
			count[i] += count[i - 1];
		}
		// sort v into auxiliar
		for(i = n - 1; i >= 0; i--) {
			int row = --count[(unsigned char) *(original + i * digits + j)];
			for(k = 0; k < digits; k++){ 
				*(auxiliar + row * digits + k) = *(original + i * digits + k);
			}
		}
		// swap array's pointers	
		char* aux = original;
		original = auxiliar;
		auxiliar = aux;
	}
	// Check if last array used as sorted isn't the given one and all elements have to be copied before return
	if(original != v){
		for(i = 0; i < n; i++){
			for(j = 0; j < digits; j++){
				*(v + i * digits + j) = *(axlr + i * digits + j);
			}
		}
	}
#ifndef USE_STACK
	free(axlr);
#endif
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
			result[i] += (weight * (int) *(matrix + i * digits + j));
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
	// Matrix's dinamic allocation
	char* result = (char*) malloc(sizeof(char) * digits * n);
    // fill the matrix with the array's numbers
    int i;
	for(i = 0; i < n; i++){
		int number = array[i];
		int j;
		for(j = 0; j < digits; j++){
			*(result + i * digits + j) = (number % BASE);
			number /= BASE;
		}
	}
	// return de pointer to the filled matrix
	return result;
}
