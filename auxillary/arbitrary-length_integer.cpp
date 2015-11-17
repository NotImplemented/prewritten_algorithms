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
namespace arbitrary_precision
{

    class huge
    {
    private:
        static const int BASE = 1000000000;

    protected:
        std::vector<int> data;
        void build(long long n)
        {
            while(n)
            {
                data.push_back(n % BASE);
                n /= BASE;
            }
        }

    public:
        huge() : data() {}

        huge(const huge& h) : data(h.data) {}

        huge(long long n) : data()
        {
            if (n < 0)
                cout << ("Negative supplied");

            build(n);
        }

        std::string value()
        {
            if (!data.size())
                return "0";

            std::string result;

            for(int i = data.size()-1; i >= 0; --i)
            {
                int base = BASE;
                int num = data[i];

                std::vector<int> digits;

                while (base > 1)
                {
                    if (!num && i == data.size()-1)
                        break;

                    digits.push_back(num % 10);

                    base /= 10;
                    num /= 10;
                }

                for(int j = digits.size()-1; j >= 0; --j)
                    result += '0' + digits[j];
            }

            return result;
        }

        friend huge& operator += (huge& a, const huge& b);
        friend huge operator + (const huge& a, const huge& b);

        friend huge& operator *= (huge& a, const huge& b);
        friend huge operator * (const huge& a, const huge& b);

        friend huge& operator -= (huge& a, const huge& b);
        friend huge operator - (const huge& a, const huge& b);

        friend huge& operator /= (huge& a, const huge& b);
        friend huge operator / (const huge& a, const huge& b);

        friend bool operator < (const huge& a, const huge& b);
        friend bool operator > (const huge& a, const huge& b);

        friend bool operator == (const huge& a, const huge& b);

    };

    bool operator == (const huge& a, const huge& b)
    {
        return !(a<b) && !(b<a);
    }

    bool operator < (const huge& a, const huge& b)
    {
        if (a.data.size() != b.data.size())
            return a.data.size() < b.data.size();

        for(int i = a.data.size()-1; i >= 0; --i)
        {
            if (a.data[i] != b.data[i])
                return a.data[i] < b.data[i];
        }

        return false;
    }

    bool operator > (const huge& a, const huge& b)
    {
        return b < a;
    }

    huge& operator /=(huge& a, const huge& b)
    {
        a = a / b;
        return a;
    }

    huge operator /(const huge& a, const huge& b)
    {
        huge c;

        if (a < b)
            return c;

        if (b == 0)
            return c;

        std::vector<int> t;
        int t_value_length = 0;


        for(int i = a.data.size()-1; i >= 0; --i)
        {
            t.push_back(a.data[i]);
            t_value_length++;

            // check if t is less than b and we need to append it
            bool less = false;
            if (t_value_length != b.data.size())
            {
                less = t_value_length < b.data.size();
            }
            else
            {
                for(int i = t.size()-t_value_length, j = b.data.size()-1; i < t.size(); ++i, --j)
                {
                    if (t[i] != b.data[j])
                    {
                        less = t[i] < b.data[j];
                        break;
                    }
                }
            }

            if (less)
            {
                if (c.data.size())
                    c.data.push_back(0);
                continue;
            }

            int l = 0, h = huge::BASE;

            while (l+1 < h)
            {
                int m = (l+h) / 2;

                long long carry = 0;

                std::vector<int> t_residual(t_value_length);
                int t_total_length = t.size();
                for (int i = 0; i < t_value_length; ++i)
                {
                    long long digit_b = i < b.data.size() ? b.data[i] : 0;

                    long long digit_t_residual = (digit_b * m) + carry;
                    carry = digit_t_residual / huge::BASE;

                    t_residual[t_value_length - 1 - i] = digit_t_residual % huge::BASE;
                }

                // compare
                bool greater = false;
                if (carry)
                {
                    greater = true;
                }
                else
                {
                    for(int j = 0, i = t.size()-t_value_length; i < t.size() && j >= 0; ++i, ++j)
                    {
                        if (t[i] != t_residual[j])
                        {
                            greater = t_residual[j] > t[i];
                            break;
                        }
                    }
                }

                if (greater)
                    h = m;
                else
                    l = m;
            }


            long long carry = 0;

            std::vector<int> t_residual(t_value_length);
            int t_total_length = t.size();
            for (int i = 0; i < t_value_length; ++i)
            {
                long long digit_b = i < b.data.size() ? b.data[i] : 0;

                long long digit_t_residual = (digit_b * l) + carry;
                carry = digit_t_residual / huge::BASE;

                t_residual[t_value_length - 1 - i] = digit_t_residual % huge::BASE;
            }


            for (int i = t.size()-1, j = t_residual.size()-1; j >= 0; --i, --j)
            {
                if (t[i] >= t_residual[j])
                    t[i] -= t_residual[j];
                else
                {
                    int k = i-1;
                    while(t[k] == 0)
                    {
                        t[k] = huge::BASE - 1;
                        --k;
                    }
                    t[k] -= 1;
                    t[i] = huge::BASE + (t[i] - t_residual[j]);
                }
            }

            int k = t.size()-t_value_length;
            while(k < t.size() && t[k] == 0)
            {
                t_value_length--;
                k++;
            }

            c.data.push_back(l);
        }

        std::reverse(c.data.begin(), c.data.end());

        return c;
    }

    huge& operator -=(huge& a, const huge& b)
    {
        for(int i = 0; i < b.data.size(); ++i)
        {
            if (a.data[i] >= b.data[i])
                a.data[i] -= b.data[i];
            else
            {
                int j = i+1;
                for(; a.data[j] == 0; ++j)
                    a.data[j] = huge::BASE-1;

                a.data[j] -= 1;

                a.data[i] = huge::BASE + (a.data[i] - b.data[i]);
            }
        }

        while(a.data.size() && a.data.back() == 0)
            a.data.resize(a.data.size()-1);

        return a;
    }

    huge operator -(const huge& a, const huge& b)
    {
        huge c(a);
        c -= b;
        return c;
    }

    huge& operator *=(huge& a, const huge& b)
    {
        a = a * b;
        return a;
    }

    huge operator *(const huge& a, const huge& b)
    {
        huge c;

        for(int i = 0; i < b.data.size(); ++i)
        {

            long long carry = 0;
            for(int j = 0; j < a.data.size() || carry; ++j)
            {
                long long digit_a = a.data.size() > j ? a.data[j] : 0;
                long long digit_b = a.data.size() > j ? b.data[i] : 0;

                long long digit_c = c.data.size() > j+i ? c.data[j+i] : 0;

                long long new_digit_c = (digit_a * digit_b) + digit_c + carry;

                carry = new_digit_c / huge::BASE;
                new_digit_c %= huge::BASE;

                if (c.data.size() > i+j)
                {
                    c.data[i+j] = new_digit_c;
                }
                else
                {
                    c.data.push_back(new_digit_c);
                }
            }


        }

        return c;
    }

    huge operator +(const huge& a, const huge& b)
    {
        huge c(a);
        c += b;
        return c;
    }

    huge& operator += (huge& a, const huge& b)
    {
        long long carry = 0;


        for(int i = 0; i < b.data.size() || carry; ++i)
        {
            int digit_a = a.data.size() > i ? a.data[i] : 0;
            int digit_b = b.data.size() > i ? b.data[i] : 0;

            long long new_digit_a = digit_a + digit_b + carry;

            carry = new_digit_a / huge::BASE;
            new_digit_a %= huge::BASE;

            if (a.data.size() > i)
            {
                a.data[i] = new_digit_a;
            }
            else
            {
                a.data.push_back(new_digit_a);
            }
        }

        return a;
    }


    class signed_huge : public huge
    {

    public:
        bool positive;

        signed_huge() : huge(), positive(false) 
        {
        }

        signed_huge(const huge& h) : huge(h), positive(false) 
        {
        }

        signed_huge(const signed_huge& h) : huge(h), positive(h.positive) 
        {
        }

        signed_huge(long long n)  
        {
            positive = n > 0;

            if (n < 0)
                n = -n;

            build(n);
        }

        std::string value()
        {
            if (!data.size())
                return "0";

            std::string result;

            if (!positive)
                result += "-";

            result += huge::value();

            return result;
        }

        friend signed_huge operator + (const signed_huge& a, const signed_huge& b);
        friend signed_huge operator * (const signed_huge& a, const signed_huge& b);
        friend signed_huge operator - (const signed_huge& a, const signed_huge& b);
        friend signed_huge operator / (const signed_huge& a, const signed_huge& b);
    };

    signed_huge operator / (const signed_huge& a, const signed_huge& b)
    {
        signed_huge c = huge(a) / huge(b);
        c.positive = a.positive && b.positive || !a.positive && !b.positive;

        return c;
    }

    signed_huge operator *(const signed_huge& a, const signed_huge& b)
    {
        signed_huge c = huge(a) * huge(b);
        c.positive = a.positive && b.positive || !a.positive && !b.positive;

        return c;
    }

    signed_huge operator -(const signed_huge& a, const signed_huge& b)
    {
        signed_huge c = b;
        c.positive = !b.positive;

        return a + c;
    }

    signed_huge operator +(const signed_huge& a, const signed_huge& b)
    {
        if (a.positive && b.positive || !a.positive && !b.positive)
        {
            signed_huge c = huge(a) + huge(b);
            c.positive = a.positive;

            return c;
        }
        else
        {
            huge huge_a = a, huge_b = b;

            if (!b.positive)
            {
                if (b > a)
                {
                    signed_huge c = huge_b - huge_a;
                    c.positive = false;
                    return c;
                }
                else
                {
                    signed_huge c = huge_a - huge_b;
                    c.positive = true;
                    return c;
                }
            }
            else if (!a.positive)
            {
                if (a > b)
                {
                    signed_huge c = huge_a - huge_b;
                    c.positive = false;
                    return c;
                }
                else
                {
                    signed_huge c = huge_b - huge_a;
                    c.positive = true;
                    return c;
                }

            }
        }
    }

}

using namespace arbitrary_precision;
std::pair<int,int> test_operation(std::string name, huge op(const huge&, const huge&), bool swap_inverted)
{
    const int MAX_TESTS = 65536;

    std::cout << "Testing " << name << "\n";

    int total=0,passed=0;
    for(int i = 0; i < MAX_TESTS; ++i)
    {

        long long a = rand(), b = rand();

        if (swap_inverted && a < b)
            std::swap(a,b);

        long long c =  op == operator+ ? a+b : op == operator* ? a*b : op == operator- ? a-b : op == operator/ ?  b ? a/b : 0 : 0;

        huge ha=a,hb=b;
        huge hc = op(ha, hb);

        std::stringstream s;
        s << c;

        std::string etalon = s.str();
        std::string result = hc.value();

        if (result != etalon)
        {
            std::cout << "Test Case #" << std::setw(4) << std::setfill('0') << i << ": ";
            std::cout << "FAILED\n";
            std::cout << "\tExpected:" << etalon << "\n";
            std::cout << "\tReceived:" << result << "\n";

        }
        else
        {
            passed++;
        }

        total++;
    }
    std::cout << "\t" << passed << "/" << total << " (" << (passed*100/total) << "%) \n\n";

    return std::make_pair<int, int> (total, passed);

}

int main(int argc, char* argv[])
{
    int total = 0, passed = 0;

    std::pair<int,int> res = test_operation("Addition", operator+, false);
    total += res.first;
    passed += res.second;

    res = test_operation("Multiplication", operator*, false);
    total += res.first;
    passed += res.second;

    res = test_operation("Subtraction", operator-, true);
    total += res.first;
    passed += res.second;

    res = test_operation("Division", operator/, true);
    total += res.first;
    passed += res.second;


    std::cout << "\n\nOverall: " << passed << "/" << total << " (" << (passed*100/total) << "%) \n\n";


    int temp;
    std::cin >> temp;
}