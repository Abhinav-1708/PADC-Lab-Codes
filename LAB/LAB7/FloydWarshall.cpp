#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

const int INF = 1e9;

void convertToAdjMatrix(map<int, vector<pair<int, int>>> &graph, vector<vector<int>> &adjMat, int V)
{
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (i == j)
                adjMat[i][j] = 0;
            else
                adjMat[i][j] = INF;
        }
    }
    for (auto &node : graph)
    {
        int u = node.first;
        for (auto &edge : node.second)
        {
            int v = edge.first, weight = edge.second;
            adjMat[u][v] = weight;
        }
    }
}

void floydWarshallSerial(vector<vector<int>> &adjMat, int V, double &serial_time)
{
    double start_time = omp_get_wtime();
    for (int k = 0; k < V; k++)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (adjMat[i][k] != INF && adjMat[k][j] != INF && adjMat[i][k] + adjMat[k][j] < adjMat[i][j])
                {
                    adjMat[i][j] = adjMat[i][k] + adjMat[k][j];
                }
            }
        }
    }
    double end_time = omp_get_wtime();
    serial_time = end_time - start_time;
    cout << "Serial execution time: " << serial_time << " seconds\n";
}

void floydWarshallParallel(vector<vector<int>> &adjMat, int V, double &parallel_time)
{
    double start_time = omp_get_wtime();
    for (int k = 0; k < V; k++)
    {
#pragma omp parallel for collapse(2)
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (adjMat[i][k] != INF && adjMat[k][j] != INF && adjMat[i][k] + adjMat[k][j] < adjMat[i][j])
                {
#pragma omp critical
                    {
                        adjMat[i][j] = adjMat[i][k] + adjMat[k][j];
                        printf("Thread %d processed update for i=%d, j=%d, k=%d\n", omp_get_thread_num(), i, j, k);
                    }
                }
            }
        }
    }
    double end_time = omp_get_wtime();
    parallel_time = end_time - start_time;
    cout << "Parallel execution time: " << parallel_time << " seconds\n";
}

void printAdjMatrix(const vector<vector<int>> &adjMat, int V)
{
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (adjMat[i][j] == INF)
                cout << "INF ";
            else
                cout << adjMat[i][j] << " ";
        }
        cout << "\n";
    }
}

int main()
{
    map<int, vector<pair<int, int>>> graph;
    int n, e;
    cout << "Enter number of nodes and edges: ";
    cin >> n >> e;
    cout << "Enter edges (source, destination, weight):\n";
    for (int i = 0; i < e; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
    }

    vector<vector<int>> adjMat(n, vector<int>(n, INF));
    convertToAdjMatrix(graph, adjMat, n);
    cout << "Original Adjacency Matrix:\n";
    printAdjMatrix(adjMat, n);

    double serial_time, parallel_time;
    vector<vector<int>> serialAdjMat = adjMat;
    floydWarshallSerial(serialAdjMat, n, serial_time);

    vector<vector<int>> parallelAdjMat = adjMat;
    floydWarshallParallel(parallelAdjMat, n, parallel_time);

    cout << "-------------------------------------\n";
    cout << "        Performance Comparison       \n";
    cout << "-------------------------------------\n";
    cout << "Serial Time     | Parallel Time \n";
    cout << serial_time << "     |     " << parallel_time << "\n";
    cout << "-------------------------------------\n";
    cout << "Updated Adjacency Matrix after Serial Execution:\n";
    printAdjMatrix(serialAdjMat, n);
    cout << "Updated Adjacency Matrix after Parallel Execution:\n";
    printAdjMatrix(parallelAdjMat, n);

    return 0;
}
