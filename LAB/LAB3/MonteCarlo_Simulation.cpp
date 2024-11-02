#include <iostream>
#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

double fullCircle(long numPoints, double radius)
{
    long pointInsideCircle = 0;
    double x, y;

    for (long i = 0; i < numPoints; i++)
    {
        // Generate Random Numbers between [-radius, +radius]
        x = (static_cast<double>(rand()) / RAND_MAX) * (2 * radius) - radius;
        y = (static_cast<double>(rand()) / RAND_MAX) * (2 * radius) - radius;

        // If this randomly generated point lies within circle increase count
        if (x * x + y * y <= radius * radius)
            pointInsideCircle++;
    }
    return 4.0 * (static_cast<double>(pointInsideCircle) / numPoints);
}
double quarterCircle(long numPoints, double radius)
{
    long pointInsideQuarterCircle = 0;
    double x, y;

    for (long i = 0; i < numPoints; i++)
    {
        // Generate Random Numbers within unit square
        x = (static_cast<double>(rand()) / RAND_MAX);
        y = (static_cast<double>(rand()) / RAND_MAX);

        // If this randomly generated point lies within  unit circle increase count
        if (x * x + y * y <= 1.0)
            pointInsideQuarterCircle++;
    }
    return 4.0 * (static_cast<double>(pointInsideQuarterCircle) / numPoints);
}

int main()
{
    long numPoints = 1000000;
    double radius = 100.0;
    srand(static_cast<unsigned>(time(0)));

    double start_time, end_time;

    start_time = omp_get_wtime();
    double piFullCircle = fullCircle(numPoints, radius);
    end_time = omp_get_wtime();
    cout << "Case 1 - Full Circle Pi Estimate: " << piFullCircle << endl;
    cout << "Execution Time (Full Circle): " << (end_time - start_time) << " seconds" << endl;
    start_time = omp_get_wtime();

    double piQuarterCircle = quarterCircle(numPoints, radius);
    end_time = omp_get_wtime();
    cout << "Case 2 - Quarter Circle Pi Estimate: " << piQuarterCircle << endl;
    cout << "Execution Time (Quarter Circle): " << (end_time - start_time) << " seconds" << endl;
    return 0;
}