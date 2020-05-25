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


int N;
vector<int> positions_x, positions_y;
list<int> available, traversed;
int bestcost, traversedcost;

/**
 * Distance between two positions
 */
int dist(int from, int to) {
    return abs(positions_x[from] - positions_x[to]) + abs(positions_y[from] - positions_y[to]);
}

/**
 * Solve step
 */
void solve() {
    if (available.empty()) {
        // solution, update best cost
        int localcost = traversedcost + dist(traversed.back(), 0);
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
        sort_available[i++] = make_pair(dist(last, nextpos) + dist(nextpos, 0), nextpos);
    }

    // sort by closest
    sort(sort_available, sort_available + i);

    // traverse elements
    for (auto &nextpos_ : sort_available) {

        // early cut: if worst distance is less than best, cut all remaining
        // repeat each time because bestcost may be updated
        // no need for another local cut
        if (traversedcost + sort_available[i - 1].first >= bestcost) break;

        int nextpos = nextpos_.second;

        int lastcost = dist(last, nextpos);


        // move as traversed
        traversed.push_back(nextpos);
        available.remove(nextpos);
        traversedcost += lastcost;

        // solve
        solve();

        // restore
        available.push_back(nextpos);
        traversed.pop_back();
        traversedcost -= lastcost;
    }
}

void prepare() {
    bestcost = INT32_MAX;

    traversed.clear();
    traversed.push_back(0);
    available.clear();
    for (int i = 1; i < positions_x.size(); ++i) available.push_back(i);
    traversedcost = 0;
}

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
    data >> N;

    for (int n = 0; n < N; ++n) {
        int pos_x, pos_y;
        // foreach scenario

        // read elements
        int A, B, M;
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

        prepare();

        // execute
        float seconds = (float) Measure([] { solve(); });

        cout << n + 1 << '/' << N << " : " << bestcost << ' ' << seconds << endl;
        solution << bestcost << ' ' << seconds << endl;

    }
}

void executeInternal() {

    // fixed length (it is not used per se)
    N = 20;

    // initialize an array with all possible positions, for random pick
    int values[N * N];
    for (int n = 0; n < N; ++n) values[n] = n;

    // execute for different number of positions
    for (int n = 2; n < 20; ++n) {

        // initialize n data
        float seconds = 0;
        int TESTS = 10;

        // execute several tests (for average)
        for (int t = 0; t < TESTS; ++t) {

            // initialize
            positions_x.clear();
            positions_y.clear();

            // add n random positions
            shuffle(values, values + (N * N), default_random_engine(chrono::system_clock::now().time_since_epoch().count()));
            for (int p = 0; p < n; ++p) {
                positions_x.push_back(values[p] / N);
                positions_y.push_back(values[p] % N);
            }

            prepare();

            // execute
            seconds += (float) Measure([] { solve(); });

        }
        cout << "n=" << n << " seconds=" << seconds / (float) TESTS << endl;
    }
}

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

int main(int argc, char *argv[]) {

#ifdef USING_DEBUG
    cout << "############################" << endl;
    cout << "# Using slow debug version #" << endl;
    cout << "############################" << endl;
#endif

    if (argc == 3) {
        executeFile(argv[1], argv[2]);
        return 0;
    }

    if (argc == 2 && strcmp(argv[1], "prueba") == 0) {
        // test
        executeFile("prueba.txt", "sol_prueba.txt");
        compare("sol_prueba.txt", "teacher_sol_prueba.txt");
        return 0;
    }

    if (argc == 2 && strcmp(argv[1], "500") == 0) {
        // test_500
        executeFile("test_500_15.txt", "sol_test_500_15.txt");
        return 0;
    }

    if (argc == 2 && strcmp(argv[1], "range") == 0) {
        // several tests
        executeInternal();
        return 0;
    }

    cout << "Usage: " << argv[0] << " <input_file> <output_file>" << endl
         << "       " << argv[0] << "testname" << endl;


    return 0;
}