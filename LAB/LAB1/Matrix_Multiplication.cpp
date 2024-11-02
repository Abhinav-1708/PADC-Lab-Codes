#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

vector<vector<int>> multiply_serial(const vector<vector<int>> &A, const vector<vector<int>> &B)
{
    int rowsA = A.size();
    int colsA = A[0].size();
    int rowsB = B.size();
    int colsB = B[0].size();

    vector<vector<int>> result(rowsA, vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < colsB; j++)
        {
            for (int k = 0; k < colsA; k++)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}
vector<vector<int>> multiply_Parallel(const vector<vector<int>> &A, const vector<vector<int>> &B)
{
    int rowsA = A.size();
    int colsA = A[0].size();
    int rowsB = B.size();
    int colsB = B[0].size();

    vector<vector<int>> result(rowsA, vector<int>(colsB, 0));

#pragma omp parallel for collapse(2)
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < colsB; j++)
        {
            for (int k = 0; k < colsA; k++)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

void createRandomMatrix(vector<vector<int>> &A, int size, int minVal = 0, int maxVal = 100)
{
    A.resize(size, vector<int>(size));
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            A[i][j] = minVal + rand() % (maxVal - minVal + 1);
        }
    }
}

int main()
{
    srand(time(0));

    vector<int> sizes = {10, 100, 500, 1000};
    double start_time, end_time;

    for (auto size : sizes)
    {
        vector<vector<int>> A, B;

        createRandomMatrix(A, size);
        createRandomMatrix(B, size);
        vector<vector<int>> C;
        start_time = omp_get_wtime();
        C = multiply_serial(A, B);
        end_time = omp_get_wtime();

        cout << size << " x " << size << " : Serial Execution Time : " << end_time - start_time << " seconds" << endl;

        start_time = omp_get_wtime();
        C = multiply_Parallel(A, B);
        end_time = omp_get_wtime();

        cout << size << " x " << size << " : Parallel Execution Time : " << end_time - start_time << " seconds" << endl;
    }

    return 0;
}
