const int mod = 1000*1000*1000+7;
const int N = 128;

typedef int mt[N][N];
typedef int vt[N];

void multiply(const vt& a, const mt& b, vt& c)
{
    for(int i=0;i<N;++i)
    {
        c[i] = 0;
        for(int j=0;j<N;++j)
        {
            c[i] += ((long long)a[j]*b[j][i]) % mod;
            if (c[i] >= mod)
                c[i] -= mod;
        }
    }
}

void multiply(const mt& a, const mt& b, mt& c)
{
    for(int i=0;i<N;++i)
    {
        for(int j=0;j<N;++j)
        {
            c[i][j] = 0;
            for(int k=0;k<N;++k)
            {
                c[i][j] += ((long long)a[i][k]*b[k][j]) % mod;
                if (c[i][j] >= mod)
                    c[i][j] -= mod;
            }
        }
    }
}

void power(mt& a, long long n, mt& result)
{
    mt temp;
    memset(result, 0, sizeof(result));
    for(int i=0;i<N;++i)
       result[i][i] = 1;

    while(n)
    {
        if (n&1)
        {
            memcpy(temp, result, sizeof(result));
            multiply(a, temp, result);
            --n;
        }
        else
        {
            memcpy(temp, a, sizeof(a));
            multiply(temp, temp, a);
            n >>= 1;
        }
    }
}
