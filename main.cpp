#include <iostream>
#include <random>
#include <algorithm>
#include <cassert>

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

int main() {

#ifdef USING_DEBUG
    cout << "############################" << endl;
    cout << "# Using slow debug version #" << endl;
    cout << "############################" << endl;
#endif

    // define
    int N = 100;
    int D = 6;

    // generate random vector
    int vector[N];
    for (int i = 0; i < N; ++i) {
        vector[i] = getRandomNumber(D);
    }
    COUT_VECTOR(vector, N);

    // sort
    sortByRadix(vector, N, D);
    COUT_VECTOR(vector, N);

    // check
    for (int i = 0; i < N - 1; ++i) {
        assert(vector[i] <= vector[i + 1]);
    }

    return 0;
}
