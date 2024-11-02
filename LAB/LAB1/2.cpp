/*
Write an openMP to print “Hello World” for k times using for loop and compute the execution
time of sequential and parallel run.
*/

#define K 100

#include <iostream>
#include <stdio.h>
#include <omp.h>
using namespace std;

int main()
{
    double start_time, end_time;

    start_time = omp_get_wtime();
    for (int i = 0; i < K; i++)
    {
        cout << "Hello World";
    }
    end_time = omp_get_wtime();

    cout << "\n For Loop Serial Execution Time:\t" << end_time - start_time << endl;

    start_time = omp_get_wtime();
#pragma omp parallel for
    for (int i = 0; i < K; i++)
    {
        cout << "Hello World";
    }

    end_time = omp_get_wtime();
    cout << "\n For Loop Parallel Execution Time:\t" << end_time - start_time << endl;
    return 0;
}
