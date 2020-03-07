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
    int *order;
    int *order_temp;
    int *count[BASE];
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

        for (auto &i : count) {
            i = new int[digits] {0};
        }
    }

    ~radix_struct() {
        for (int i = 0; i < digits; ++i) {
            delete[] array[i];
        }
        delete[] array;
        delete[] order;
        delete[] order_temp;

        for (auto &i : count) {
            delete[] i;
        }
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
 * Counts the number of digits for each element
 * @param elements struct with elements, input and output
 */
void countDigits(radix_struct &elements) {

    // count
    for (int i = 0; i < elements.n; ++i) {
        for (int digit = 0; digit < elements.digits; ++digit) {
            elements.count[elements.digit(i, digit)][digit]++;
        }
    }

    // convert count to accumulate O(BASE) (BASE << n)
    for (int b = 1; b < BASE; ++b) {
        for (int digit = 0; digit < elements.digits; ++digit) {
            elements.count[b][digit] += elements.count[b-1][digit];
        }
    }
}

/**
 * Sorts the array based on the specified digit of the numbers.
 * Local order is kept
 * @param elements struct with elements, input and output
 * @param digit digit to sort (0=less significative)
 */
void sortByDigit(radix_struct &elements, int digit) {

    // copy to new array in order O(n)
    for (int i = elements.n - 1; i >= 0; --i) {
        int pos = --elements.count[elements.digit(i, digit)][digit];
        elements.changeOrder(i, pos);
    }

    // move from temp to array O(1)
    elements.commitOrderChanges();
}

/**
 * Sorts an array of integers by using radix sort
 * @param elements struct with elements, input and output
 */
void sortByRadix(radix_struct &elements) {
    countDigits(elements);
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
void print(T v, int n) {
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
        for (int N = 5000; N <= 50000; N += 10000) {
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
                time_our += Measure([&data_our] {
                    sortByRadix(data_our);
                });

                time_std += Measure([&data_std, N] {
                    sort(data_std, data_std + N);
                });

//        print(data_std, N);

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
                }
            }

            time_our /= REPEAT;
            time_std /= REPEAT;
            output << digits << " " << N << " " << time_our << " " << time_std << endl;
        }
    }

    return 0;
}
