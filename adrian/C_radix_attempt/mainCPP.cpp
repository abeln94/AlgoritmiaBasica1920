#include <iostream>
#include <random>
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include "pure_radix.h"

using namespace std;

#define BASE 10

#define DIGITS 3 // <=9

// -------------------- INTEGER --------------------

/**
 * Struct for integers saved as digits
 */
struct int_digit {
    /**
     * The digits
     */
    char digits[DIGITS];

    /**
     * @return the corresponding int
     */
    int as_int() const {
        int i = 0;
        for (int d = DIGITS - 1; d >= 0; --d) {
            i = i * BASE + digits[d];
        }
        return i;
    }

    /**
     * For assignment/access
     */
    char &operator[](int d) {
        return digits[d];
    }

    /**
     * For copy
     */
    int_digit &operator=(int_digit const &rhs) {
//        if(&rhs != this) { // slower
        for (int d = 0; d < DIGITS; ++d) {
            digits[d] = rhs.digits[d];
        }
//        }
        return *this;
    }
};

/**
 * For print
 */
ostream &operator<<(ostream &o, int_digit i) {
    o << i.as_int();
    return o;
}

// -------------------- RANDOM --------------------

static random_device rd;
static mt19937 generator(rd());

/**
 * Generates a random number of DIGITS digits
 * @param as_int the number saved as int (output)
 * @param as_int_digit the number saved as int_digit (output)
 */
void generateRandomNumber(int &as_int, int_digit &as_int_digit) {
    for (int d = 0; d < DIGITS; ++d) {
        as_int_digit[d] = uniform_int_distribution<>(0, BASE - 1)(generator);
    }
    as_int = as_int_digit.as_int();
}

// -------------------- SORT --------------------

/**
 * Sorts the array based on the specified digit of the numbers.
 * Local order is kept
 * @param array array of elements, input and output
 * @param n length of array
 * @param digit digit to sort (0=less significative)
 */
void sortByDigit(int_digit array[], int n, int digit) {

    // count number of each digit O(n)
    int count[BASE] = {0};
    for (int i = 0; i < n; ++i) {
        count[array[i][digit]]++;
    }

    // convert count to accumulate O(BASE) (BASE << n)
    for (int i = 1; i < BASE; ++i) {
        count[i] += count[i - 1];
    }

    // copy to new array in order O(n*DIGITS)
    auto *arraycopy = new int_digit[n]; // using new because otherwise it is allocated on the heap and produces a segmentation fault with very big n
    for (int i = n - 1; i >= 0; --i) {
        int pos = --count[array[i][digit]];
        arraycopy[pos] = array[i];
    }

    // back to original array O(n*DIGITS)
    for (int i = 0; i < n; ++i) {
        array[i] = arraycopy[i];
    }

    delete[](arraycopy);
}

/**
 * Sorts an array of integers by using radix sort
 * @param array array of elements, input and output
 * @param n length of array
 * @param digits number of digits of the max element
 */
void sortByRadix(int_digit array[], int n) {
    for (int d = 0; d < DIGITS; ++d) {
        sortByDigit(array, n, d);
    }
}

// -------------------- MEASURE --------------------

/**
 * Measures time of execution of code
 * @param action code to execute and measure
 * @return the time it took as microseconds
 */
int Measure(const function<void()> &action) {
    if (action == nullptr) return 0;

    auto start = chrono::high_resolution_clock::now();
    action();
    auto end = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

// -------------------- UTILS --------------------

/**
 * Pretty print an array (why isn't this on the std?)
 * @tparam T type of elements in the array
 * @param v array
 * @param n size of array
 */
template<class T>
void print(T v[], int n) {
    cout << "[";
    for (int i = 0; i < n; ++i)
        cout << (i != 0 ? ", " : "") << v[i];
    cout << "]" << endl;
}

// -------------------- MAIN --------------------

int main() {
	// define number of integers to sort
	int N = 10;
	setBase(BASE); // set the base of the numbers to sort in the lib
	
	int data_std[N];
    int_digit data_our[N];
    for (int i = 0; i < 10; ++i) {
		generateRandomNumber(data_std[i], data_our[i]);
    }
    char** data_mine = intArrayToCharMatrix(data_std, N, DIGITS); // Mine, Adrián
	print(data_std, N);
	
	int time_our = 0;
	int time_mine = 0; // Mine, Adrián again
    int time_std = 0;

    // sort
    time_our += Measure([&data_our, N] {
		sortByRadix(data_our, N);
    });

	time_our += Measure([&data_mine, N] {
		radixSort(data_mine, N, DIGITS);
    });

    time_std += Measure([&data_std, N] {
		sort(data_std, data_std + N);
    });

	int* data_mine_int = charMatrixToIntArray(data_mine, N, DIGITS);
    // check
    for (int i = 0; i < N - 1; ++i) {
		if (data_our[i].as_int() > data_our[i + 1].as_int()) {
			cout << "data_our not sorted: ";
            print(data_our, N);
            return -1;
        }
        if (data_mine_int[i] > data_mine_int[i + 1]) {
			cout << "data_mine not sorted: ";
            print(data_mine, N);
            return -1;
		}
        if (data_std[i] > data_std[i + 1]) {
            cout << "data_std not sorted: ";
            print(data_std, N);
            return -1;
        }
	}

	print(data_std, N);

    cout << "Sorted " << N << " elements:" << endl \
			 << "-Radix: " << time_our << endl \
			 << "-Pure Radix: " << time_mine << endl \
			 << "-STD sort: " << time_std << endl;

    return 0;
}
