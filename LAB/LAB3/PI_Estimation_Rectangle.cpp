#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

double leftIntegration(int num_steps)
{
    double step = 1.0 / num_steps;
    double area = 0.0;
    omp_set_num_threads(4);
#pragma omp parallel
    {
        double thread_sum = 0.0;
        int thread_num = omp_get_thread_num();

#pragma omp for
        for (int i = 0; i < num_steps; i++)
        {
            double x = i * step;
            thread_sum += 4.0 / (1.0 + x * x);
        }
#pragma omp critical
        {
            cout << "Thread Id " << thread_num << "\t Sum : " << thread_sum << endl;
            area += thread_sum;
        }
    }
    return area * step;
}
double rightIntegration(int num_steps)
{
    double step = 1.0 / num_steps;
    double area = 0.0;
    omp_set_num_threads(4);
#pragma omp parallel
    {
        double thread_sum = 0.0;
        int thread_num = omp_get_thread_num();

#pragma omp for
        for (int i = 0; i < num_steps; i++)
        {
            double x = (i + 1) * step;
            thread_sum += 4.0 / (1.0 + x * x);
        }
#pragma omp critical
        {
            cout << "Thread Id " << thread_num << "\t Sum : " << thread_sum << endl;
            area += thread_sum;
        }
    }
    return area * step;
}
double midIntegration(int num_steps)
{
    double step = 1.0 / num_steps;
    double area = 0.0;
    omp_set_num_threads(4);
#pragma omp parallel
    {
        double thread_sum = 0.0;
        int thread_num = omp_get_thread_num();

#pragma omp for
        for (int i = 0; i < num_steps; i++)
        {
            double x = (i + 0.5) * step;
            thread_sum += 4.0 / (1.0 + x * x);
        }
#pragma omp critical
        {
            cout << "Thread Id " << thread_num << "\t Sum : " << thread_sum << endl;
            area += thread_sum;
        }
    }
    return area * step;
}

int main()
{
    long num_steps = 1000000;
    double start_time, end_time;

    start_time = omp_get_wtime();
    double pi_parallel = leftIntegration(num_steps);
    end_time = omp_get_wtime();
    cout << "Parallel Pi: (left Integration) " << pi_parallel << endl;
    cout << "Parallel Execution Time: " << (end_time - start_time) << " seconds\n";

    start_time = omp_get_wtime();
    pi_parallel = rightIntegration(num_steps);
    end_time = omp_get_wtime();
    cout << "Parallel Pi: (right Integration) " << pi_parallel << endl;
    cout << "Parallel Execution Time: " << (end_time - start_time) << " seconds\n";
    
    start_time = omp_get_wtime();
    pi_parallel = rightIntegration(num_steps);
    end_time = omp_get_wtime();
    cout << "Parallel Pi: (mid Integration) " << pi_parallel << endl;
    cout << "Parallel Execution Time: " << (end_time - start_time) << " seconds\n";

    return 0;
}