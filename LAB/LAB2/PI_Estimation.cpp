#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

double serial(long num_steps)
{
    double step = 1.0 / num_steps;
    double sum = 0.0;

    for (long i = 0; i < num_steps; i++)
    {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    return sum * step;
}

double parallel(long num_steps)
{
    double step = 1.0 / num_steps;
    double sum = 0.0;
    omp_set_num_threads(5);

#pragma omp parallel
    {
        double thread_sum = 0.0;
        int thread_num = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

#pragma omp for
        for (long i = 0; i < num_steps; i++)
        {
            double x = (i + 0.5) * step;
            thread_sum += 4.0 / (1.0 + x * x);
        }

#pragma omp critical
        {
            cout << "Thread Id " << thread_num << "\t Sum : " << thread_sum << endl;
            sum += thread_sum;
        }
    }
    return sum * step;
}

int main()
{
    long num_steps = 1000000;
    double start_time, end_time;

    start_time = omp_get_wtime();
    double pi_serial = serial(num_steps);
    end_time = omp_get_wtime();
    cout << "Serial Pi: " << pi_serial << endl;
    cout << "Serial Execution Time: " << (end_time - start_time) << " seconds\n";

    start_time = omp_get_wtime();
    double pi_parallel = parallel(num_steps);
    end_time = omp_get_wtime();
    cout << "Parallel Pi: " << pi_parallel << endl;
    cout << "Parallel Execution Time: " << (end_time - start_time) << " seconds\n";

    return 0;
}
