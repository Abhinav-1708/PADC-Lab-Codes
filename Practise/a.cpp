#include <iostream>
#include <omp.h>
#include <stdio.h>
using namespace std;

int main()
{
    omp_set_num_threads(4);
#pragma omp parallel
    {
        int ID = omp_get_thread_num();
        cout << "HELLO WORLD " << ID << " "<< endl;
    }
    return 0;
}