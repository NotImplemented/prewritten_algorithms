#include <functional> 
#include <iostream> 
#include <sstream> 
#include <utility> 
#include <complex> 
#include <vector> 
#include <string> 
#include <cmath> 
#include <queue> 
#include <map> 
#include <set>

using namespace std;

#define PI 3.14159265358979323846

vector<complex<double> > fft(const vector<complex<double> >& x)
{
 const int n = x.size();

 vector<complex<double> > result;

 if (n == 1)
 {
 result.push_back(x[0]);
 return result;
 }

 vector<complex<double> > a;
 vector<complex<double> > b;

 for(int i = 0; i < n; ++i)
 {
 if (i & 1)
 b.push_back(x[i]);
 else
 a.push_back(x[i]);
 }

 vector<complex<double> > fft_a = fft(a);
 vector<complex<double> > fft_b = fft(b);

 vector<complex<double> > w;

 for(int i = 0; i < n; ++i)
 {
 double angle = 2 * PI * i / n;
 complex<double> phi(cos(angle), sin(angle));

 w.push_back(phi);
 }

 for(int i = 0; i < n; ++i)
 {
 result.push_back(fft_a[i % (n / 2)] + w[i] * fft_b[i % (n / 2)]);
 }


 return result;
}

int reverse(int i, int right)
{
 int left = 1;

 while(right > left)
 {
 if ((i & right) && !(i & left))
 {
 i -= right;
 i += left;
 }
 else if (!(i & right) && (i & left))
 {
 i += right;
 i -= left;
 }

 right >>= 1;
 left <<= 1;
 }

 return i;
}

void fft_(vector<complex<double> >& x)
{
 int n = x.size();

 for(int i = 0; i < n; ++i)
 {
 int reversed = reverse(i, n/2);
 if (reversed > i)
 swap(x[i], x[reversed]);
 }

 for(int size = 1; size < n; size <<= 1)
 {
 double phi = PI / size;
 complex<double> base(cos(phi), sin(phi));

 for( int start = 0; start + 2*size <= n; start += 2*size)
 {
 const int end = start + 2*size;
 complex<double> w(1, 0);

 for(int i = 0; i < size; ++i)
 {
 complex<double> a = x[start + i];
 complex<double> b = x[start + i + size];

 x[start+i] = a + w*b;
 x[start + i + size] = a - w*b;

 w *= base;
 }
 }
 }
}

void test_fft()
{
 complex<double> input[] = { 4.0, 3.0, 2.0, 1.0, 0, 0, 0, 0 };
 vector<complex<double> > polynomial(input, input + sizeof(input) / sizeof(input[0]));


 vector<complex<double> > etalon = fft(polynomial);
 fft_(polynomial);

 bool passed = etalon.size() == polynomial.size();
 const double eps = 10e-6;
 
 for(int i = 0; i < polynomial.size(); ++i)
 {
 if (abs(polynomial[i] - etalon[i]) > eps)
 passed = false;
 }

 if (passed)
 cout << "PASSED!" << endl;
 else
 cout << "FAILED!" << endl;
}


int main()
{
 test_fft();

 return 0;
}