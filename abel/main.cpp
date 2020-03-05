#include <iostream>
#include <random>
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>

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
    char **temp;
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
        temp = new char *[digits];
        for (int i = 0; i < digits; ++i) {
            array[i] = new char[n];
            temp[i] = new char[n];
            for (int j = 0; j < n; j++) {
                array[i][j] = (numbers[j] / (int) pow(BASE, i)) % BASE;
            }
        }

    }

    ~radix_struct() {
        for (int i = 0; i < digits; ++i) {
            delete[] array[i];
            delete[] temp[i];
        }
        delete[] array;
        delete[] temp;
    }

    /**
     * @return the corresponding int
     */
    int as_int(int e) const {
        int i = 0;
        for (int d = digits - 1; d >= 0; --d) {
            i = i * BASE + array[d][e];
        }
        return i;
    }

};

/**
 * For print
 */
ostream &operator<<(ostream &o, const radix_struct &elements) {
    o << "[";
    for (int i = 0; i < elements.n; ++i)
        o << (i != 0 ? ", " : "") << elements.as_int(i);
    o << "]" << endl;
    return o;
}

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
        count[elements.array[digit][i]]++;
    }

    // convert count to accumulate O(BASE) (BASE << n)
    for (int i = 1; i < BASE; ++i) {
        count[i] += count[i - 1];
    }

    // copy to new array in order O(n*BASE)
    for (int i = elements.n - 1; i >= 0; --i) {
        int pos = --count[elements.array[digit][i]];
        for(int j=0;j<elements.digits;++j){
            elements.temp[j][pos] = elements.array[j][i];
        }
    }

    // move from temp to array O(1)
    SWAP(elements.temp, elements.array);
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
 * Pretty print an array (why this isn't on the std?)
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

#ifdef USING_DEBUG
    cout << "############################" << endl;
    cout << "# Using slow debug version #" << endl;
    cout << "############################" << endl;

    int n[] = {34, 12, 4};
    radix_struct s(2, 3, n);
    sortByRadix(s);
#endif

    ofstream output;
    output.open("output.csv");

    // define
    int REPEAT = 10;

    for (int digits = 1; digits <= 9; digits++) {
        for (int N = 5000; N <= 50000; N += 1000) {
            cout << "\rdigits=" << digits << ", N=" << N << flush;
            int time_our = 0;
            int time_std = 0;
            for (int t = 0; t < REPEAT; ++t) {

                // generate random vector
                int data_std[N];
                for (int i = 0; i < N; ++i) {
                    data_std[i] = generateRandomNumber(digits);
                }
                radix_struct data_our(digits, N, data_std);
//        print(data_std, N);

                // sort
                time_our += Measure([&data_our, N] {
                    sortByRadix(data_our);
                });

                time_std += Measure([&data_std, N] {
                    sort(data_std, data_std + N);
                });

//        print(data_std, N);

                // check
                for (int i = 0; i < N - 1; ++i) {
                    if (data_our.as_int(i) > data_our.as_int(i + 1)) {
                        cout << "data_our not sorted: ";
                        cout << data_our;
                        return -1;
                    }
                    if (data_std[i] > data_std[i + 1]) {
                        cout << "data_std not sorted: ";
                        print(data_std, N);
                        return -1;
                    }
                }
            }

            time_our /= REPEAT;
            time_std /= REPEAT;
            output << digits << " " << N << " " << time_our << " " << time_std << endl;
        }
    }

    return 0;
}
