// test mplib 

#include <iostream>
#include <sstream>
#include "mp.h"

int fail_count = 0;

void test_mpint();
void test_mpint_constructors();
void test_mpint_io();
void test_mpint_operators();
void test_mpint_relational_operators();
void test_mpint_arithmetic_operators();
void test_mpfloat();
void test_mpfloat_constructors();
void test_mpfloat_io();
void test_mpfloat_operators();
void test_mpfloat_functions();

void test_fail(std::string const &, std::string const &, std::string const &);
void comparison_test_fail(std::string const &, mpint const &, mpint const &);
void unary_test_fail(std::string const &, mpint const &, mpint const &);
void binary_test_fail(std::string const &, mpint const &, mpint const &, mpint const &);

int main() {
  std::cout << "Testing Multi-Precision C++ Classes" << std::endl;

  test_mpint();
  test_mpfloat();

  if (fail_count == 0)
    std::cout << "All tests passed" << std::endl;
  else
    std::cout << fail_count << " test" << (fail_count == 1 ? "" : "s") << " passed." << std::endl;
  
  return 0;
}

void test_mpint() {
  test_mpint_constructors();
  test_mpint_io();
  test_mpint_operators();
}

void test_mpint_constructors() {
  std::ostringstream ss;
  
  std::string A = "0";
  std::string C = "9223372036854775807";
  std::string D = "-9223372036854775808";
  std::string F = "123456789012345678901234567890";
  std::string G = "-987654321098765432109876543210";
  std::string K1 = "123456789012345678901234567890";
  std::string K2 = "-123456789012345678901234567890";
  std::string L = "246913578024691357802469135780";
  std::string M = "-864197532086419753208641975320";
  // mpint()

  mpint a;

  ss << a;
  if (ss.str() != A)
    test_fail("mpint()", ss.str(), A);
  ss.str("");

  // mpint(long)

  mpint b(0);

  ss << b;
  if (ss.str() != A)
    test_fail("mpint(long)", ss.str(), A);
  ss.str("");
  
  mpint c((long)0x7fffffffffffffff);

  ss << c;
  if (ss.str() != C)
    test_fail("mpint(long)", ss.str(), C);
  ss.str("");

  mpint d((long)0x8000000000000000);

  ss << d;
  if (ss.str() != D)
    test_fail("mpint(long)", ss.str(), D);
  ss.str("");

  // mpint(string const &)

  mpint e(A);
  
  ss << e;
  if (ss.str() != A)
    test_fail("mpint(string)", ss.str(), A);
  ss.str("");
  
  mpint f(F);

  ss << f;
  if (ss.str() != F)
    test_fail("mpint(string)", ss.str(), F);
  ss.str("");
 
  mpint g(G);

  ss << g;
  if (ss.str() != G)
    test_fail("mpint(string)", ss.str(), G);
  ss.str("");
  
    
  // mpint(mpint const &)

  mpint h(a);

  ss << h;
  if (ss.str() != A)
    test_fail("mpint(mpint const &)", ss.str(), A);
  ss.str("");
    
  mpint i(f);
  
  ss << i;
  if (ss.str() != F)
    test_fail("mpint(mpint const &)", ss.str(), F);
  ss.str("");
  
  mpint j(g);

  ss << j;
  if (ss.str() != G)
    test_fail("mpint(mpint const &)", ss.str(), G);
  ss.str("");

  // mpint(mpint &&)

  mpint k = mpint(K1) + mpint(K2);

  ss << k;
  if (ss.str() != A)
    test_fail("mpint(mpint &&)", ss.str(), A);
  ss.str("");

  mpint l = mpint(K1) + mpint(K1);

  ss << l;
  if (ss.str() != L)
    test_fail("mpint(mpint &&)", ss.str(), L);
  ss.str("");

  mpint m = mpint(F) + mpint(G);

  ss << m;
  if (ss.str() != M)
    test_fail("mpint(mpint &&)", ss.str(), M);
  ss.str("");

}

void test_mpint_io() {
  std::string A = "0";
  std::string B = "123456789012345678901234567890";
  std::string C = "-987654321098765432109876543210";

  mpint x;

  {
    std::istringstream in(A);
    std::ostringstream out;
    
    in >> x;
    out << x;
    if (out.str() != A)
      test_fail("mpint I/O", out.str(), A);
  }

  {
    std::istringstream in(B);
    std::ostringstream out;

    in >> x;
    out << x;
    if (out.str() != B)
      test_fail("mpint I/O", out.str(), B);
  }

  {
    std::istringstream in(C);
    std::ostringstream out;

    in >> x;
    out << x;
    if (out.str() != C)
      test_fail("mpint I/O", out.str(), C);
  }
}

void test_mpint_operators() {
  test_mpint_relational_operators();
  test_mpint_arithmetic_operators();
}

void test_mpint_relational_operators() {
  mpint a("-123456789012345678901234567891");
  mpint b("-123456789012345678901234567890");
  mpint b2("-123456789012345678901234567890");
  mpint c(0);
  mpint c2(0);
  mpint d("123456789012345678901234567890");
  mpint d2("123456789012345678901234567890");
  mpint e("123456789012345678901234567891");
 
  if ((c == c2) != true) comparison_test_fail("mpint == mpint", c, c2);
  if ((c == d) != false) comparison_test_fail("mpint == mpint", c, d);
  if ((d == c) != false) comparison_test_fail("mpint == mpint", d, c);
  if ((c == b) != false) comparison_test_fail("mpint == mpint", c, b);
  if ((b == c) != false) comparison_test_fail("mpint == mpint", b, c);
  if ((d == b) != false) comparison_test_fail("mpint == mpint", d, b);
  if ((b == d) != false) comparison_test_fail("mpint == mpint", b, d);
  if ((d == d2) != true) comparison_test_fail("mpint == mpint", d, d);
  if ((d == e) != false) comparison_test_fail("mpint == mpint", d, e);
  if ((e == d) != false) comparison_test_fail("mpint == mpint", e, d);
  if ((b == b2) != true) comparison_test_fail("mpint == mpint", b, b);
  if ((b == a) != false) comparison_test_fail("mpint == mpint", b, a);
  if ((a == b) != false) comparison_test_fail("mpint == mpint", a, b);

  if ((c != c2) != false) comparison_test_fail("mpint != mpint", c, c2);
  if ((c != d) != true) comparison_test_fail("mpint != mpint", c, d);
  if ((d != c) != true) comparison_test_fail("mpint != mpint", d, c);
  if ((c != b) != true) comparison_test_fail("mpint != mpint", c, b);
  if ((b != c) != true) comparison_test_fail("mpint != mpint", b, c);
  if ((d != b) != true) comparison_test_fail("mpint != mpint", d, b);
  if ((b != d) != true) comparison_test_fail("mpint != mpint", b, d);
  if ((d != d2) != false) comparison_test_fail("mpint != mpint", d, d);
  if ((d != e) != true) comparison_test_fail("mpint != mpint", d, e);
  if ((e != d) != true) comparison_test_fail("mpint != mpint", e, d);
  if ((b != b2) != false) comparison_test_fail("mpint != mpint", b, b);
  if ((b != a) != true) comparison_test_fail("mpint != mpint", b, a);
  if ((a != b) != true) comparison_test_fail("mpint != mpint", a, b);

  if ((c < c2) != false) comparison_test_fail("mpint < mpint", c, c2);
  if ((c < d) != true) comparison_test_fail("mpint < mpint", c, d);
  if ((d < c) != false) comparison_test_fail("mpint < mpint", d, c);
  if ((c < b) != false) comparison_test_fail("mpint < mpint", c, b);
  if ((b < c) != true) comparison_test_fail("mpint < mpint", b, c);
  if ((d < b) != false) comparison_test_fail("mpint < mpint", d, b);
  if ((b < d) != true) comparison_test_fail("mpint < mpint", b, d);
  if ((d < d2) != false) comparison_test_fail("mpint < mpint", d, d);
  if ((d < e) != true) comparison_test_fail("mpint < mpint", d, e);
  if ((e < d) != false) comparison_test_fail("mpint < mpint", e, d);
  if ((b < b2) != false) comparison_test_fail("mpint < mpint", b, b);
  if ((b < a) != false) comparison_test_fail("mpint < mpint", b, a);
  if ((a < b) != true) comparison_test_fail("mpint < mpint", a, b);

  if ((c <= c2) != true) comparison_test_fail("mpint <= mpint", c, c2);
  if ((c <= d) != true) comparison_test_fail("mpint <= mpint", c, d);
  if ((d <= c) != false) comparison_test_fail("mpint <= mpint", d, c);
  if ((c <= b) != false) comparison_test_fail("mpint <= mpint", c, b);
  if ((b <= c) != true) comparison_test_fail("mpint <= mpint", b, c);
  if ((d <= b) != false) comparison_test_fail("mpint <= mpint", d, b);
  if ((b <= d) != true) comparison_test_fail("mpint <= mpint", b, d);
  if ((d <= d2) != true) comparison_test_fail("mpint <= mpint", d, d);
  if ((d <= e) != true) comparison_test_fail("mpint <= mpint", d, e);
  if ((e <= d) != false) comparison_test_fail("mpint <= mpint", e, d);
  if ((b <= b2) != true) comparison_test_fail("mpint <= mpint", b, b);
  if ((b <= a) != false) comparison_test_fail("mpint <= mpint", b, a);
  if ((a <= b) != true) comparison_test_fail("mpint <= mpint", a, b);

  if ((c > c2) != false) comparison_test_fail("mpint > mpint", c, c2);
  if ((c > d) != false) comparison_test_fail("mpint > mpint", c, d);
  if ((d > c) != true) comparison_test_fail("mpint > mpint", d, c);
  if ((c > b) != true) comparison_test_fail("mpint > mpint", c, b);
  if ((b > c) != false) comparison_test_fail("mpint > mpint", b, c);
  if ((d > b) != true) comparison_test_fail("mpint > mpint", d, b);
  if ((b > d) != false) comparison_test_fail("mpint > mpint", b, d);
  if ((d > d2) != false) comparison_test_fail("mpint > mpint", d, d);
  if ((d > e) != false) comparison_test_fail("mpint > mpint", d, e);
  if ((e > d) != true) comparison_test_fail("mpint > mpint", e, d);
  if ((b > b2) != false) comparison_test_fail("mpint > mpint", b, b);
  if ((b > a) != true) comparison_test_fail("mpint > mpint", b, a);
  if ((a > b) != false) comparison_test_fail("mpint > mpint", a, b);

  if ((c >= c2) != true) comparison_test_fail("mpint >= mpint", c, c2);
  if ((c >= d) != false) comparison_test_fail("mpint >= mpint", c, d);
  if ((d >= c) != true) comparison_test_fail("mpint >= mpint", d, c);
  if ((c >= b) != true) comparison_test_fail("mpint >= mpint", c, b);
  if ((b >= c) != false) comparison_test_fail("mpint >= mpint", b, c);
  if ((d >= b) != true) comparison_test_fail("mpint >= mpint", d, b);
  if ((b >= d) != false) comparison_test_fail("mpint >= mpint", b, d);
  if ((d >= d2) != true) comparison_test_fail("mpint >= mpint", d, d);
  if ((d >= e) != false) comparison_test_fail("mpint >= mpint", d, e);
  if ((e >= d) != true) comparison_test_fail("mpint >= mpint", e, d);
  if ((b >= b2) != true) comparison_test_fail("mpint >= mpint", b, b);
  if ((b >= a) != true) comparison_test_fail("mpint >= mpint", b, a);
  if ((a >= b) != false) comparison_test_fail("mpint >= mpint", a, b);

}

void test_mpint_arithmetic_operators() {
  mpint a(0);
  mpint b("9999999999999999999999999999999999999999");
  mpint c("-9999999999999999999999999999999999999999");
  mpint d("19999999999999999999999999999999999999998");
  mpint e("-19999999999999999999999999999999999999998");
  mpint f("99999999999999999999999999999999999999980000000000000000000000000000000000000001");
  mpint g("-99999999999999999999999999999999999999980000000000000000000000000000000000000001");
  mpint h("99999999999999999999999999999999999999999999999999999999999999999999999999999998");
  mpint i("-99999999999999999999999999999999999999999999999999999999999999999999999999999998");
  mpint j("10000000000000000000000000000000000000000");
  mpint k("-10000000000000000000000000000000000000000");
  mpint l("9999999999999999999999999999999999999998");
  mpint m("-9999999999999999999999999999999999999998");
  
  // + (unary nop)
  
  mpint x = +a;
  if (x != a)
    unary_test_fail("+mpint", x, a);
  x = +b;
  if (x != b)
    unary_test_fail("+mpint", x, b);
  x = +c;
  if (x != c)
    unary_test_fail("+mpint", x, c);

  // - (negation)

  x = -a;
  if (x != a)
    unary_test_fail("-mpint", x, a);
  x = -b;
  if (x != c)
    unary_test_fail("-mpint", x, c);
  x = -c;
  if (x != b)
    unary_test_fail("-mpint", x, b);

  // + (addition)

  x = c + c;
  if (x != e)
    binary_test_fail("mpint + mpint", x, c, c);
  x = a + c;
  if (x != c)
    binary_test_fail("mpint + mpint", x, a, c);
  x = b + c;
  if (x != a)
    binary_test_fail("mpint + mpint", x, b, c);
  x = c + a;
  if (x != c)
    binary_test_fail("mpint + mpint", x, c, a);
  x = a + a;
  if (x != a)
    binary_test_fail("mpint + mpint", x, a, a);
  x = b + a;
  if (x != b)
    binary_test_fail("mpint + mpint", x, b, a);
  x = c + b;
  if (x != a)
    binary_test_fail("mpint + mpint", x, c, b);
  x = a + b;
  if (x != b)
    binary_test_fail("mpint + mpint", x, a, b);
  x = b + b;
  if (x != d)
    binary_test_fail("mpint + mpint", x, b, b);
  
  // - (subtraction)

  x = c - c;
  if (x != a)
    binary_test_fail("mpint - mpint", x, c, c);
  x = a - c;
  if (x != b)
    binary_test_fail("mpint - mpint", x, a, c);
  x = b - c;
  if (x != d)
    binary_test_fail("mpint - mpint", x, b, c);
  x = c - a;
  if (x != c)
    binary_test_fail("mpint - mpint", x, c, a);
  x = a - a;
  if (x != a)
    binary_test_fail("mpint - mpint", x, a, a);
  x = b - a;
  if (x != b)
    binary_test_fail("mpint - mpint", x, b, a);
  x = c - b;
  if (x != e)
    binary_test_fail("mpint - mpint", x, c, b);
  x = a - b;
  if (x != c)
    binary_test_fail("mpint - mpint", x, a, b);
  x = b - b;
  if (x != a)
    binary_test_fail("mpint - mpint", x, b, b);

  // * (multiplication)

  x = c * c; // -9*-9 = 81
  if (x != f)
    binary_test_fail("mpint * mpint", x, c, c);
  x = a * c; // 0*-9 = 0
  if (x != a)
    binary_test_fail("mpint * mpint", x, a, c);
  x = b * c; // 9*-9 = -81
  if (x != g)
    binary_test_fail("mpint * mpint", x, b, c);
  x = c * a; // -9*a = 0
  if (x != a)
    binary_test_fail("mpint * mpint", x, c, a);
  x = a * a; // 0*0 = 0
  if (x != a)
    binary_test_fail("mpint * mpint", x, a, a);
  x = b * a; // 9*0 = 0
  if (x != a)
    binary_test_fail("mpint * mpint", x, b, a);
  x = c * b; // -9*9 = -81
  if (x != g)
    binary_test_fail("mpint * mpint", x, c, b);
  x = a * b; // 0*9 = 0
  if (x != a)
    binary_test_fail("mpint * mpint", x, a, b);
  x = b * b; // 9*9 = 81
  if (x != f)
    binary_test_fail("mpint * mpint", x, b, b);

  // / (division)

  x = a / b;
  if (x != a)
    binary_test_fail("mpint / mpint", x, a, b);

  x = a / c;
  if (x != a)
    binary_test_fail("mpint / mpint", x, a, c);

  x = h / b;
  if (x != j)
    binary_test_fail("mpint / mpint", x, h, b);

  x = i / b;
  if (x != k)
    binary_test_fail("mpint / mpint", x, i, b);
    
  x = h / c;
  if (x != k)
    binary_test_fail("mpint / mpint", x, h, c);
  
  x = i / c;
  if (x != j)
    binary_test_fail("mpint / mpint", x, i, c);
  
  // % (remainder -- don't get me started on why this is NOT a modulus!)

  x = a % b;
  if (x != a)
    binary_test_fail("mpint % mpint", x, a, b);

  x = a % c;
  if (x != a)
    binary_test_fail("mpint % mpint", x, a, c);

  x = h % b;
  if (x != l)
    binary_test_fail("mpint % mpint", x, h, b);

  x = i % b;
  if (x != m)
    binary_test_fail("mpint % mpint", x, i, b);
    
  x = h % c;
  if (x != m)
    binary_test_fail("mpint % mpint", x, h, c);
  
  x = i % c;
  if (x != l)
    binary_test_fail("mpint % mpint", x, i, c);
  
}

void test_mpfloat() {
  test_mpfloat_constructors();
  test_mpfloat_io();
  test_mpfloat_operators();
  test_mpfloat_functions();
}

void test_mpfloat_constructors() {
  std::ostringstream ss;
  std::string A = "0";
  std::string C = "9.223372e18";
  std::string D = "-9.223372e18";
  std::string F = "1.1";
  std::string G = "-1.1";
  std::string I = "123.456";

  
  // mpfloat()
  
  mpfloat a;

  ss << a;
  if (ss.str() != A)
    test_fail("mpfloat()", ss.str(), A);
  ss.str("");
  
  // mpfloat(long)

  mpfloat b(0);

  ss << b;
  if (ss.str() != A)
    test_fail("mpfloat(long)", ss.str(), A);
  ss.str("");

  mpfloat c((long)0x7fffffffffffffff);
  
  ss << c;
  if (ss.str() != C)
    test_fail("mpfloat(long)", ss.str(), C);
  ss.str("");

  mpfloat d((long)0x8000000000000000);

  ss << d;
  if (ss.str() != D)
    test_fail("mpfloat(long)", ss.str(), D);
  ss.str("");
    
  // mpfloat(double)

  mpfloat e(0.0);

  ss << e;
  if (ss.str() != A)
    test_fail("mpfloat(long)", ss.str(), A);
  ss.str("");

  mpfloat f(1.1);

  ss << f;
  if (ss.str() != F)
    test_fail("mpfloat(double)", ss.str(), F);
  ss.str("");

  mpfloat g(-1.1);
  
  ss << g;
  if (ss.str() != G)
    test_fail("mpfloat(double)", ss.str(), G);
  ss.str("");

  // mpfloat(string)

  mpfloat h(A);

  ss << h;
  if (ss.str() != A)
    test_fail("mpfloat(double)", ss.str(), A);
  ss.str("");

  mpfloat i(I);
    
  ss << i;
  if (ss.str() != I)
    test_fail("mpfloat(double)", ss.str(), I);
  ss.str("");

  // mpfloat(mpint)
  // mpfloat(mpfloat const &)
  // mpfloat(mpfloat &&)

  
}

void test_mpfloat_io() {
  std::cout << "***** mpfloat input/output not tested" << std::endl;
}

void test_mpfloat_operators() {
  std::cout << "***** mpfloat operators not tested" << std::endl;
}

void test_mpfloat_functions() {
  std::cout << "***** mpfloat functions not tested" << std::endl;
}

void test_fail(std::string const &t, std::string const &r, std::string const &s) {
  std::cout << "***** " << t << " test failed: "
	    << r << " should be " << s << std::endl;
}

void comparison_test_fail(std::string const &t, mpint const &r, mpint const &s) {
  std::cout << "***** " << t << " test failed: "
	    << r << " and " << s << std::endl;
}

void unary_test_fail(std::string const &t, mpint const &z, mpint const &x) {
  std::cout << "***** " << t << " test failed: "
	    << z << " from " << x << std::endl;
}

void binary_test_fail(std::string const &t, mpint const &z, mpint const &x, mpint const &y) {
  std::cout << "***** " << t << " test failed: "
	    << z << " from " << x << " and " << y << std::endl;
}
