#include <iostream>
#include <random>
#include <algorithm>
#include <cassert>
#include <array>
#include <chrono>
#include <fstream>

using namespace std;

#define BASE 10

// -------------------- RANDOM --------------------

static random_device rd;
static mt19937 generator(rd());

int getRandomNumber(int digits) {
    return uniform_int_distribution<>(0, pow(BASE, digits) - 1)(generator);
}

// -------------------- SORT --------------------

#define SWAP(v, a, b) auto temp = v[a]; v[a]=v[b]; v[b]=temp
#define DIGIT(n, d) (((n) / (int) pow(BASE,(d))) % BASE)

/**
 * Sorts the array based on the specified digit of the numbers.
 * Local order is kept
 * @param array array of elements, input and output
 * @param n length of array
 * @param digit digit to sort (0=less significative)
 */
void sortByDigit(int array[], int n, int digit) {

    // count number of each digit O(n)
    int count[BASE] = {0};
    for (int i = 0; i < n; ++i) {
        count[DIGIT(array[i], digit)]++;
    }

    // convert count to accumulate O(BASE)
    for (int i = 1; i < BASE; ++i) {
        count[i] += count[i - 1];
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
}

/**
 * Sorts an array of integers by using radix sort
 * @param array array of elements, input and output
 * @param n length of array
 * @param digits nuber of digits of the max element
 */
void sortByRadix(int array[], int n, int digits) {
    // sort by each digit
    for (int i = 0; i < digits; ++i) {
        sortByDigit(array, n, i);
    }
}

// -------------------- MAIN --------------------

#define COUT_VECTOR(v, n) cout << "["; for (int i = 0; i < n; ++i) cout << (i != 0 ? ", " : "") << v[i]; cout << "]" << endl;

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
    int D = 9; // <=9
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
                assert(data_our[i] <= data_our[i + 1]);
                assert(data_std[i] <= data_std[i + 1]);
            }
        }

        time_our /= REPEAT;
        time_std /= REPEAT;
        output << N << " " << time_our << " " << time_std << endl;
    }

    return 0;
}
