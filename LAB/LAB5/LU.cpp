#include <iostream>
#include <vector>
#include <omp.h>
#include <atomic>
#include <ctime>
#include <random> // for random number generation

using namespace std;

// Function to perform LU Decomposition
void LU_Decomposition(const vector<vector<double>> &A,vector<vector<double>> &L, vector<vector<double>> &U, int n)
{
    vector<atomic<int>> thread_work(omp_get_max_threads());

#pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            L[i][j] = (i == j) ? 1.0 : 0.0;
            U[i][j] = A[i][j];
            thread_work[omp_get_thread_num()]++;
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (U[i][i] == 0)
        {
#pragma omp critical
            {
                cerr << "Error: Zero pivot encountered at row " << i << "." << endl;
            }
            return;
        }

#pragma omp parallel for
        for (int j = i + 1; j < n; j++)
        {
            double factor = U[j][i] / U[i][i];
            L[j][i] = factor;
            for (int k = i; k < n; k++)
            {
                U[j][k] -= factor * U[i][k];
            }
            thread_work[omp_get_thread_num()]++;
        }
    }

    cout << "LU_Decomposition thread work:" << endl;
    for (int i = 0; i < omp_get_max_threads(); i++)
    {
        cout << "Thread " << i << ": " << thread_work[i] << " operations" << endl;
    }
}

vector<double> Gauss(vector<vector<double>> &L, vector<double> &b, int n)
{
    vector<atomic<int>> thread_work(omp_get_max_threads());
    vector<double> y(n);

    for (int i = 0; i < n; i++)
    {
        double sum = b[i];
#pragma omp parallel for reduction(- : sum)
        for (int j = 0; j < i; j++)
        {
            sum -= L[i][j] * y[j];
            thread_work[omp_get_thread_num()]++;
        }
        y[i] = sum / L[i][i];
    }

    cout << "Gauss thread work:" << endl;
    for (int i = 0; i < omp_get_max_threads(); i++)
    {
        cout << "Thread " << i << ": " << thread_work[i] << " operations" << endl;
    }
    return y;
}

void printMatrix(const vector<vector<double>> &matrix)
{
    for (const auto &row : matrix)
    {
        for (const auto &val : row)
        {
            cout << val << " ";
        }
        cout << endl;
    }
}

int main()
{
    int n;
    cout << "Enter the size of matrix A (n x n): ";
    cin >> n;

    vector<vector<double>> A(n, vector<double>(n));
    vector<vector<double>> L(n, vector<double>(n));
    vector<vector<double>> U(n, vector<double>(n));
    vector<double> b(n), y(n), x(n);

    // Random number generation using Mersenne Twister
    std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    std::uniform_real_distribution<double> dist(0.0, 99.0);

    // Generate random values for matrix A
    cout << "Randomly generated matrix A (" << n << "x" << n << "):" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = dist(rng);
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    // Generate random values for vector b
    cout << "Randomly generated vector b (" << n << "x 1):" << endl;
    for (int i = 0; i < n; i++)
    {
        b[i] = dist(rng);
        cout << b[i] << " ";
    }
    cout << endl;

    double start = omp_get_wtime();
    LU_Decomposition(A, L, U, n);
    y = Gauss(L, b, n);
    x = Gauss(U, y, n);
    double end = omp_get_wtime();

    cout << "\nMatrix L:" << endl;
    printMatrix(L);
    cout << "\nMatrix U:" << endl;
    printMatrix(U);
    cout << endl;

    cout << "The solution matrix x is: " << endl;
    for (auto e : x)
    {
        cout << e << " ";
    }
    cout << endl;

    cout << "Time Taken: " << (end - start) << " seconds" << endl;
    return 0;
}
