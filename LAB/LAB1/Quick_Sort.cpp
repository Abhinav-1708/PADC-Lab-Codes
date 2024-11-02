#include <iostream>
#include <stdio.h>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

void createRandomArray(vector<int> &A, int size, int minVal = 0, int maxVal = 100)
{
    A.resize(size);
    for (int i = 0; i < size; i++)
    {
        A[i] = minVal + rand() % (maxVal - minVal + 1);
    }
}

int partition(vector<int> &A, int low, int high)
{
    int pivot = A[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (A[j] <= pivot)
        {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[high]);
    return i + 1;
}

void quickSortSerialHelper(vector<int> &A, int low, int high)
{
    if (low < high)
    {
        int pi = partition(A, low, high);
        quickSortSerialHelper(A, low, pi - 1);
        quickSortSerialHelper(A, pi + 1, high);
    }
}

void quickSortSerial(vector<int> &A)
{
    quickSortSerialHelper(A, 0, A.size() - 1);
}

void quickSortParallelHelper(vector<int> &A, int low, int high)
{
    if (low < high)
    {
        int pi = partition(A, low, high);

#pragma omp parallel sections
        {
#pragma omp section
            quickSortParallelHelper(A, low, pi - 1);

#pragma omp section
            quickSortParallelHelper(A, pi + 1, high);
        }
    }
}

void quickSortParallel(vector<int> &A)
{
#pragma omp parallel
    {
#pragma omp single
        quickSortParallelHelper(A, 0, A.size() - 1);
    }
}

void print(vector<int> &A)
{
    for (int i = 0; i < A.size(); i++)
    {
        cout << A[i] << " ";
    }
    cout << endl;
}

int main()
{
    srand(time(0));
    vector<int> sizes = {10, 100, 1000, 10000};
    double start_time, end_time;

    for (auto size : sizes)
    {
        vector<int> A(size);

        // Serial Quick Sort
        createRandomArray(A, size);
        if (size == 10)
        {
            cout << "Unsorted Array\t:\t";
            print(A);
        }
        start_time = omp_get_wtime();
        quickSortSerial(A);
        end_time = omp_get_wtime();
        cout << size << " : Serial Execution Time : " << end_time - start_time << " seconds" << endl;
        if (size == 10)
        {
            cout << "Sorted Array\t:\t";
            print(A);
        }

        // Parallel Quick Sort
        createRandomArray(A, size);
        if (size == 10)
        {
            cout << "Unsorted Array\t:\t";
            print(A);
        }
        start_time = omp_get_wtime();
        quickSortParallel(A);
        end_time = omp_get_wtime();
        cout << size << " : Parallel Execution Time : " << end_time - start_time << " seconds" << endl;
        if (size == 10)
        {
            cout << "Sorted Array\t:\t";
            print(A);
        }
    }

    return 0;
}
