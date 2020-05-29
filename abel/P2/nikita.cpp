#include <iostream>
#include <random>
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <thread>
#include <stack>
#include <list>
#include <cstring>
#include <map>

using namespace std;

// -------------------- MEASURE --------------------

/**
 * Measures time of execution of code
 * @param action code to execute and measure
 * @return the time it took as seconds
 */
float Measure(const function<void()> &action) {
    if (action == nullptr) return 0;

    auto start = chrono::high_resolution_clock::now();
    action();
    auto end = chrono::high_resolution_clock::now();

    return (float) chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0f / 1000.0f;
}

// -------------------- ALGORITHM --------------------

vector<int> positions_x, positions_y;
list<int> available, traversed;
int bestcost, traversedcost;
vector<vector<int>> distances;

/**
 * Solve step
 */
void solve_step() {

    if (available.empty()) {
        // solution, update best cost
        int from = traversed.back();
        int localcost = traversedcost + distances[from][0];
        if (localcost < bestcost) {
            bestcost = localcost;
        }
        return;
    }


    // last traversed element
    int last = traversed.back();

    // get all next elements
    pair<int, int> sort_available[available.size()];
    int i = 0;
    for (int &nextpos : available) {
        sort_available[i++] = make_pair(distances[last][nextpos] + distances[nextpos][0], nextpos);
    }

    // sort by closest
    sort(sort_available, sort_available + i);
    int worst = traversedcost + sort_available[i - 1].first;

    // traverse elements
    for (auto &nextpos_ : sort_available) {

        // early cut: if worst distance is less than best, cut all remaining
        // repeat each time because bestcost may be updated
        // no need for another local cut
        if (worst >= bestcost) break;

        int nextpos = nextpos_.second;

        int lastcost = distances[last][nextpos];


        // move as traversed
        traversed.push_back(nextpos);
        available.remove(nextpos);
        traversedcost += lastcost;

        // solve
        solve_step();

        // restore
        available.push_back(nextpos);
        traversed.pop_back();
        traversedcost -= lastcost;
    }
}

/**
 * Starts the solving process
 */
void solve() {

    // initialize variables
    bestcost = INT32_MAX;
    traversedcost = 0;

    // initialize vectors
    traversed.clear();
    traversed.push_back(0);
    available.clear();
    for (int i = 1; i < positions_x.size(); ++i) available.push_back(i);

    // initialize distances
    distances = vector<vector<int>>(positions_x.size());
    for (int i = 0; i < positions_x.size(); ++i) {
        distances[i] = vector<int>(positions_x.size());
        for (int j = 0; j < positions_x.size(); ++j) {
            distances[i][j] = abs(positions_x[i] - positions_x[j]) + abs(positions_y[i] - positions_y[j]);
        }
    }

    // run
    solve_step();
}

/**
 * Runs the algorithm for the provided file, writes results to a new file
 * @param filenameIn name of the input file
 * @param filenameOut name for the output file
 */
void executeFile(const string &filenameIn, const string &filenameOut) {

    // prepare files
    ifstream data(filenameIn);
    if (!data.is_open()) {
        cout << "Unable to open input file " << filenameIn << endl;
        return;
    }

    ofstream solution(filenameOut);
    if (!solution.is_open()) {
        cout << "Unable to open output file " << filenameOut << endl;
        return;
    }

    // read N
    int N;
    data >> N;

    // for statistics
    map<int, float> n_seconds;
    map<int, int> n_tests;

    for (int n = 0; n < N; ++n) {
        // foreach scenario

        // read elements
        int A, B, pos_x, pos_y, M;
        data >> A >> B
             >> pos_x >> pos_y
             >> M;


        // initialize
        positions_x.clear();
        positions_y.clear();
        positions_x.push_back(pos_x);
        positions_y.push_back(pos_y);


        // read mines
        for (int _m = 0; _m < M; ++_m) {
            data >> pos_x >> pos_y;

            bool add = true;

            // don't add if it is already a position
            for (int mm = 0; mm < positions_x.size() && add; ++mm) {
                if (pos_x == positions_x[mm] && pos_y == positions_y[mm]) add = false;
            }

            // add
            if (add) {
                positions_x.push_back(pos_x);
                positions_y.push_back(pos_y);
            }
        }

        // execute
        float seconds = (float) Measure([] { solve(); });

        // write results
        cout << n + 1 << '/' << N << ", n=" << positions_x.size() << " : " << bestcost << ' ' << seconds << endl;
        solution << bestcost << ' ' << seconds << endl;

        // update statistics
        n_seconds[positions_x.size()] += seconds;
        ++n_tests[positions_x.size()];

    }

    // print statistics
    cout << "Statistics:" << endl;
    for (auto values : n_seconds) {
        cout << "n=" << values.first << " :"
             << " tests=" << n_tests[values.first]
             << " average=" << values.second / n_tests[values.first] << endl;
    }
}

/**
 * Runs multiple random tests
 */
void executeInternal() {

    int N = 20; // arbitrary size of the board
    int n_start = 2; // minimum value of N to test
    int n_end = 20; // maximum value of N to test
    int TESTS = 50; // number of tests per N

    // initialize an array with all possible positions, for random pick
    int values[N * N];
    for (int n = 0; n < N * N; ++n) values[n] = n;

    // execute for different number of positions
    for (int n = n_start; n <= n_end; ++n) {

        // initialize n data
        float seconds = 0;

        // execute several tests (for average)
        for (int t = 0; t < TESTS; ++t) {

            // initialize
            positions_x.clear();
            positions_y.clear();

            // pick n random positions
            shuffle(values, values + (N * N), default_random_engine(chrono::system_clock::now().time_since_epoch().count()));
            for (int p = 0; p < n; ++p) {
                positions_x.push_back(values[p] / N);
                positions_y.push_back(values[p] % N);
            }

            // execute
            cout << "<" << flush;
            seconds += (float) Measure([] { solve(); });
            cout << ">" << flush;

        }
        cout << " n=" << n << " seconds=" << seconds / (float) TESTS << endl;
    }
}

/**
 * Compares the results of two executions
 * @param filenameOur name of the file to check
 * @param filenameBase name of the file to use as a base
 */
void compare(const string &filenameOur, const string &filenameBase) {

    // prepare files
    ifstream our(filenameOur);
    if (!our.is_open()) {
        cout << "Unable to open input file " << filenameOur << endl;
        return;
    }

    ifstream base(filenameBase);
    if (!base.is_open()) {
        cout << "Unable to open output file " << filenameBase << endl;
        return;
    }

    // read each result
    int our_sol, base_sol;
    float our_time, base_time;
    while (our >> our_sol >> our_time) {
        base >> base_sol >> base_time;

        cout << "Took " << our_time << " seconds, base time " << base_time << " seconds - ";

        if (our_sol != base_sol) {
            cout << "INVALID SOLUTION, expected " << base_sol << " but returned " << our_sol << endl;
        } else {
            cout << "VALID SOLUTION of " << base_sol << endl;
        }
    }
}

/**
 * MAIN
 */
int main(int argc, char *argv[]) {

#ifdef USING_DEBUG
    cout << "############################" << endl;
    cout << "# Using slow debug version #" << endl;
    cout << "############################" << endl;
#endif

    if (argc == 3) {
        // three arguments: execute file
        executeFile(argv[1], argv[2]);
        return 0;
    }

    if (argc == 2) {
        // two arguments: execute command

        // teacher prueba
        if (strcmp(argv[1], "prueba") == 0) {
            executeFile("prueba.txt", "sol_prueba.txt");
            compare("sol_prueba.txt", "teacher_sol_prueba.txt");
            return 0;
        }

        // test_500
        if (strcmp(argv[1], "500") == 0) {
            executeFile("test_500_15.txt", "sol_test_500_15.txt");
            return 0;
        }

        // several tests
        if (strcmp(argv[1], "range") == 0) {
            executeInternal();
            return 0;
        }

        cout << "Invalid command: " << argv[1] << endl;
    }

    // invalid usage
    cout << "Usage: " << argv[0] << " <input_file> <output_file>" << endl
         << "       " << argv[0] << "command" << endl;

    return 0;
}