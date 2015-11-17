// index starts from one 

template<class T, const T& (*op)(const T&, const T&), T initial_value>
struct fenwick
{

private:

    std::vector<T> data;
    std::vector<T> data2up;
    std::vector<T> data2down;
    int N;

    int leftmost_one(int x)
    {
        return x ^ (x&(x-1));
    }

    int go_up(int x)
    {
        return x + leftmost_one(x);
    }

    int go_down(int x)
    {
        return x - leftmost_one(x);
    }

public:

    fenwick(int N) : N(N), data(N+1), data2up(N+1), data2down(N+1)
    {
    }

    T get(int a, int b)
    {
        if (a > b)
            throw std::exception("Left range should be less or equal to right range.");

        T result = initial_value;

        for(;go_up(a) <= b; a = go_up(a))
        {
            result = op(result, data2up[a]);
        }


        for(;go_down(b) >= a; b = go_down(b))
        {
            result = op(result, data2down[b]);
        }


        result = op(result, data[a]);


        return result;
    }

    void update(int x, int value)
    {
        if (x == 0)
            throw std::exception("Index should be greater than zero.");

        data[x] = op(data[x], value);

        int a = x, b = x;

        while(a <= N)
        {
            data2down[a] = op(data2down[a], value);
            a = go_up(a);
        }

        while(b > 0)
        {
            data2up[b] = op(data2up[b], value);
            b = go_down(b);
        }
    }
};

typedef fenwick<int, std::max<int>, 0> fenwick_max;
typedef fenwick<int, std::min<int>, INT_MAX> fenwick_min;