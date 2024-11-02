#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

void dijkstra_serial(const map<int, vector<vector<int>>> &g, int src, int tgt)
{
    double start_time = omp_get_wtime();

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    map<int, int> dist, par;

    for (const auto &node : g)
        dist[node.first] = INT_MAX;
    dist[src] = 0;

    pq.push({0, src});

    while (!pq.empty())
    {

        int cur = pq.top().second;
        pq.pop();

        if (cur == tgt)
            break;
        
        for (const auto &nb : g.at(cur))
        {
            int nxt = nb[0];
            int wt = nb[1];
            if (dist[cur] + wt < dist[nxt])
            {
                dist[nxt] = dist[cur] + wt;
                par[nxt] = cur;
                pq.push({dist[nxt], nxt});
            }
        }
    }

    double end_time = omp_get_wtime();
    double time_taken = end_time - start_time;

    if (dist[tgt] == INT_MAX)
    {
        cout << "No path from " << src << " to " << tgt << endl;
        return;
    }
    
    cout << "Serial: Shortest path from " << src << " to " << tgt << " dist " << dist[tgt] << ":\n";

    vector<int> path;
    for (int at = tgt; at != src; at = par[at])
        path.push_back(at);
    path.push_back(src);
    reverse(path.begin(), path.end());

    for (int i = 0; i < path.size(); i++)
    {
        cout << path[i];
        if (i != path.size() - 1)
            cout << " -> ";
    }
    cout << endl
         << "Serial took: " << time_taken << " seconds\n\n";
}

void dijkstra_parallel(const map<int, vector<vector<int>>> &g, int src, int tgt)
{
    double start_time = omp_get_wtime();
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    map<int, int> dist, par;
    for (const auto &node : g)
        dist[node.first] = INT_MAX;
    dist[src] = 0;
    pq.push({0, src});

    omp_lock_t lock;
    omp_init_lock(&lock);

#pragma omp parallel
    {
        while (true)
        {
            int cur = -1;
#pragma omp critical
            {
                if (!pq.empty())
                {
                    cur = pq.top().second;
                    pq.pop();
                }
            }
            if (cur == -1)
                break;
            if (cur == tgt)
                break;
#pragma omp parallel for shared(dist, par, pq) schedule(static)
            for (int i = 0; i < g.at(cur).size(); i++)
            {
                int nxt = g.at(cur)[i][0];
                int wt = g.at(cur)[i][1];
                omp_set_lock(&lock);
                if (dist[cur] + wt < dist[nxt])
                {
                    dist[nxt] = dist[cur] + wt;
                    par[nxt] = cur;
                    pq.push({dist[nxt], nxt});
                    printf("Thread %d processed edge from %d to %d with new dist %d\n",
                           omp_get_thread_num(), cur, nxt, dist[nxt]);
                }
                omp_unset_lock(&lock);
            }
        }
    }

    omp_destroy_lock(&lock);
    double end_time = omp_get_wtime();
    double time_taken = end_time - start_time;
    if (dist[tgt] == INT_MAX)
    {
        cout << "No path from " << src << " to " << tgt << endl;
        return;
    }
    cout << "Parallel: Shortest path from " << src << " to " << tgt << " dist " << dist[tgt] << ":\n";
    vector<int> path;
    for (int at = tgt; at != src; at = par[at])
        path.push_back(at);
    path.push_back(src);
    reverse(path.begin(), path.end());
    for (int i = 0; i < path.size(); i++)
    {
        cout << path[i];
        if (i != path.size() - 1)
            cout << " -> ";
    }
    cout << endl
         << "Parallel took: " << time_taken << " seconds\n\n";
}

int main()
{
    map<int, vector<vector<int>>> g;
    int n, e;


    cout << "Nodes and edges: ";
    cin >> n >> e;

    for (int i = 0; i < n; i++)
        g[i] = {};

    cout << "Enter edges (src, dest, weight):\n";
    for (int i = 0; i < e; i++)
    {
        int s, d, wt;
        cin >> s >> d >> wt;
        g[s].push_back({d, wt});
    }

    int src, tgt;
    cout << "Start and end nodes: ";
    cin >> src >> tgt;

    double time_serial, time_parallel;

    time_serial = omp_get_wtime();
    dijkstra_serial(g, src, tgt);
    time_serial = omp_get_wtime() - time_serial;

    time_parallel = omp_get_wtime();
    dijkstra_parallel(g, src, tgt);
    time_parallel = omp_get_wtime() - time_parallel;

    cout << "-------------------------------------\n";
    cout << "        Performance Comparison       \n";
    cout << "-------------------------------------\n";
    cout << "Serial Time     | Parallel Time \n";
    cout << time_serial << "         | " << time_parallel << "\n";
    cout << "-------------------------------------\n";
    return 0;
}
