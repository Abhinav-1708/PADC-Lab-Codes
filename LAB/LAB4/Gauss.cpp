#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

void generateRandomValues(vector<vector<double>> &A, vector<double> &Coeff, int n, int minVal = 1, int maxVal = 100)
{
    srand(time(0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = minVal + rand() % (maxVal - minVal + 1);
        }
        Coeff[i] = minVal + rand() % (maxVal - minVal + 1);
    }
}

void printMatrix(const vector<vector<double>> &A)
{
    for (const auto &row : A)
    {
        for (const auto &elem : row)
        {
            cout << setw(10) << fixed << setprecision(2) << elem << " ";
        }
        cout << endl;
    }
}

void printVector(const vector<double> &v)
{
    for (const auto &elem : v)
    {
        cout << setw(10) << fixed << setprecision(2) << elem << " ";
    }
    cout << endl;
}

// Function to perform Gaussian elimination in parallel
vector<double> Gauss(vector<vector<double>> &A, vector<double> &Coeff, int n, double &execution_time)
{
    vector<double> x(n);
    double start_time, end_time;

    start_time = omp_get_wtime();

    // Forward elimination to make A upper triangular
    for (int i = 0; i < n - 1; i++)
    {
        omp_set_num_threads(5);
#pragma omp parallel for
        for (int j = i + 1; j < n; j++)
        {
            int thread_id = omp_get_thread_num();  
            cout << "Thread " << thread_id << " is working on row " << j << " for pivot row " << i << endl;

            double multiplier = A[j][i] / A[i][i];
            for (int k = i; k < n; k++)
            {
                A[j][k] -= multiplier * A[i][k];
            }
            Coeff[j] -= multiplier * Coeff[i];
        }
    }

    // Back substitution to solve for x
    for (int i = n - 1; i >= 0; i--)
    {
        double sum = Coeff[i];
        for (int j = i + 1; j < n; j++)
        {
            sum -= A[i][j] * x[j];
        }
        x[i] = sum / A[i][i];
    }

    end_time = omp_get_wtime();
    execution_time = end_time - start_time;

    return x;
}

int main()
{
    vector<int> sizes = {2, 3, 5, 10};  

    for (int size : sizes)
    {
        int n = size;
        vector<vector<double>> A(n, vector<double>(n));
        vector<double> Coeff(n);

        // Initialize matrix A and vector Coeff with random values
        generateRandomValues(A, Coeff, n);
        cout << "Matrix A : \n";
        printMatrix(A);
        double execution_time = 0.0;
        vector<double> x = Gauss(A, Coeff, n, execution_time);

        // Print the upper triangular matrix after Gaussian elimination
        cout << "Matrix Size: " << n << endl;
        cout << "Upper Triangular Matrix A after Gaussian Elimination:\n";
        printMatrix(A);

        cout << "\nVector Coeff:\n";
        printVector(Coeff);
        // Print vector b (Coefficients)
        cout << "\nVector Coeff after Gaussian Elimination:\n";
        printVector(Coeff);

        // Print the solution vector x
        cout << "\nSolution Vector x:\n";
        printVector(x);

        // Print execution time
        cout << "\nTime Taken: " << execution_time << " seconds\n";
        cout << "---------------------------------------------------------------\n";
    }

    return 0;
}
