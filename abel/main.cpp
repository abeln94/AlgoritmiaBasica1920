#include <iostream>
#include <random>
#include <algorithm>
#include <cassert>
#include <array>
#include <chrono>
#include <fstream>

using namespace std;

#define BASE 10

#define DIGITS 6 // <=9

// -------------------- integer as digits --------------------

typedef int int_digit[DIGITS];

int getInt(const int_digit &data) {
    int i = 0;
    for (int d = DIGITS - 1; d >= 0; --d) {
        i = i * BASE + data[d];
    }
    return i;
}

// -------------------- RANDOM --------------------

static random_device rd;
static mt19937 generator(rd());

void getRandomNumber(int &as_int, int_digit &as_int_digit) {
    for (int d = 0; d < DIGITS; ++d) {
        as_int_digit[d] = uniform_int_distribution<>(0, BASE - 1)(generator);
    }
    as_int = getInt(as_int_digit);
}

// -------------------- SORT --------------------

#define SWAP(v, a, b) auto temp = v[a]; v[a]=v[b]; v[b]=temp


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
        for (int d = 0; d < DIGITS; ++d) {
            arraycopy[pos][d] = array[i][d];
        }
    }

    // back to original array O(n*DIGITS)
    for (int i = 0; i < n; ++i) {
        for (int d = 0; d < DIGITS; ++d) {
            array[i][d] = arraycopy[i][d];
        }
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

// -------------------- MAIN --------------------

#define COUT_VECTOR(v, n) cout << "["; for (int i = 0; i < n; ++i) cout << (i != 0 ? ", " : "") << v[i]; cout << "]" << endl

#define MEASURE(variable, code) {                                                      \
    auto start = chrono::high_resolution_clock::now();                                 \
    code                                                                               \
    auto end = chrono::high_resolution_clock::now();                                   \
    variable += chrono::duration_cast<chrono::microseconds>(end - start).count();      \
}

int main() {

#ifdef USING_DEBUG
    cout << "############################" << endl;
    cout << "# Using slow debug version #" << endl;
    cout << "############################" << endl;
#endif

    ofstream output;
    output.open("output.csv");

    // define
    int REPEAT = 10;

    for (int N = 20000; N <= 200000; N += 2000) {
        cout << "\rN=" << N << flush;
        int time_our = 0;
        int time_std = 0;
        for (int t = 0; t < REPEAT; ++t) {

            // generate random vector
            int data_std[N];
            int_digit data_our[N];
            for (int i = 0; i < N; ++i) {
                getRandomNumber(data_std[i], data_our[i]);
            }
//        COUT_VECTOR(vector, N);

            // sort
            MEASURE(time_our, {
                sortByRadix(data_our, N);
            })

            MEASURE(time_std, {
                sort(data_std, data_std + N);
            })

//        COUT_VECTOR(vector, N);

            // check
            for (int i = 0; i < N - 1; ++i) {
                if (getInt(data_our[i]) > getInt(data_our[i + 1])) {
                    cout << "data_our not sorted";
                    COUT_VECTOR(data_our, N);
                    return -1;
                }
                if (data_std[i] > data_std[i + 1]) {
                    cout << "data_std not sorted";
                    COUT_VECTOR(data_std, N);
                    return -1;
                }
            }
        }

        time_our /= REPEAT;
        time_std /= REPEAT;
        output << N << " " << time_our << " " << time_std << endl;
    }

    return 0;
}
