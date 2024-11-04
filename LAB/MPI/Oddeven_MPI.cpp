#include <iostream>
#include <mpi.h>
#include <vector>
#include <algorithm> // for std::sort
#include <cstdlib>
#include <ctime>
using namespace std;

int IncOrder(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void CompareSplit(int local_ele, vector<int> &local_arr, vector<int> &rec_arr, vector<int> &workspace, int keepsmall)
{
    for (int i = 0; i < local_ele; i++)
    {
        workspace[i] = local_arr[i];
        workspace[local_ele + i] = rec_arr[i];
    }

    sort(workspace.begin(), workspace.end());

    if (keepsmall)
    {
        for (int i = 0; i < local_ele; i++)
        {
            local_arr[i] = workspace[i];
        }
    }
    else
    {
        for (int i = 0; i < local_ele; i++)
        {
            local_arr[i] = workspace[local_ele + i];
        }
    }
}

int main(int argc, char **argv)
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(0) + rank);
    MPI_Status status;

    int n = 16;
    int local_ele = n / size;

    vector<int> arr(n);
    vector<int> local_arr(local_ele);
    vector<int> rec_arr(local_ele);
    vector<int> workspace(2 * local_ele, 0);

    if (rank == 0)
    {
        cout << "Initial array: ";
        for (int i = 0; i < n; i++)
        {
            arr[i] = rand() % 100 + 1;
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    MPI_Scatter(arr.data(), local_ele, MPI_INT, local_arr.data(), local_ele, MPI_INT, 0, MPI_COMM_WORLD);

    sort(local_arr.begin(), local_arr.end());

    int oddrank = (rank % 2 == 0) ? rank - 1 : rank + 1;
    int evenrank = (rank % 2 == 0) ? rank + 1 : rank - 1;

    if (oddrank < 0 || oddrank >= size)
        oddrank = MPI_PROC_NULL;
    if (evenrank < 0 || evenrank >= size)
        evenrank = MPI_PROC_NULL;

    for (int i = 0; i < size; i++)
    {
        if (i % 2 == 1)
        {
            MPI_Sendrecv(local_arr.data(), local_ele, MPI_INT, oddrank, 1,
                         rec_arr.data(), local_ele, MPI_INT, oddrank, 1,
                         MPI_COMM_WORLD, &status);
        }
        else
        {
            MPI_Sendrecv(local_arr.data(), local_ele, MPI_INT, evenrank, 1,
                         rec_arr.data(), local_ele, MPI_INT, evenrank, 1,
                         MPI_COMM_WORLD, &status);
        }

        if (status.MPI_SOURCE != MPI_PROC_NULL)
        {
            CompareSplit(local_ele, local_arr, rec_arr, workspace, rank < status.MPI_SOURCE);
        }
    }

    vector<int> final_sorted_elements;
    if (rank == 0)
    {
        final_sorted_elements.resize(n);
    }

    MPI_Gather(local_arr.data(), local_ele, MPI_INT, final_sorted_elements.data(), local_ele, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        cout << "Sorted array: ";
        for (int i = 0; i < n; i++)
        {
            cout << final_sorted_elements[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
