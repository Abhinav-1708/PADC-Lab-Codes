#include <iostream>
#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

double fullCircleParallel(long numPoints, double radius)
{
    long totalPointsInsideCircle = 0;
    double x, y;
    omp_set_num_threads(4);

// Parallel region with reduction for total count inside circle
#pragma omp parallel reduction(+ : totalPointsInsideCircle)
    {
        int threadNum = omp_get_thread_num();
        long pointsInsideCircle = 0;

// Use each thread to handle a subset of points
#pragma omp for private(x, y)
        for (long i = 0; i < numPoints; i++)
        {
            // Generate random numbers between [-radius, +radius]
            x = (static_cast<double>(rand()) / RAND_MAX) * (2 * radius) - radius;
            y = (static_cast<double>(rand()) / RAND_MAX) * (2 * radius) - radius;

            // Count if the point lies within the circle
            if (x * x + y * y <= radius * radius)
                pointsInsideCircle++;
        }

        // Print area calculated by each thread
        cout << "Thread " << threadNum << " calculated area portion: "
             << static_cast<double>(pointsInsideCircle) / numPoints * 4.0 << endl;

        // Update total points inside circle
        totalPointsInsideCircle += pointsInsideCircle;
    }

    return 4.0 * (static_cast<double>(totalPointsInsideCircle) / numPoints);
}

double quarterCircleParallel(long numPoints, double radius)
{
    long totalPointsInsideQuarterCircle = 0;
    double x, y;

// Parallel region with reduction for total count inside quarter circle
#pragma omp parallel reduction(+ : totalPointsInsideQuarterCircle)
    {
        int threadNum = omp_get_thread_num();
        long pointsInsideQuarterCircle = 0;

// Use each thread to handle a subset of points
#pragma omp for private(x, y)
        for (long i = 0; i < numPoints; i++)
        {
            // Generate random numbers within unit square
            x = (static_cast<double>(rand()) / RAND_MAX);
            y = (static_cast<double>(rand()) / RAND_MAX);

            // Count if the point lies within the quarter circle
            if (x * x + y * y <= 1.0)
                pointsInsideQuarterCircle++;
        }

        // Print area calculated by each thread
        cout << "Thread " << threadNum << " calculated area portion: "
             << static_cast<double>(pointsInsideQuarterCircle) / numPoints * 4.0 << endl;

        // Update total points inside quarter circle
        totalPointsInsideQuarterCircle += pointsInsideQuarterCircle;
    }

    return 4.0 * (static_cast<double>(totalPointsInsideQuarterCircle) / numPoints);
}

int main()
{
    long numPoints = 1000000;
    double radius = 100.0;
    srand(static_cast<unsigned>(time(0)));

    double start_time, end_time;

    // Full Circle Pi Estimation (Parallel)
    start_time = omp_get_wtime();
    double piFullCircle = fullCircleParallel(numPoints, radius);
    end_time = omp_get_wtime();
    cout << "Case 1 - Full Circle Pi Estimate: " << piFullCircle << endl;
    cout << "Execution Time (Full Circle, Parallel): " << (end_time - start_time) << " seconds" << endl;

    // Quarter Circle Pi Estimation (Parallel)
    start_time = omp_get_wtime();
    double piQuarterCircle = quarterCircleParallel(numPoints, radius);
    end_time = omp_get_wtime();
    cout << "Case 2 - Quarter Circle Pi Estimate: " << piQuarterCircle << endl;
    cout << "Execution Time (Quarter Circle, Parallel): " << (end_time - start_time) << " seconds" << endl;

    return 0;
}
