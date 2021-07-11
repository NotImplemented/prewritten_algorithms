#include <functional> 
#include <iostream> 
#include <sstream> 
#include <string> 
#include <utility> 
#include <vector> 
#include <cmath> 
#include <queue> 
#include <map> 
#include <set>

using namespace std;

const int MAX_N = 200004;
const int LOG_MAX_N = 20;

int N;
char text[MAX_N];
int cnt[MAX_N];
int permutation[2][MAX_N];
int equivalences[LOG_MAX_N][MAX_N];

int calculate_common_prefix(int a, int b, int size, int position)
{
    while(size > 0)
    {
        if (equivalences[position][a] == equivalences[position][b])
        {
            int a_next = a + size;
            if (a_next >= N)
                a_next -= N;

            int b_next = b + size;
            if (b_next >= N)
                b_next -= N;

            return size + calculate_common_prefix(a_next, b_next, size / 2, position-1);
        }

        size >>= 1;
        position--;
    }

    return 0;
}


int main()
{
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    // freopen("out.txt", "w", stdout);
#endif

    scanf("%d\n", &N);

    const int A_size = N;
    const int B_size = N;


    gets(text);
    text[N] = '*';

    gets(text + N+1);
    text[N] = '*';
    text[2*N + 1] = '$';
    text[2*N + 2] = 0;

    N = 2*N + 2;

    const int ALPHABETH_SIZE = CHAR_MAX;

    memset(cnt, 0, sizeof(cnt[0]) * ALPHABETH_SIZE);

    for(int i = 0; i < N; ++i)
        cnt[text[i]]++;

    for(int i = 1; i < ALPHABETH_SIZE; ++i)
        cnt[i] += cnt[i-1];

    for(int i = 0; i < N; ++i)
    {
        permutation[0][cnt[text[i]]-1] = i;
        cnt[text[i]]--;
    }

    equivalences[0][permutation[0][0]] = 0;
    for(int i = 1; i < N; ++i)
    {
        char ch = text[permutation[0][i-1]];
        char ch_next = text[permutation[0][i]];

        if (ch == ch_next)
            equivalences[0][permutation[0][i]] = equivalences[0][permutation[0][i-1]];
        else
            equivalences[0][permutation[0][i]] = equivalences[0][permutation[0][i-1]] + 1;
    }


    int size = 1, p = 1, t = 0;
    for(; size < 2*N; size <<= 1, ++p, t = 1-t)
    {
        int from = t;
        int to = 1-t;

        memset(cnt, 0, sizeof(cnt[0])*N);

        for(int i = 0; i < N; ++i)
        {
            int at = permutation[from][i] - size;
            while (at < 0)
                at += N;

            cnt[equivalences[p-1][at]]++;
        }

        for(int i = 1; i < N; ++i)
            cnt[i] += cnt[i-1];

        for(int i = N-1; i >= 0; --i)
        {
            int at = permutation[from][i] - size;
            while (at < 0)
                at += N;

            permutation[to][cnt[equivalences[p-1][at]]-1] = at;
            cnt[equivalences[p-1][at]]--;
        }

        for(int i = 1; i < N; ++i)
        {
            int at = permutation[to][i-1] + size;
            while (at >= N)
                at -= N;

            int at_next = permutation[to][i] + size;
            while (at_next >= N)
                at_next -= N;

            if (equivalences[p-1][permutation[to][i-1]] == equivalences[p-1][permutation[to][i]] && 
                equivalences[p-1][at] == equivalences[p-1][at_next])
                equivalences[p][permutation[to][i]] = equivalences[p][permutation[to][i-1]];
            else
                equivalences[p][permutation[to][i]] = equivalences[p][permutation[to][i-1]] + 1;
        }
    }

    int length = 0;
    int position = -1;

    for(int i = 1; i < N; ++i)
    {
        int at = permutation[t][i-1];
        int at_next = permutation[t][i];

        if (at < A_size && at_next < A_size)
            continue;

        if (at > A_size && at_next > A_size)
            continue;

        int common_prefix = calculate_common_prefix(at, at_next, size, p-1);

        if (common_prefix > length)
        {
            length = common_prefix;
            if (at < A_size)
                position = at;
            else
                position = at_next;
        }
    }

    if (length > 0)
    {
        text[position+length] = 0;
        printf("%s", text + position);
    }


    printf("\n");
} 
