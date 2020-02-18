#include <iostream>
#include <random>
#include <algorithm>
#include <cassert>
#include <array>
#include <chrono>
#include <fstream>

using namespace std;

#define BASE 4

// -------------------- RANDOM --------------------

static random_device rd;
static mt19937 generator(rd());

int getRandomNumber(int digits) {
    return uniform_int_distribution<>(0, pow(BASE, digits) - 1)(generator);
}

// -------------------- SORT --------------------

#define SWAP(v, a, b) auto temp = v[a]; v[a]=v[b]; v[b]=temp

#if (BASE & (BASE-1)) == 0
#define DIGIT(n, d) (((n) >> (d * ((int)log2(BASE))) ) % BASE)
#else
int powers_BASE[63];

    __attribute__((constructor)) void initialize_powers() {
        powers_BASE[0] = 1;
        int i = 1;
        int pow = 1;
        while (pow * BASE > pow) {
            powers_BASE[i++] = (pow *= BASE);
        }
    }
    #define DIGIT(n, d) (((n) / powers_BASE[d] ) % BASE)
#endif

/**
 * Sorts the array based on the specified digit of the numbers.
 * Local order is kept
 * @param array array of elements, input and output
 * @param n length of array
 * @param digit digit to sort (0=less significative)
 */
void sortByDigit(int array[], int n, int digit, int digits) {

    if (n <= 0 || digit < 0) return;

    // count number of each digit O(n)
    int count[BASE] = {0};
    for (int i = 0; i < n; ++i) {
        count[DIGIT(array[i], digit)]++;
    }

    // convert count to accumulate O(BASE) (BASE << n)
    // also copy to another array
    int limits[BASE + 1] = {0};
    limits[1] = count[0];
    for (int i = 1; i < BASE; ++i) {
        limits[i + 1] = (count[i] += count[i - 1]);
    }

    // copy to new array in order O(n)
    int arraycopy[n];
    for (int i = n - 1; i >= 0; --i) {
        arraycopy[--count[DIGIT(array[i], digit)]] = array[i];
    }

    // back to original array O(n)
    for (int i = 0; i < n; ++i) {
        array[i] = arraycopy[i];
    }

    // sort new array for next digit
    for (int i = 0; i < BASE; ++i) {
        sortByDigit(&array[limits[i]], limits[i + 1] - limits[i], digit - 1, digits);
    }
}

/**
 * Sorts an array of integers by using radix sort
 * @param array array of elements, input and output
 * @param n length of array
 * @param digits number of digits of the max element
 */
void sortByRadix(int array[], int n, int digits) {
    sortByDigit(array, n, digits - 1, digits);
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
    int D = 6; // <=9
    int REPEAT = 10;

    for (int N = 20000; N <= 200000; N += 2000) {
        cout << "\rN=" << N << flush;
        int time_our = 0;
        int time_std = 0;
        for (int t = 0; t < REPEAT; ++t) {

            // generate random vector
            int data_our[N], data_std[N];
            for (int i = 0; i < N; ++i) {
                data_our[i] = data_std[i] = getRandomNumber(D);
            }
//        COUT_VECTOR(vector, N);

            // sort
            MEASURE(time_our, {
                sortByRadix(data_our, N, D);
            })

            MEASURE(time_std, {
                sort(data_std, data_std + N);
            })

//        COUT_VECTOR(vector, N);

            // check
            for (int i = 0; i < N - 1; ++i) {
                if (data_our[i] > data_our[i + 1]) {
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
