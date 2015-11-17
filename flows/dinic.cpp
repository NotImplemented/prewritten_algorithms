// Max-Flow Algorithms Comparison:
//  Dinic O(N2*M)
//  Edmonds-Karp O(N*M2)

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <ctime>
#include <queue>
#include <map>

using namespace std;
struct dinic
{
    struct edge
    {
        int u;
        int v;
        int flow;
        int capacity;
    };

    int N, s, t;

    std::vector<int> distance;
    std::vector<size_t> pointers;

    std::vector<edge> edges;
    std::vector<std::vector<int> > incident;

    dinic(const int N, int s, int t) : N(N), s(s), t(t), incident(N, std::vector<int>())
    {
    }

    dinic(const std::vector<std::vector<int> >& capacity, int s, int t) : N(capacity.size()), s(s), t(t), incident(N, std::vector<int>())
    {
        for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < N; ++j)
            {
                if (capacity[i][j])
                    add_edge(i, j, capacity[i][j]);
            }
        }
    }

    void add_edge(int u, int v, int capacity)
    {
        edge e_forward = {u, v, 0, capacity};
        edges.push_back(e_forward);
        incident[u].push_back(edges.size()-1);

        edge e_backward = {v, u, 0, 0};
        edges.push_back(e_backward);
        incident[v].push_back(edges.size()-1);
    }

    int max_flow()
    {
        int total_flow = 0;

        while(build_layered_network())
        {
            pointers.assign(N, 0);

            while(int f = augment(s, INT_MAX))
                total_flow += f;
        }

        return total_flow;
    }

private:

    bool build_layered_network()
    {
        distance.assign(N, -1);
        queue<int> q;

        distance[s] = 0;
        q.push(s);

        while(!q.empty())
        {
            int u = q.front(); q.pop();

            for(int i = 0; i < (int)incident[u].size(); ++i)
            {
                edge& e = edges[incident[u][i]];
                if (e.capacity - e.flow > 0 && distance[e.v] == -1)
                {
                    distance[e.v] = distance[u] + 1;
                    q.push(e.v);
                }
            }
        }

        return distance[t] != -1;
    }

    int augment(int u, int flow_restriction)
    {
        if (u == t)
            return flow_restriction;

        for(;pointers[u] < incident[u].size(); pointers[u]++)
        {
            int ptr = pointers[u];
            edge& e = edges[incident[u][ptr]];
            edge& e_reverse = edges[incident[u][ptr]^1];

            if (distance[u] + 1 != distance[e.v])
                continue;

            if (e.capacity - e.flow == 0)
                continue;

            if (int pushed = augment(e.v, std::min<int>(flow_restriction, e.capacity - e.flow )))
            {
                e.flow += pushed;
                e_reverse.flow -= pushed;

                return pushed;
            }
        }

        return 0;
    }
};
struct edmonds_karp
{
    int N, s, t;

    const std::vector<std::vector<int> >& capacity;
    std::vector<std::vector<int> > flow;
    std::vector<int> augmenting_path;

    edmonds_karp(const std::vector<std::vector<int> >& capacity, int s, int t) : capacity(capacity), N(capacity.size()), flow(N, std::vector<int>(N)), s(s), t(t)
    {

    }

    int max_flow()
    {
        int total_flow = 0;

        while(find_augmenting_path())
        {
            total_flow += augment();
        }

        return total_flow;
    }

private:

    bool find_augmenting_path()
    {
        std::vector<int> distance(N, -1);
        std::vector<int> from(N, -1);
        queue<int> q;

        distance[s] = 0;
        q.push(s);

        while(!q.empty())
        {
            int v = q.front(); q.pop();

            for(int i = 0; i < N; ++i)
            {
                if ( capacity[v][i] - flow[v][i] > 0 && distance[i] == -1)
                {
                    distance[i] = distance[v] + 1;
                    from[i] = v;
                    q.push(i);
                }
            }
        }

        if (distance[t] == -1)
        {
            return false;
        }

        augmenting_path.clear();

        int v = t;
        while(v != s)
        {
            augmenting_path.push_back(v);
            v = from[v];
        }

        augmenting_path.push_back(s);

        std::reverse(augmenting_path.begin(), augmenting_path.end());

        return true;
    }

    int augment()
    {
        int pushed_flow = INT_MAX;
        for(size_t i = 0; i < augmenting_path.size()-1; ++i)
        {
            int v = augmenting_path[i], u = augmenting_path[i+1];
            pushed_flow = std::min<int>(pushed_flow, capacity[v][u] - flow[v][u]);
        }

        for(size_t i = 0; i < augmenting_path.size()-1; ++i)
        {
            int v = augmenting_path[i], u = augmenting_path[i+1];
            flow[v][u] += pushed_flow;
            flow[u][v] -= pushed_flow;
        }

        return pushed_flow;
    }
};
struct test_result
{
    int dinic_result, edmonds_karp_result;
    double dinic_time, edmonds_karp_time;
};

test_result run_test(const int test, const int V, const int max_capacity, const double density)
{
    std::vector<std::vector<int> > capacity(V, std::vector<int>(V));

    for(int i = 0; i < V; ++i)
        for(int j = i + 1; j < V; ++j)
            if (rand() % 0xff > density * 0xff)
                capacity[i][j] = rand() % max_capacity;

    clock_t start_dinic = std::clock();

    dinic dinic_algorithm(capacity, 0, V-1);
    int dinic_max_flow = dinic_algorithm.max_flow();

    double dinic_time = ( std::clock() - start_dinic ) / (double)CLOCKS_PER_SEC;


    clock_t start_edmonds_karp = std::clock();

    edmonds_karp edmonds_karp_algorithm(capacity, 0, V-1);
    int edmonds_karp_max_flow = edmonds_karp_algorithm.max_flow();

    double edmonds_karp_time = ( std::clock() - start_edmonds_karp ) / (double)CLOCKS_PER_SEC;

    test_result result = {dinic_max_flow, edmonds_karp_max_flow, dinic_time, edmonds_karp_time};
    return result;
}

std::string format_time(const double time)
{
    char buf[0xff] = {0};
    sprintf(buf, "%5.3f", time);

    return std::string(buf);
}

std::string format_percentile(const double percentile)
{
    char buf[0xff] = {0};
    sprintf(buf, "%3d%%", (int)(percentile*100));

    return std::string(buf);
}

std::string format_test(const int test)
{
    char buf[0xff] = {0};
    sprintf(buf, "%2d%", test);

    return std::string(buf);
}

void run_scenario(const std::string& scenario_name, const int tests, const int V, const double density, const int max_capacity)
{
    std::cout << "Scenario: " << scenario_name << std::endl;
    std::cout << "Tests: " << tests << std::endl;
    std::cout << "Vertices: " << V << std::endl;
    std::cout << "Density: " << density << std::endl;
    std::cout << "Maximal Capacity: " << max_capacity << std::endl;
    std::cout << std::endl;

    int passed = 0, failed = 0;
    
    double max_dinic_time = 0, min_dinic_time = -1;
    double max_edmonds_karp_time = 0, min_edmonds_karp_time = -1;
    double total_dinic_time = 0, total_edmonds_karp_time = 0;

    double total_percentile = 0;
    double max_percentile = 0, min_percentile = -1;
    int max_percentile_test = -1, min_percentile_test = -1;

    for(int t = 0; t < tests; ++t)
    {
        test_result result = run_test(t+1, V, max_capacity, density);

        max_dinic_time = std::max (max_dinic_time, result.dinic_time);
        min_dinic_time = min_dinic_time != -1 ? std::min (min_dinic_time, result.dinic_time) : result.dinic_time;

        total_dinic_time += result.dinic_time;
        total_edmonds_karp_time += result.edmonds_karp_time;

        max_edmonds_karp_time = std::max (max_edmonds_karp_time, result.edmonds_karp_time);
        min_edmonds_karp_time = min_edmonds_karp_time != -1 ? std::min (min_edmonds_karp_time, result.edmonds_karp_time) : result.edmonds_karp_time;
        
        double percentile = result.dinic_time / result.edmonds_karp_time;
        if (percentile > max_percentile)
        {
            max_percentile = percentile;
            max_percentile_test = t+1;
        }

        if (percentile < min_percentile || min_percentile == -1)
        {
            min_percentile = percentile;
            min_percentile_test = t+1;
        }

        total_percentile += percentile;


        if (result.dinic_result == result.edmonds_karp_result)
        {
            std::cout << "Test Case #" << format_test(t+1) << ": PASSED!" << std::endl;
            passed++;
        }
        else
        {
            std::cout << "Test Case #" << format_test(t+1) << ": FAILED!" << std::endl;
            std::cout << "\t" << "Dinic: {" << result.dinic_result << "} Edmonds: {" << result.edmonds_karp_result << "}" << std::endl;
            failed++;
        }

        std::cout << "Dinic Time: " << format_time(result.dinic_time) << " " <<
            "Edmonds-Karp Time: " << format_time(result.edmonds_karp_time) << 
            " (" << int (percentile * 100) << "%)" << std::endl;

        std::cout << std::endl;
    }

    std::cout << "Test Cases Passed: " << passed << " / " << tests << " (" << (int)(passed * 100.0 / tests) << "%)" << std::endl;
    std::cout << "Test Cases Failed: " << failed << " / " << tests << " (" << (int)(failed * 100.0 / tests) << "%)" << std::endl;
    std::cout << std::endl;

    std::cout << "Dinic Min Time: " << format_time(min_dinic_time) << std::endl;
    std::cout << "Dinic Max Time: " << format_time(max_dinic_time) << std::endl;
    std::cout << "Dinic Avg Time: " << format_time(total_dinic_time / tests) << std::endl;
    std::cout << std::endl;

    std::cout << "Edmonds-Karp Min Time: " << format_time(min_edmonds_karp_time) << std::endl;
    std::cout << "Edmonds-Karp Max Time: " << format_time(max_edmonds_karp_time) << std::endl;
    std::cout << "Edmonds-Karp Avg Time: " << format_time(total_edmonds_karp_time / tests) << std::endl;
    std::cout << std::endl;

    std::cout << "Min Percentile:" << format_percentile(min_percentile) << " (Test Case #" << min_percentile_test << ")" << std::endl;
    std::cout << "Max Percentile:" << format_percentile(max_percentile) << " (Test Case #" << max_percentile_test << ")"<< std::endl;
    std::cout << std::endl;

    std::cout << "Avg Percentile:" << format_percentile(total_percentile / tests) << std::endl;
    std::cout << std::endl << std::endl;
}
int main(int argc, char* argv[])
{
    run_scenario("Dense Network", 16, 256, 0.8, 32);
    run_scenario("Sparse Network", 16, 256, 0.2, 32);

    return 0;
}