#include <functional> 
#include <iostream> 
#include <algorithm> 
#include <sstream> 
#include <string> 
#include <utility> 
#include <vector> 
#include <cmath> 
#include <queue> 
#include <map> 
#include <set>

using namespace std;

template<typename T>
class segment_tree
{
    std::vector<T> data;
    int n;

public:

    segment_tree(int size)
    {
        n = 1;
        while (n <= size*2)
        {
            n *= 2;
        }

        data.assign(2 * n, 0);
 }

 T get_sum(int i)
 {
     if (i < 0)
         return T();

     return get_sum(0, i);
 }


 T get_sum(int l, int r)
 {
     return get_sum(l, r, 1, 0, n - 1);
 }

 T get_sum(int l, int r, int vertex, int range_left, int range_right)
 {
     if (l == range_left && r == range_right)
     {
         return data[vertex];
     }

     T result = T();

     int new_range_left = range_left + (range_right - range_left) / 2;
     int new_range_right = new_range_left + 1;

     if (l <= new_range_left)
     {
         result += get_sum(l, min(r, new_range_left), vertex * 2, range_left, new_range_left);
     }
     if (r >= new_range_right)
     {
          result += get_sum(max(l, new_range_right), r, vertex * 2 + 1, new_range_right, range_right);
     }

     return result;
 }

 void update(const T t, int index)
 {
     index += n;

     while (index)
     {
         data[index] += t;
         index /= 2;
     }
 }
};

int readint()
{
 int n;
 scanf("%d", &n);

 return n;
}

void factorize(const vector<int>& permutation, vector<int>& factorized)
{
 int n = permutation.size();
 factorized.assign(n, 0);

 segment_tree<int> tree(n);
 for (int i = 0; i < n; ++i)
 tree.update(1, i);

 
 for (int i = 0; i < n; ++i)
 {
     factorized[i] = tree.get_sum(permutation[i] - 1);
     tree.update(-1, permutation[i]);
 }
}


int get_statistic(segment_tree<int>& tree, int s, int n)
{
 int low = -1, high = n - 1;

 while (low + 1 < high)
 {
     int middle = (low + high) / 2;

     int amount = tree.get_sum(middle);

     if (amount >= s)
     {
         high = middle;
     }
     else
     {
         low = middle;
     }
 }

 return high;
}

int main()
{
#ifndef ONLINE_JUDGE
 freopen("in.txt", "r", stdin);
#endif


 int n = readint();

 vector<int> p, q;

 for (int i = 0; i < n; ++i)
     p.push_back(readint());

 for (int i = 0; i < n; ++i)
     q.push_back(readint());


 vector<int> factorized_p;
 vector<int> factorized_q;

 factorize(p, factorized_p);
 factorize(q, factorized_q);

 vector<int> factorized_sum(n);

 int carry = 0;
 for (int i = n - 1, j = 0; i >= 0; --i, ++j)
 {
    int k = factorized_p[i] + factorized_q[i] + carry;

    carry = 0;

    while (k > j)
    {
        carry++;
        k -= (j + 1);
    }

    factorized_sum[i] = k;
 }


 segment_tree<int> tree(n);
 for (int i = 0; i < n; ++i)
 {
    tree.update(1, i);
 }

 vector<int> result(n);

 for (int i = 0; i < n; ++i)
 {
     int s = get_statistic(tree, factorized_sum[i]+1, n);
     tree.update(-1, s);

     result[i] = s;
 }

 for (int i = 0; i < n; ++i)
 {
     printf("%d ", result[i]);
 }

} 
