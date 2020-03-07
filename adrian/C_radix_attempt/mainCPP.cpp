#include <iostream>
#include <random>
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
extern "C"{
#include "pure_radix.h"
}

using namespace std;

#define BASE 10

#define SWAP(a, b) auto c = a; a = b; b = c

// -------------------- INTEGER --------------------

/**
 * Struct for sorting with radix
 */
struct radix_struct {
    /**
     * The digits
     */
    char **array;
    int *order;
    int *order_temp;
    int digits;
    int n;

    /**
     * Constructor
     * @param digits number of digits
     * @param n number of elements
     * @param numbers Array of numbers to convert
     */
    radix_struct(int digits, int n, const int numbers[]) {
        this->digits = digits;
        this->n = n;

        array = new char *[digits];
        for (int i = 0; i < digits; ++i) {
            array[i] = new char[n];
            for (int j = 0; j < n; j++) {
                array[i][j] = (numbers[j] / (int) pow(BASE, i)) % BASE;
            }
        }

        order = new int[n];
        order_temp = new int[n];
        for (int i = 0; i < n; ++i) {
            order[i] = i;
        }
    }

    ~radix_struct() {
        for (int i = 0; i < digits; ++i) {
            delete[] array[i];
        }
        delete[] array;
        delete[] order;
        delete[] order_temp;
    }

    /**
     * @param element which elements
     * @return the corresponding element as int
     */
    int operator[](int element) const {
        int i = 0;
        for (int d = digits - 1; d >= 0; --d) {
            i = i * BASE + digit(element, d);
        }
        return i;
    }

    /**
     * @param element which element
     * @param digit which digit
     * @return the corresponding digit
     */
    char digit(int element, int digit) const {
        return array[digit][order[element]];
    }

    /**
     * Moves the element from position 'from' to position 'to'
     * Must commit changes with commitOrderChanges, changes are not visible from other function until then
     * @param from current position of element
     * @param to final position of element
     */
    void changeOrder(int from, int to) {
        order_temp[to] = order[from];
    }

    /**
     * Makes changes from changeOrder final so they are visible to the other functions
     */
    void commitOrderChanges() {
        SWAP(order, order_temp);
    }

};

// -------------------- RANDOM --------------------

static random_device rd;
static mt19937 generator(rd());

/**
 * Generates a random number of DIGITS digits
 * @param as_int the number saved as int (output)
 * @param as_int_digit the number saved as radix_struct (output)
 */
int generateRandomNumber(int digits) {
    return uniform_int_distribution<>(0, pow(BASE, digits) - 1)(generator);
}

// -------------------- SORT --------------------

/**
 * Sorts the array based on the specified digit of the numbers.
 * Local order is kept
 * @param origin array of unsorted elements, input
 * @param destiny array of sorted elements, output
 * @param n length of array
 * @param digit digit to sort (0=less significative)
 */
void sortByDigit(radix_struct &elements, int digit) {

    // count number of each digit O(n)
    int count[BASE] = {0};
    for (int i = 0; i < elements.n; ++i) {
        count[elements.digit(i, digit)]++;
    }

    // convert count to accumulate O(BASE) (BASE << n)
    for (int i = 1; i < BASE; ++i) {
        count[i] += count[i - 1];
    }

    // copy to new array in order O(n)
    for (int i = elements.n - 1; i >= 0; --i) {
        int pos = --count[elements.digit(i, digit)];
        elements.changeOrder(i, pos);
    }

    // move from temp to array O(1)
    elements.commitOrderChanges();
}

/**
 * Sorts an array of integers by using radix sort
 * @param elements struct with elements, input and output
 * @param n length of array
 * @param digits number of digits of the max element
 */
void sortByRadix(radix_struct &elements) {
    for (int d = 0; d < elements.digits; ++d) {
        sortByDigit(elements, d);
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
 * Pretty print any array (why this isn't on the std?)
 * @tparam T type of elements in the array
 * @param v array
 * @param n size of array
 */
template<typename T>
void print(T &v, int n) {
    cout << "[";
    for (int i = 0; i < n; ++i)
        cout << (i != 0 ? ", " : "") << v[i];
    cout << "]" << endl;
}
template<typename T>
void print(T v[], int n) {
    cout << "[";
    for (int i = 0; i < n; ++i)
        cout << (i != 0 ? ", " : "") << v[i];
    cout << "]" << endl;
}

// -------------------- MAIN --------------------

int main() {

    ofstream output;
    output.open("output.csv");

    // define
    int REPEAT = 10;

    for (int digits = 1; digits <= 9; digits++) {
        for (int N = 5000; N <= 50000; N += 1000) {
            cout << "\rdigits=" << digits << ", N=" << N << flush;
            int time_our = 0;
            int time_std = 0;
            int time_purec = 0;
            for (int t = 0; t < REPEAT; ++t) {

                // generate random vector
                int data_std[N];
                for (int i = 0; i < N; ++i) {
                    data_std[i] = generateRandomNumber(digits);
                }
                radix_struct data_our(digits, N, data_std);
                char* data_purec = intArrayToCharMatrix(data_std, N, digits);
//        print(data_std, N);

                // sort
                time_our += Measure([&data_our, N] {
                    sortByRadix(data_our);
                });

                time_std += Measure([&data_std, N] {
                    sort(data_std, data_std + N);
                });
                
                time_purec += Measure([&data_purec, N, digits] {
					radixSort(data_purec, N, digits);
				});

//        print(data_std, N);

				int* data_purec_int = charMatrixToIntArray(data_purec, N, digits);
                // check
                for (int i = 0; i < N - 1; ++i) {
                    if (data_our[i] > data_our[i + 1]) {
                        cout << "data_our not sorted: ";
                        print(data_our, N);
                        return -1;
                    }
                    if (data_std[i] > data_std[i + 1]) {
                        cout << "data_std not sorted: ";
                        print(data_std, N);
                        return -1;
                    }
                    if (data_purec_int[i] > data_purec_int[i + 1]) {
						cout << "data_purec not sorted: ";
						print(data_purec_int, N);
						return -1;
					}
                }
            }

            time_our /= REPEAT;
            time_std /= REPEAT;
            time_purec /= REPEAT;
            output << digits << " " << N << " " << time_our << " " << time_std << " " << time_purec << endl;
        }
    }

    return 0;
}
