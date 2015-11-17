// Minimum-Cost Maximum-Flow
// O(B*V*E*log(V)), where B is upper bound on the largest supply of any node
pair<int, int> min_cost_max_flow(const vector<vector<int> >& capacity, const vector<vector<int> >& cost, vector<vector<int> >& flow, const int src, const int dst)
{
    const int N = capacity.size();

    // Obtain potentials using Bellman-Ford algorithm
    vector<int> potential(N, INT_MAX);
    potential[src] = 0;
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            for(int k = j+1; k < N; ++k)
            {
                if (flow[j][k] < capacity[j][k] && potential[j] != INT_MAX && potential[j] + cost[j][k] < potential[k])
                {
                    potential[k] = potential[j] + cost[j][k];
                }
            }

            for(int k = 0; k < j; ++k)
            {
                if (flow[j][k] > 0 && potential[k] != INT_MAX && potential[k] - cost[j][k] < potential[j])
                {
                    potential[j] = potential[k] - cost[j][k];
                }
            }
        }
    }

    if (potential[dst] == INT_MAX)
        return make_pair(0, 0);

    while(true)
    {
        vector<int> distance(N, INT_MAX);
        vector<int> used(N);
        vector<int> predecessor(N, -1);

        // Find shortest path to destination using Dijkstra algorithm with potentials
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > q;
        q.push(make_pair(0, src));

        while(!q.empty())
        {
            int d = q.top().first;
            int v = q.top().second;

            q.pop();

            if (used[v])
                continue;

            if (v == dst)
                break;

            distance[v] = d;
            used[v] = true;

            for(int u = 0; u < N; ++u)
            {
                if (used[u])
                    continue;

                int c = INT_MAX;
                if (u > v && flow[v][u] < capacity[v][u] && potential[v] != INT_MAX && potential[u] != INT_MAX)
                {
                    c = cost[v][u] + potential[v] - potential[u];
                }
                if (v > u && flow[u][v] > 0 && potential[v] != INT_MAX && potential[u] != INT_MAX)
                {
                    c = -cost[u][v] + potential[v] - potential[u];
                }

                if (c != INT_MAX && distance[u] > distance[v] + c)
                {
                    predecessor[u] = v;
                    distance[u] = distance[v] + c;
                    q.push(make_pair(distance[u], u));
                }
            }
        }

        if (predecessor[dst] == -1)
            break;

        // Recalculate potentials
        for(int i = 0; i < N; ++i)
        {
            if (distance[i] != INT_MAX)
                potential[i] += distance[i];
        }

        // Find amount of flow to augment
        int flow_amount = INT_MAX;
        int p_curr = dst;
        int p_prev = predecessor[p_curr];

        while (p_prev != -1)
        {
            if (p_prev < p_curr)
            {
                if (capacity[p_prev][p_curr] != INT_MAX)
                    flow_amount = min(flow_amount, capacity[p_prev][p_curr] - flow[p_prev][p_curr]);
            }
            else
                flow_amount = min(flow_amount, flow[p_curr][p_prev]);

            p_curr = p_prev;
            p_prev = predecessor[p_prev];
        }


        // Augment flow
        p_curr = dst;
        p_prev = predecessor[p_curr];

        while (p_prev != -1)
        {
            if (p_prev < p_curr)
                flow[p_prev][p_curr] += flow_amount;
            else
                flow[p_curr][p_prev] -= flow_amount;

            p_curr = p_prev;
            p_prev = predecessor[p_prev];
        }
    }

    int result_flow = 0;
    for(int i = 0; i < N; ++i)
    {
        result_flow += flow[src][i];
    }

    int result_cost = 0;
    for(int i = 0; i < N; ++i)
    {
        for(int j = i+1; j < N; ++j)
        {
            result_cost += flow[i][j] * cost[i][j];
        }
    }


    return make_pair(result_cost, result_flow);
}