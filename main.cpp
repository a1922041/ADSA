#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
using namespace std;


//structure to represent an edge between two cities (u and v) with cost
struct Edge {
    int u, v;
    int cost;
};


// detect cycles when building the Minimum Spanning Tree 
struct DSU {
    vector<int> p;  //keep track of connected components

    // Constructor , each city is its own parent (separate component)
    DSU(int n) : p(n) { iota(p.begin(), p.end(), 0); }

    // Find operation with path compression
    // Finds the "root" of a node's set
    int find(int x) {
        return p[x] == x ? x : p[x] = find(p[x]);
    }

    // Union operation:
    // Merges the sets that contain nodes a and b.
    // Returns true if merged successfully, false if already connected.
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false; // already in the same set
        p[b] = a; // merge sets
        return true;
    }
};

// Converts character cost (A–Z, a–z) into a number (0–51)
int charCost(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';      // A=0, B=1, ..., Z=25
    return c - 'a' + 26;                           // a=26, b=27, ..., z=51
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);


    string countryInput, buildInput, destroyInput;
    cin >> countryInput >> buildInput >> destroyInput;


    auto split = [](string s) {
        vector<string> parts;
        string temp;
        for (char c : s) {
            if (c == ',') {
                parts.push_back(temp);
                temp.clear();
            } else temp.push_back(c);
        }
        if (!temp.empty()) parts.push_back(temp);
        return parts;
    };


    vector<string> country = split(countryInput);
    vector<string> build = split(buildInput);
    vector<string> destroy = split(destroyInput);
    int N = country.size();  // number of cities

    long long totalDestroy = 0; // sum of all destruction costs (initially assume destroy all roads)
    vector<Edge> edges;         // store all possible edges (existing + possible builds)

    // Loop through all city pairs (i, j), only once (since the graph is undirected)
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (country[i][j] == '1') {
                // Road already exists → we may destroy it later if not needed
                int d = charCost(destroy[i][j]);
                totalDestroy += d;           // assume we pay to destroy all roads
                edges.push_back({i, j, -d}); // keeping the road "saves" destruction cost (negative cost)
            } else {
                // Road does not exist → we may build it later if needed
                int b = charCost(build[i][j]);
                edges.push_back({i, j, b});  // cost to build a new road
            }
        }
    }

    // sort edges by cost 
    // negative costs (savings) come first, then small build costs, then large build costs
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.cost < b.cost;
    });

    // Initialize Disjoint Set Union for N cities
    DSU dsu(N);
    long long mstCost = 0; // total cost for chosen edges (MST)

    // Kruskal's algorithm: build MST by adding the cheapest edges that don't form cycles
    for (auto &e : edges) {
        if (dsu.unite(e.u, e.v)) { // if citiesnot yet connected
            mstCost += e.cost;     // edge's cost (or saving)
        }
    }

    // Final total cost:
    long long result = totalDestroy + mstCost;


    cout << result << "\n";
    return 0;
}
