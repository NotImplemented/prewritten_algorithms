// Maximum weighted matching
// Hungarian algorithm O(N3)
// Input: square matrix w[i][j] = edge weight 
// Output: maximum matching weight

#include <functional>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <numeric>
#include <utility>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <vector>
#include <bitset>
#include <queue>
#include <deque>
#include <stack>
#include <cmath>
#include <ctime>
#include <list>
#include <map>
#include <set>


void augment(std::vector<int>& matchedA, std::vector<int>& matchedB, std::vector<int>& tree, int i, int j)
{
    while(true)
    {
        int j_new = matchedA[i], i_new = j_new != -1 ? tree[j_new] : -1;

        matchedA[i] = j;
        matchedB[j] = i;

        if (j_new == -1)
            break;

        i = i_new;
        j = j_new;
    }
}

void update_delta(std::vector<int>& deltaB, const std::vector<int>& labelA, const std::vector<int>& labelB, const std::vector<std::vector<int> >& w, int i)
{
    const int A = w.size(), B = w[0].size();

    for(int j = 0; j < B; ++j)
    {
        int delta_edge = deltaB[j];

        if (labelA[i] + labelB[j] - w[i][j] < labelA[delta_edge] + labelB[j] - w[delta_edge][j])
        {
            deltaB[j] = i;
        }
    }
}

int hungarian(const std::vector<std::vector<int> >& w)
{
    if (!w.size())
        return 0;

    const int A = w.size(), B = w[0].size();
    std::vector<int> labelA(A, 0), labelB(B);


    // setup initial labeling
    for(int i = 0; i < A; ++i)
        for(int j = 0; j < B; ++j)
            labelB[j] = std::max<int>(labelB[j], w[i][j]);

    std::vector<int> matchedA(A,-1), matchedB(B,-1);

    bool path_found = true;
    while(path_found)
    {
        path_found = false;

        std::vector<int> visitedA(A,0), visitedB(B,0), tree(B,-1);

        std::queue<int> q;

        int unmatched = -1;
        for(int i = 0; i < A; ++i)
            if (matchedA[i] == -1)
                unmatched = i;

        if (unmatched == -1)
            break;

        std::vector<int> deltaB(B, unmatched);


        q.push(unmatched);
        visitedA[unmatched] = true;

        while(!q.empty())
        {
            int v = q.front(); q.pop();

            for(int j = 0; j < B; ++j)
            {
                if (labelA[v] + labelB[j] != w[v][j])
                    continue;

                if (visitedB[j])
                    continue;

                visitedB[j] = true;
                int u = matchedB[j];

                if (u == -1)
                {
                    path_found = true;
                    augment(matchedA, matchedB, tree, v, j);
                    break;
                }
                else if (!visitedA[u])
                {
                    tree[j] = v;
                    visitedA[u] = true;
                    q.push(u);

                    update_delta(deltaB, labelA, labelB, w, u);

                }
            }

            if (!q.size() && !path_found)
            {
                int delta = -1, delta_origin = -1;

                // find min delta
                for(int j = 0; j < B; ++j)
                    if (!visitedB[j] && (delta == -1 || delta > labelA[deltaB[j]] + labelB[j] - w[deltaB[j]][j]))
                        delta = labelA[deltaB[j]] + labelB[j] - w[deltaB[j]][j], delta_origin = j; 

                for(int j = 0; j < B; ++j)
                    if (visitedB[j])
                        labelB[j] += delta;

                for(int i = 0; i < A; ++i)
                    if (visitedA[i])
                        labelA[i] -= delta;


                int i = deltaB[delta_origin], j = delta_origin;

                int u = matchedB[j];
                if (u == -1)
                {
                    path_found = true;
                    augment(matchedA, matchedB, tree, i, j);
                }
                else if (!visitedA[u])
                {
                    tree[j] = i;
                    visitedA[u] = true;
                    q.push(u);
                    update_delta(deltaB, labelA, labelB, w, u);

                }
            }

            if (path_found)
                break;
        }
    }


    int res = 0;
    for(int i = 0; i < A; ++i)
    {
        if (matchedA[i] != -1)
            res += w[i][matchedA[i]];
    }


    return res;
}


std::vector<std::vector<int> > prepare_test_case(int max_n, int max_w)
{
    const int N = rand() % (max_n-1) + 1;

    std::vector<std::vector<int> > w(N, std::vector<int>(N));

    for(int i=0;i<N;++i)
        for(int j=0;j<N;++j)
            w[i][j] = (rand() % max_w) + 1;

    return w;
}

int hungarian_etalon(const std::vector<std::vector<int> >& w)
{
    const int N = w.size();

    std::vector<int> p(N);
    for(int i=0;i<N;++i)
        p[i] = i;


    int res = 0;
    do
    {
        int matching = 0;
        for(int i = 0; i < N; ++i)
        {
            matching += w[i][p[i]];
        }

        res = std::max<int>(res, matching);
    }
    while(next_permutation(p.begin(), p.end()));

    return res;
}

int main(int argc, char* argv[])
{
    const int MAX_TESTS = 1024;

    const int MAX_N = 10;
    const int MAX_W = 4;

    int total = 0, passed = 0;
    for(int i = 0; i < MAX_TESTS; ++i, total++)
    {
        std::vector<std::vector<int> > w = prepare_test_case(MAX_N, MAX_W);

        int res = hungarian(w);
        int res_etalon = hungarian_etalon(w);

        std::cout << "Test Case #" << std::setw(3) << i << ": ";

        if (res == res_etalon)
        {
            std::cout << "PASSED!\n";
            passed++;
        }
        else
        {
            std::cout << "FAILED!\n";
            std::cout << "\tExpected: " << res_etalon << "\n";
            std::cout << "\tReceived: " << res << "\n";
        }
    }

    std::cout << "\n" << passed << "/" << total << " (" << (passed*100/total) << "%)\n";

    int temp;
    std::cin >> temp;
}