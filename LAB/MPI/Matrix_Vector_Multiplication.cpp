#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m_rows = 4, m_cols = 4;
    int local_rows = m_rows / size; // Assumes m_rows is divisible by size
    int mat[local_rows][m_cols];
    int vec[m_cols];
    int res[local_rows] = {0};

    int full_matrix[m_rows][m_cols];
    if (rank == 0) {
        srand(time(0));

        // Initialize full matrix and vector on root process
        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                full_matrix[i][j] = rand() % 100 + 1;
            }
        }

        for (int i = 0; i < m_cols; i++) {
            vec[i] = rand() % 100 + 1;
        }
    }

    // Distribute rows to other processes
    MPI_Scatter(full_matrix, local_rows * m_cols, MPI_INT, mat, local_rows * m_cols, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vec, m_cols, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            res[i] += mat[i][j] * vec[j]; // Fixed index
        }
    }

    int final_result[m_rows];
    MPI_Gather(res, local_rows, MPI_INT, final_result, local_rows, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < m_rows; i++) {
            cout << final_result[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
