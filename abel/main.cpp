#include <iostream>
#include <random>
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <thread>

using namespace std;

#define BASE 10
#define NUM_THREADS 4
#define MAX_DIGITS 9


#define N_FIRST 50000
#define N_LAST 500000
#define N_STEP 10000

#define SWAP(a, b) auto c = a; a = b; b = c

// -------------------- INTEGER --------------------

/**
 * Struct for sorting with radix
 */
//struct radix_struct {
    /**
     * The digits
     */
    char data_array[MAX_DIGITS][N_LAST + 1];
    int *data_order;
    int *data_order_temp;
    int data_order_a[N_LAST + 1];
    int data_order_b[N_LAST + 1];
    int data_count[NUM_THREADS][BASE][MAX_DIGITS];
    int data_digits;
    int data_n;

    /**
     * Constructor
     * @param digits number of digits
     * @param n number of elements
     * @param numbers Array of numbers to convert
     */
    void initialize(int _digits, int _n, const int numbers[]) {
        data_digits = _digits;
        data_n = _n;

        for (int i = 0; i < data_digits; ++i) {
            for (int j = 0; j < data_n; j++) {
                data_array[i][j] = (numbers[j] / (int) pow(BASE, i)) % BASE;
            }
        }

        for (int i = 0; i < data_n; ++i) {
            data_order_a[i] = i;
        }

        data_order = data_order_a;
        data_order_temp = data_order_b;

        for (int i = 0; i < NUM_THREADS; ++i) {
            for (int j = 0; j < BASE; ++j) {
                for (int k = 0; k < data_digits; ++k) {
                    data_count[i][j][k] = 0;
                }
            }
        }
    }


    /**
     * @param element which element
     * @param digit which digit
     * @return the corresponding digit
     */
    char get_digit(int element, int digit) {
        return data_array[digit][data_order[element]];
    }

struct {
    /**
     * @param element which elements
     * @return the corresponding element as int
     */
    int operator[](int element) const {
        int i = 0;
        for (int d = data_digits - 1; d >= 0; --d) {
            i = i * BASE + get_digit(element, d);
        }
        return i;
    }
} printer;

    /**
     * Moves the element from position 'from' to position 'to'
     * Must commit changes with commitOrderChanges, changes are not visible from other function until then
     * @param from current position of element
     * @param to final position of element
     */
    void changeOrder(int from, int to) {
        data_order_temp[to] = data_order[from];
    }

    /**
     * Makes changes from changeOrder final so they are visible to the other functions
     */
    void commitOrderChanges() {
        SWAP(data_order, data_order_temp);
    }

//};

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

void thread_count(int id) {

    // count digits O(n*digits)
    int start = (data_n / NUM_THREADS) * id + (data_n % NUM_THREADS > id ? id : data_n % NUM_THREADS);
    int end = (data_n / NUM_THREADS) * (id + 1) + (data_n % NUM_THREADS > (id + 1) ? (id + 1) : data_n % NUM_THREADS);

    for (int i = start; i < end; i++) {
        for (int digit = 0; digit < data_digits; ++digit) {
            data_count[id][get_digit(i, digit)][digit]++;
        }
    }

    // convert count to accumulate O(BASE*digits)
    for (int b = 1; b < BASE; ++b) {
        for (int digit = 0; digit < data_digits; ++digit) {
            data_count[id][b][digit] += data_count[id][b - 1][digit];
        }
    }
}

/**
 * Counts the number of digits for each element
 * @param elements struct with elements, input and output
 */
void countDigits() {

    // launch threads to count
    thread threads[NUM_THREADS];
    for (int t = 0; t < NUM_THREADS; ++t) {
        threads[t] = thread(thread_count, t);
    }
    for (auto &thread : threads) {
        thread.join();
    }

    // accumulate all into first O(threads*base*digits)
    for (int t = 1; t < NUM_THREADS; ++t) {
        for (int i = 0; i < BASE; ++i) {
            for (int d = 0; d < data_digits; ++d) {
                data_count[0][i][d] += data_count[t][i][d];
            }
        }
    }
}


/**
 * Sorts the array based on the specified digit of the numbers.
 * Local order is kept
 * @param elements struct with elements, input and output
 * @param digit digit to sort (0=less significative)
 */
void sortByDigit(int digit) {

    // copy to new array in order O(n)
    for (int i = data_n - 1; i >= 0; --i) {
        int pos = --data_count[0][get_digit(i, digit)][digit];
        changeOrder(i, pos);
    }

    // move from temp to array O(1)
    commitOrderChanges();
}

/**
 * Sorts an array of integers by using radix sort
 * @param elements struct with elements, input and output
 */
void sortByRadix() {
    countDigits();
    for (int d = 0; d < data_digits; ++d) {
        sortByDigit(d);
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
void print(T v, int n) {
    cout << "[";
    for (int i = 0; i < n; ++i)
        cout << (i != 0 ? ", " : "") << v[i];
    cout << "]" << endl;
}


// -------------------- MAIN --------------------

int data_std[N_LAST + 1];

int main() {

#ifdef USING_DEBUG
    cout << "############################" << endl;
    cout << "# Using slow debug version #" << endl;
    cout << "############################" << endl;

    int n[] = {34, 12, 4};
    initialize(2, 3, n);
    sortByRadix();
    print(printer, data_n);
#endif

    ofstream output;
    output.open("output.csv");

    // define
    int REPEAT = 10;

    for (int digits = 1; digits <= MAX_DIGITS; digits++) {
        for (int N = N_FIRST; N <= N_LAST; N += N_STEP) {
            cout << "\rdigits=" << digits << ", N=" << N << flush;
            int time_our = 0;
            int time_std = 0;
            for (int t = 0; t < REPEAT; ++t) {
                // generate random vector
                for (int i = 0; i < N; ++i) {
                    data_std[i] = generateRandomNumber(digits);
                }
                initialize(digits, N, data_std);
//        print(data_std, N);

                // sort
                time_our += Measure([] {
                    sortByRadix();
                });

                time_std += Measure([N] {
                    sort(data_std, data_std + N);
                });

//        print(data_std, N);

                // check
                for (int i = 0; i < N - 1; ++i) {
                    if (printer[i] > printer[i + 1]) {
                        cout << "data_our not sorted: ";
                        print(printer, N);
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
