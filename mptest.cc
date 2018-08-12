#include <iostream>
#include "mp.h"

using namespace std;

int digits;
int size;

void test_mpint_constructors() {
  mpint a;
  cout << a << endl;
  
  mpint b(-5);
  cout << b << endl;
  mpint c(0);
  cout << c << endl;
  mpint d(5);
  cout << d << endl;
  int ee = -2147483648;
  mpint e(ee);
  cout << e << endl;
  
  mpint f(0u);
  cout << f << endl;
  mpint g(5u);
  cout << g << endl;

  mpint h(-5l);
  cout << h << endl;
  mpint i(0l);
  cout << i << endl;
  mpint j(5l);
  cout << j << endl;
  long kk = 0x8000000000000000;
  mpint k(kk);
  cout << k << endl;
  
  mpint l(0ul);
  cout << l << endl;
  mpint m(5ul);
  cout << m << endl;
  
  mpint n("15226050279225333605356183781326374297180681149613"
	  "80688657908494580122963258952897654000350692006139");
  cout << n << endl;
  mpint o("-1234567890");
  cout << o << endl;

  mpint q(n);
  cout << q << endl;
}

void test_mpint_assignment() {
  mpint a, b, c, d;
  int dd;
  mpint e, f;
  mpint g, h, i, j;
  long jj;
  mpint k, l;
  mpint m;
  
  a = -5;
  cout << a << endl;
  b = 0;
  cout << b << endl;
  c = 5;
  cout << c << endl;
  dd = -2147483648;
  d = dd;
  cout << d << endl;

  e = 0u;
  cout << e << endl;
  f = 5u;
  cout << f << endl;

  g = -5l;
  cout << g << endl;
  h = 0l;
  cout << h << endl;
  i = 5l;
  cout << i << endl;
  jj = 0x8000000000000000;
  j = jj;
  cout << j << endl;

  k = 0ul;
  cout << k << endl;
  l = 5ul;
  cout << l << endl;

  mpint mm("15226050279225333605356183781326374297180681149613"
	  "80688657908494580122963258952897654000350692006139");
  m = mm;
  cout << m << endl;
}

void test_mpint_addition() {
  mpint a("1234567890123456789012345678901234567890");
  mpint b("9876543210987654321098765432109876543210");

  mpint c = a + b;
  cout << c << endl;

  mpint d("-555555555555555555555555555555555555555555555555555555555555555555555");
  mpint e("444444444444444444444444444444444444444444444444444444444444444444444");
  mpint f = d + e;
  mpint g = e + d;
  cout << f << endl;
  cout << g << endl;

  mpint h("55555555555555555555555555555555555555555555555555555555555555555555555");
  mpint i("-55555555555555555555555555555555555");
  mpint j = h + i;
  mpint k = i + h;
  cout << j << endl;
  cout << k << endl;

  mpint l("1234567890123456789012345678901234567890123456789012345678901234567890123456789");
  mpint m("-1234567890123456789012345678901234567890123456789012345678901234567890123456789");
  mpint n = l + m;
  mpint o = m + l;
  cout << n << endl;
  cout << o << endl;
}

void test_mpint() {
  test_mpint_constructors();
  test_mpint_assignment();
  test_mpint_addition();
}

void test_mpfloat() {
  ;
}

int main() {
  test_mpint();
  test_mpfloat();
  return 0;
}

