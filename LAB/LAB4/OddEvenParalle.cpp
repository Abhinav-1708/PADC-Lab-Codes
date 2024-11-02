#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// Function to generate random values in the array
void randomValues(vector<int> &temp, int size, int minVal = 0, int maxVal = 100) {
    temp.resize(size);
    for (int i = 0; i < size; i++) {
        temp[i] = minVal + rand() % (maxVal - minVal + 1);
    }
}

// Parallel Odd-Even Transposition Sort using OpenMP
void oeSortParallel(vector<int> &arr) {
    bool sorted = false;
    size_t n = arr.size();

    // Continue until the array is sorted
    while (!sorted) {
        sorted = true;

        // Perform the odd phase in parallel
        #pragma omp parallel for
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                #pragma omp critical
                {
                    sorted = false;
                }
            }
        }

        // Perform the even phase in parallel
        #pragma omp parallel for
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                #pragma omp critical
                {
                    sorted = false;
                }
            }
        }
    }
}

// Serial Odd-Even Transposition Sort
void oeSortSerial(vector<int> &temp) {
    bool sorted = false;
    size_t n = temp.size();
    while (!sorted) {
        sorted = true;
        // Odd phase
        for (int i = 1; i < n - 1; i += 2) {
            if (temp[i] > temp[i + 1]) {
                swap(temp[i], temp[i + 1]);
                sorted = false;
            }
        }
        // Even phase
        for (int i = 0; i < n - 1; i += 2) {
            if (temp[i] > temp[i + 1]) {
                swap(temp[i], temp[i + 1]);
                sorted = false;
            }
        }
    }
}

// Function to print the array
void print(const vector<int> &temp) {
    for (int i = 0; i < temp.size(); i++) {
        cout << temp[i] << " ";
    }
    cout << endl;
}
int main() {
    srand(static_cast<unsigned>(time(0))); // Seed for random number generation
    vector<int> sizes = {10, 100, 1000};
    double start_time, end_time;

    for (auto size : sizes) {
        cout << "\nSIZE: " << size << endl;
        vector<int> A;

        // Serial Execution
        randomValues(A, size);
        if (size == 10) {
            cout << "Unsorted Array:\t";
            print(A);
        }
        start_time = omp_get_wtime();
        oeSortSerial(A);
        end_time = omp_get_wtime();
        if (size == 10) {
            cout << "Sorted Array:\t";
            print(A);
        }
        cout << "Serial Execution Time:\t" << end_time - start_time << " seconds" << endl;

        // Parallel Execution
        randomValues(A, size);
        if (size == 10) {
            cout << "Unsorted Array:\t";
            print(A);
        }
        start_time = omp_get_wtime();
        oeSortParallel(A);
        end_time = omp_get_wtime();
        if (size == 10) {
            cout << "Sorted Array:\t";
            print(A);
        }
        cout << "Parallel Execution Time:\t" << end_time - start_time << " seconds" << endl;
    }

    return 0;
}
