#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int num = 0;

    while (num < 5) {
        if (rank == 0) {
            cout << "Process 0 sending " << num << " to process 1" << endl;
            MPI_Send(&num, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
            MPI_Recv(&num, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);  
            cout << "Process 0 received " << num << " from process 1" << endl;
            num++;
        } else if (rank == 1) {
            MPI_Recv(&num, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            cout << "Process 1 received " << num << " from process 0" << endl;
            num++;
            cout << "Process 1 sending " << num << " to process 0" << endl;
            MPI_Send(&num, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
