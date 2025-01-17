#ifndef RADIX_H
#define RADIX_H

/*
 * CHAR MATRIX REPRESENTATION
 * Array of N integers is representated as a matrix NxDIGITS of 
 * characters. Each row of the matrix represent an integer in the array, 
 * and the matrix has DIGITS number of columns, being DIGITS the number 
 * of digits of the largest number in the array. Numbers in a row are 
 * ordered by least to most significant digit per column's value: 
 * 	array[i]=123 => martix[i][0]=3; matrix[i][1]=2; matrix[i][2]=1
 * 
 * It's obviated in this explanation the '\0' character of end of string 
 * in C, because the rows are not strings. Each position in the row n 
 * of the matrix represents a digit of the n-th number in the array, but 
 * although them are stored into char variables, they are codified with 
 * the binary unsigned number of the digit, not with the ASCII code of 
 * that number
 */

/**
 * Sorts an array of integers by using radix sort
 * @param array array of elements, input and output
 * @param n length of array
 * @param digits number of digits of the max element
 */
void radixSort(char* v, int n, unsigned int digits);

/**
 * Given a matrix of N rows and digits columns, returns an array of 
 * integers of N positions with equivalent values to given according 
 * to the explained representation of integers as char sequence
 * @param matrix of digits
 * @param n length of array
 * @param digits number of digits of the max element
 */
int* charMatrixToIntArray(char* matrix, int n, unsigned int digits);

/**
 * Given an array of N positions of digits size integers, returns a 
 * matrix of N rows and digits columns with equivalent values to given 
 * according to the explained representation of integers as char 
 * sequence
 * @param array of integers
 * @param n length of array
 * @param digits number of digits of the max element
 */
char* intArrayToCharMatrix(int* array, int n, unsigned int digits);

#endif
