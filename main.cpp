//
// Created by Manju Muralidharan on 10/19/25.
//
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include "heap.h"
using namespace std;

// Global arrays for node information
const int MAX_NODES = 64;
int weightArr[MAX_NODES];
int leftArr[MAX_NODES];
int rightArr[MAX_NODES];
char charArr[MAX_NODES];

// Function prototypes
void buildFrequencyTable(int freq[], const string& filename);
int createLeafNodes(int freq[]);
int buildEncodingTree(int nextFree);
void generateCodes(int root, string codes[]);
void encodeMessage(const string& filename, string codes[]);

int main() {
    int freq[26] = {0};

    // Step 1: Read file and count letter frequencies
    buildFrequencyTable(freq, "input.txt");

    // Step 2: Create leaf nodes for each character with nonzero frequency
    int nextFree = createLeafNodes(freq);
    if (nextFree == 0) {
        cout << "No alphabetic characters found.\n";
        return 0;
    }

    // Step 3: Build encoding tree using your heap
    int root = buildEncodingTree(nextFree);

    // Step 4: Generate binary codes using an STL stack
    string codes[26];
    for (int i = 0; i < 26; ++i) codes[i].clear();
    generateCodes(root, codes);

    // Step 5: Encode the message and print output
    encodeMessage("input.txt", codes);

    return 0;
}

/*------------------------------------------------------
    Function Definitions (Students will complete logic)
  ------------------------------------------------------*/

// Step 1: Read file and count frequencies
void buildFrequencyTable(int freq[], const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    char ch;
    while (file.get(ch)) {
        // Convert uppercase to lowercase
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';

        // Count only lowercase letters
        if (ch >= 'a' && ch <= 'z')
            freq[ch - 'a']++;
    }
    file.close();

    cout << "Frequency table built successfully.\n";
}

// Step 2: Create leaf nodes for each character
int createLeafNodes(int freq[]) {
    int nextFree = 0;
    // Initialize all nodes to defaults
    for (int i = 0; i < MAX_NODES; ++i) {
        charArr[i] = '\0';
        weightArr[i] = 0;
        leftArr[i] = -1;
        rightArr[i] = -1;
    }

    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            charArr[nextFree]   = char('a' + i);
            weightArr[nextFree] = freq[i];
            leftArr[nextFree]   = -1;
            rightArr[nextFree]  = -1;
            nextFree++;
        }
    }
    cout << "Created " << nextFree << " leaf nodes.\n";
    return nextFree;
}

// Step 3: Build the encoding tree using heap operations
int buildEncodingTree(int nextFree) {
    // 1) Create a MinHeap
    MinHeap hp;

    // 2) Push all leaf indices
    for (int i = 0; i < nextFree; ++i) {
        hp.push(i, weightArr);
    }

    // Edge case: only one symbol
    if (hp.size == 1) {
        return hp.pop(weightArr);
    }

    int curr = nextFree; // next free index for internal nodes

    // 3) Combine nodes until one root remains
    while (hp.size > 1) {
        int a = hp.pop(weightArr);
        int b = hp.pop(weightArr);

        if (curr >= MAX_NODES) {
            cerr << "Error: node array overflow.\n";
            return a; // return something valid to avoid UB
        }

        // Create parent node
        charArr[curr]   = '\0'; // internal
        weightArr[curr] = weightArr[a] + weightArr[b];
        leftArr[curr]   = a;
        rightArr[curr]  = b;

        hp.push(curr, weightArr);
        curr++;
    }

    // 4) Return root index
    return hp.pop(weightArr);
}

// Step 4: Use an STL stack to generate codes
void generateCodes(int root, string codes[]) {
    if (root < 0) return;

    // Single-node case: only one character in input
    if (leftArr[root] == -1 && rightArr[root] == -1) {
        if (charArr[root] >= 'a' && charArr[root] <= 'z') {
            codes[charArr[root] - 'a'] = "0";
        }
        return;
    }

    // Iterative DFS using stack of (node, code)
    stack<pair<int, string>> st;
    st.push({root, ""});

    while (!st.empty()) {
        auto [u, code] = st.top();
        st.pop();

        int L = leftArr[u], R = rightArr[u];
        bool leaf = (L == -1 && R == -1);

        if (leaf) {
            // Leaf: record its code into codes[letter]
            if (charArr[u] >= 'a' && charArr[u] <= 'z') {
                // If code is empty (shouldn't happen here), use "0"
                codes[charArr[u] - 'a'] = (code.empty() ? "0" : code);
            }
        } else {
            // Push right first so left is processed first
            if (R != -1) st.push({R, code + "1"});
            if (L != -1) st.push({L, code + "0"});
        }
    }
}

// Step 5: Print table and encoded message
void encodeMessage(const string& filename, string codes[]) {
    cout << "\nCharacter : Code\n";
    for (int i = 0; i < 26; ++i) {
        if (!codes[i].empty())
            cout << char('a' + i) << " : " << codes[i] << "\n";
    }

    cout << "\nEncoded message:\n";

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
        return;
    }

    char ch;
    while (file.get(ch)) {
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';
        if (ch >= 'a' && ch <= 'z')
            cout << codes[ch - 'a'];
        // per spec: ignore non-letters
    }
    cout << "\n";
    file.close();
}
