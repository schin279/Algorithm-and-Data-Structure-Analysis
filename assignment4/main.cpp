#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
using namespace std;

// disjoint set data structure to maintain the connected components
struct DS {
    vector<int> parent;
    vector<int> rank;

    DS(int size) {
        parent.resize(size);
        rank.resize(size, 1);

        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
    }
    // function to find the root of a set in the disjoint set
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }
    // function to union two sets in the disjoint set based on rank
    void unionSet(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) {
            return; // if they are already in the same set, no need to union
        }
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootY] < rank[rootX]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    } 
};

// structure to represent roads
struct Road {
    int from, to, cost;
    Road(int f, int t, int c) : from(f), to(t), cost(c) {}
};

// function to compare roads by cost in descending order
bool is_higher(const Road &a, const Road &b) {
    return a.cost > b.cost;
}
// function to compare roads by cost in ascending order
bool is_lower(const Road &a, const Road &b) {
    return a.cost < b.cost;
}

class RedesignRoadSystem {
public:
   // function to calculate the cost represented by English letters
    int calcCost(char l) {
        if ('A' <= l && l <= 'Z') {
            return l - 'A'; // 'A' to 'Z' mapped to 0 to 25
        } else {
            return l - 'a' + 26; // 'a' to 'z' mapped to 26 to 51
        }
    }
    // function to calculate the minimum cost for reconstruction
    int minCost(string c, string b, string d) {
        // split the input strings into vectors
        vector<string> country = splitString(c, ",");
        vector<string> build = splitString(b, ",");
        vector<string> destroy = splitString(d, ",");

        vector<Road> r1, r2; // vector to store roads for building and destroying
        int N = country.size(); // the number of cities in a country

        // map roads based on their paths and associated costs
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < i; j++) {
                // determine the cost based on whether the road is built or destroyed
                if (country[i][j] == '1') {
                    r1.push_back(Road(i, j, calcCost(destroy[i][j]))); // add roads to the destruction vector
                } else {
                    r2.push_back(Road(i, j, calcCost(build[i][j]))); // add roads to the construction vector
                }
            }
        }
        
        DS set(N);
        vector<Road>::iterator ii;
        int cost = 0; // total cost
        // sort the roads by cost in descending order
        sort(r1.begin(), r1.end(), is_higher);
        for (ii = r1.begin(); ii != r1.end(); ii++) {
            // check if including this road doesn't create a cycle
            if (set.find(ii->from) != set.find(ii->to)) {
                set.unionSet(ii->from, ii->to); // union the sets of 'from' and 'to'
            } else {
                cost += ii->cost; // add the road's cost to the total cost
            }
        }
        // sort the roads by cost in ascending order
        sort(r2.begin(), r2.end(), is_lower);
        for (ii = r2.begin(); ii != r2.end(); ii++) {
            // check if including this road doesn't create a cycle
            if (set.find(ii->from) != set.find(ii->to)) {
                set.unionSet(ii->from, ii->to); // union the sets of 'from' and 'to'
                cost += ii->cost; // add the road's cost to the total cost
            }
        }
        return cost; // return the minimum cost
    }

private:
    // function to split a string based on a delimiter
    vector<string> splitString(const string &input, const string &delimiter) {
        vector<string> result; // vector to store the substrings
        size_t pos = 0;
        size_t prev = 0;
        // split the input string using the specified delimiter
        while ((pos = input.find(delimiter, prev)) != string::npos) {
            result.push_back(input.substr(prev, pos - prev));
            prev = pos + delimiter.length();
        }
        result.push_back(input.substr(prev)); // add the last substring
        return result; // return the vector of substrings
    }
};

int main() {
    RedesignRoadSystem rr;

    string input;
    getline(cin, input);

    istringstream iss(input);
    string country, build, destroy;
    iss >> country >> build >> destroy;

    int cost = rr.minCost(country, build, destroy); // calculate the minimum cost
    cout << cost << endl;
}