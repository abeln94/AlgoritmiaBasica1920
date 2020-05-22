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

using namespace std;

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
        // solution
        int localcost = traversedcost + dist(traversed.back(), 0);
        if (localcost < bestcost) {
            bestcost = localcost;
        }
        return;
    }

    // get next elements, sorted by closest first
    int last = traversed.back();
    pair<int, int> sort_available[available.size()];

    int i = 0;
    for (int &nextpos : available) {
        sort_available[i++] = make_pair(dist(last, nextpos), nextpos);
    }
    sort(sort_available, sort_available + available.size());

    // traverse elements
    for (auto &nextpos_ : sort_available) {
        int nextpos = nextpos_.second;

        int lastcost = dist(last, nextpos);

        traversed.push_back(nextpos);
        available.remove(nextpos);
        traversedcost += lastcost;
        if (traversedcost < bestcost)
            solve();
        traversedcost -= lastcost;
        available.push_back(nextpos);
        traversed.pop_back();
    }
}

void execute(const string &filenameIn) {
    // prepare files
    ifstream data(filenameIn);
    if (!data.is_open()) {
        cout << "Unable to open input file " << filenameIn << endl;
        return;
    }

    string filenameOut = "sol_" + filenameIn;
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

        // more initialize
        bestcost = INT32_MAX;

        traversed.clear();
        traversed.push_back(0);
        available.clear();
        for (int i = 1; i < positions_x.size(); ++i) available.push_back(i);
        traversedcost = 0;

        // execute
        float seconds = (float) Measure([] { solve(); }) / 1000.0f / 1000.0f;

        cout << n + 1 << '/' << N << " : " << bestcost << ' ' << seconds << endl;
        solution << bestcost << ' ' << seconds << endl;

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

int main() {

#ifdef USING_DEBUG
    cout << "############################" << endl;
    cout << "# Using slow debug version #" << endl;
    cout << "############################" << endl;
#endif

    execute("prueba.txt");

    compare("sol_prueba.txt", "teacher_sol_prueba.txt");

    execute("test_500_15.txt");

    return 0;
}