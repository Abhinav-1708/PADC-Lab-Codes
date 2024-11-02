#include<iostream>
#include<stdio.h>
#include<omp.h>
using namespace std;

/*
a) Write a simple C/C++ to print “Hello World”. 
b) Write an openMP program to print “Hello World” with parallel computation.  
*/

int main()
{
    // set number of threads to 6
    omp_set_num_threads(6);
    #pragma omp parallel
    {
        printf("Thread ID: %d, Hello World\n", omp_get_thread_num());
    }

    return 0;
}