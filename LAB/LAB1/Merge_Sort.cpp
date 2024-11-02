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

void merge(vector<int> &A, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> leftHalf(n1), rightHalf(n2);

    for (int i = 0; i < n1; i++)
        leftHalf[i] = A[left + i];
    for (int j = 0; j < n2; j++)
        rightHalf[j] = A[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (leftHalf[i] <= rightHalf[j])
            A[k++] = leftHalf[i++];
        else
            A[k++] = rightHalf[j++];
    }
    while (i < n1)
        A[k++] = leftHalf[i++];
    while (j < n2)
        A[k++] = rightHalf[j++];
}

void mergeSortSerialHelper(vector<int> &A, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSortSerialHelper(A, left, mid);
        mergeSortSerialHelper(A, mid + 1, right);
        merge(A, left, mid, right);
    }
}

void mergeSortSerial(vector<int> &A)
{
    mergeSortSerialHelper(A, 0, A.size() - 1);
}

void mergeSortParallelHelper(vector<int> &A, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            mergeSortParallelHelper(A, left, mid);

#pragma omp section
            mergeSortParallelHelper(A, mid + 1, right);
        }
        merge(A, left, mid, right);
    }
}

void mergeSortParallel(vector<int> &A)
{
#pragma omp parallel
    {
#pragma omp single
        mergeSortParallelHelper(A, 0, A.size() - 1);
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

        // Serial merge sort
        createRandomArray(A, size);
        start_time = omp_get_wtime();
        mergeSortSerial(A);
        end_time = omp_get_wtime();
        cout << size << " : Serial Execution Time : " << end_time - start_time << " seconds" << endl;

        if (size == 10)
        {
            cout << "Unsorted Array\t:\t";
            print(A);
        }

        // Parallel merge sort
        createRandomArray(A, size);
        start_time = omp_get_wtime();
        mergeSortParallel(A);
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
