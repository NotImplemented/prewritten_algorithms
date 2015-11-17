// Maximal matching in bipartite graph
// Kuhn Algorithm O(V*E)
#include <queue>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <functional>

struct max_bipartite_matching
{
    std::vector<int> visited;
    std::vector<int> matched_to;
    std::vector<int> matched_from;

    std::vector<std::vector<int> > G;
    int A, B;


    max_bipartite_matching(const std::vector<std::vector<int> >& G) : G(G), A(G.size()), B(G[0].size())
    {
    }

    int calculate_dfs()
    {
        int max_matching_size = 0;

        matched_from.assign(B, -1);
        matched_to.assign(A, -1);

        // find initial matching
        for(int i = 0; i < A; ++i)
        {
            for(int j = 0; j < B; ++j)
            {
                if (G[i][j] && matched_from[j] == -1)
                {
                    matched_from[j] = i;
                    matched_to[i] = j;
                    max_matching_size++;
                    break;
                }
            }
        }


        for(int i = 0; i < A; ++i)
        {
            visited.assign(A, false);

            if (matched_to[i] == -1 && augment(i))
            {
                max_matching_size++;
            }

        }

        return max_matching_size;
    }

    int calculate_bfs()
    {
        int max_matching_size = 0;

        matched_from.assign(B, -1);
        matched_to.assign(A, -1);

        // find initial matching
        for(int i = 0; i < A; ++i)
        {
            for(int j = 0; j < B; ++j)
            {
                if (G[i][j] && matched_from[j] == -1)
                {
                    matched_from[j] = i;
                    matched_to[i] = j;
                    max_matching_size++;
                    break;
                }
            }
        }

        
        for(int i = 0; i < A; ++i)
        {
            if (matched_to[i] == -1)
            {
                bool augmenting_path_found = false;

                std::vector<int> from(A, -1);
                visited.assign(A, false);

                std::queue<int> q;
                visited[i] = true;
                q.push(i);

                while(!q.empty() && !augmenting_path_found)
                {
                    int u = q.front();
                    q.pop();

                    for(int j = 0; j < B && !augmenting_path_found; ++j)
                    {
                        if (!G[u][j])
                            continue;

                        if (matched_from[j] == -1)
                        {
                            // augment
                            int a = u;
                            int b = j;

                            while(true)
                            {
                                int prev_a = from[a];
                                int prev_b = matched_to[a];

                                matched_to[a] = b;
                                matched_from[b] = a;

                                if (prev_a == -1)
                                    break;

                                a = prev_a;
                                b = prev_b;
                            }

                            max_matching_size++;
                            augmenting_path_found = true;
                        }
                        else if (!visited[matched_from[j]])
                        {
                            q.push(matched_from[j]);
                            visited[matched_from[j]] = true;
                            from[matched_from[j]] = u;
                        }
                    }
                }
            }
        }
        


        return max_matching_size;
    }

private:
    bool augment(int i)
    {
        visited[i] = true;

        for(int j = 0; j < B; ++j)
        {
            if (G[i][j] && (matched_from[j] == -1 || !visited[matched_from[j]] && augment(matched_from[j])) )
            {
                matched_from[j] = i;
                matched_to[i] = j;
                return true;
            }
        }

        return false;
    }
};

void generate_test_case(int A, int B, double density, std::vector<std::vector<int> >& G)
{
    G.assign(A, std::vector<int>(B));
    for(int i = 0; i < A; ++i)
    {
        for(int j = 0; j < B; ++j)
        {
            G[i][j] = rand() % 0xFF < 0xFF * density;
        }
    }

}

int solve(const std::vector<std::vector<int> >& G)
{
    max_bipartite_matching matching(G);
    return matching.calculate_bfs();
}

int solve_bf(const std::vector<std::vector<int> >& G)
{
    int max_matching = 0;

    const int A = G.size();
    const int B = G[0].size();

    if (B > A)
    {
        std::vector<int> p(B);
        for(int i = 0; i < B; ++i)
            p[i] = i;

        do
        {
            int matching = 0;
            for(int i = 0; i < A; ++i)
            {
                if (G[i][p[i]])
                    matching++;
            }

            max_matching = std::max<int>(max_matching, matching);

        }
        while(std::next_permutation(p.begin(), p.end()));
    }
    else
    {
        std::vector<int> p(A);
        for(int i = 0; i < A; ++i)
            p[i] = i;

        do
        {
            int matching = 0;
            for(int i = 0; i < B; ++i)
            {
                if (G[p[i]][i])
                    matching++;
            }

            max_matching = std::max<int>(max_matching, matching);
        }
        while(std::next_permutation(p.begin(), p.end()));
    }

    return max_matching;
}

bool run_test(int t, int A, int B, double density)
{
    std::vector<std::vector<int> > G;
    generate_test_case(A, B, density, G);

    int result = solve(G);
    int result_etalon = solve_bf(G);

    if (result != result_etalon)
    {
        std::cout << "Test Case #" << std::setw(2) << t << ": FAILED!" << std::endl;
        std::cout << "\tExpected: " << result_etalon << " Received: " << result << std::endl;

        return false;
    }
    else
    {
        std::cout << "Test Case #" << std::setw(2) << t << ": PASSED!" << std::endl;

        return true;
    }
}

int main(int argc, char* argv[])
{
    const int A = 6;
    const int B = 8;
    const double density = 0.5;
    const int TESTS = 64;

    for(int t = 1; t <= TESTS; ++t)
    {
        run_test(t, A, B, density);
    }

    return 0;
}