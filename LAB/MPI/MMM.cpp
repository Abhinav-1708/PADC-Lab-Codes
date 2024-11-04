#include <mpi.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 3; // Matrix dimension
    int mat[N][N] = {0};
    int mat2[N][N] = {0};
    int final[N][N] = {0};
    int local_buf[N]; // Buffer for each process to receive part of mat
    int local_res[N] = {0}; // Local result buffer

    if (rank == 0) {
        srand(time(0));

        // Initialize matrices
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                mat[i][j] = rand() % 10 + 1;
                mat2[i][j] = rand() % 10 + 1;
            }
        }

        // Print initial matrices
        cout << "Matrix 1 (mat):" << endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }

        cout << "Matrix 2 (mat2):" << endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << mat2[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Scatter rows of mat to all processes
    MPI_Scatter(mat, N, MPI_INT, local_buf, N, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Broadcast mat2 to all processes
    MPI_Bcast(mat2, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute local result
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            local_res[j] = 0; // Reset local result for each column
            for (int k = 0; k < N; k++) {
                local_res[j] += local_buf[k] * mat2[k][j];
            }
        }
    }

    // Gather results into final matrix
    MPI_Gather(local_res, N, MPI_INT, final, N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Print the final result
        cout << "Result Matrix (final):" << endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << final[i][j] << " ";
            }
            cout << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
