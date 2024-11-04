#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
using namespace std;

// Merge function for Merge Sort
void merge(vector<pair<string, int>> &freq, int low, int mid, int high) {
    int n1 = mid - low + 1;
    int n2 = high - mid;

    vector<pair<string, int>> left(n1);
    vector<pair<string, int>> right(n2);

    for (int i = 0; i < n1; i++)
        left[i] = freq[low + i];
    for (int i = 0; i < n2; i++)
        right[i] = freq[mid + 1 + i];

    int i = 0, j = 0, k = low;

    // Merging the two halves in ascending order based on frequency
    while (i < n1 && j < n2) {
        if (left[i].second <= right[j].second) {
            freq[k++] = left[i++];
        } else {
            freq[k++] = right[j++];
        }
    }

    // Copy the remaining elements
    while (i < n1)
        freq[k++] = left[i++];
    while (j < n2)
        freq[k++] = right[j++];
}

// Merge Sort function
void merge_sort(vector<pair<string, int>> &freq, int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;

        merge_sort(freq, low, mid);
        merge_sort(freq, mid + 1, high);

        merge(freq, low, mid, high);
    }
}

int main() {
    ifstream file("abc.txt");
    if (!file) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    map<string, int> m;  // Map to store word frequencies
    vector<pair<string, int>> freq;  // Vector to store map entries for sorting

    string word;
    while (file >> word) {
        m[word] += 1;  // Increment the count for each word
    }

    // Transfer map entries to vector
    for (const auto &entry : m) {
        freq.push_back(make_pair(entry.first, entry.second));
    }

    // Perform Merge Sort on the vector by frequency
    merge_sort(freq, 0, freq.size() - 1);

    // Reverse to get descending order
    reverse(freq.begin(), freq.end());

    // Output top 10 sorted frequencies
    for (int i = 0; i < 10 && i < freq.size(); i++) {
        cout << freq[i].first << " " << freq[i].second << endl;
    }
    return 0;
}
