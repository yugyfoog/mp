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

void test_mpint() {
  test_mpint_constructors();
}

void test_mpfloat() {
  ;
}

int main() {
  test_mpint();
  test_mpfloat();
  return 0;
}

