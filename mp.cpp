#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <bit>
#include <cstdint>
#include <cmath>
#include <cstring>
#include "mp.h"

mpint::mpint() {
  make_zero();
}

mpint::mpint(long x) {
  if (x == 0)
    make_zero();
  else if (x < 0) {
    make_from_digit(-x);
    sign = -1;
  }
  else
    make_from_digit(x);
}

mpint::mpint(std::string const &s) {
  mpint x;
  long sgn = 1;
  
  auto cptr = s.begin();

  if (*cptr == '+')
    cptr++;
  else if (*cptr == '-') {
    sgn = -1;
    cptr++;
  }
  for (; cptr != s.end(); cptr++) {
    if (!isdigit(*cptr))
      break;
    x = 10*x + (*cptr - '0');
  }
  if (x.sign == 0)
    make_zero();
  else
    copy(sgn*x);
  x.digits = 0;
}

mpint::mpint(mpint const &x) {
  if (x.sign == 0)
    make_zero();
  else
    copy(x);
}

mpint::mpint(mpint &&x) {
  if (x.sign == 0)
    make_zero();
  else {
    move(x);
    x.digits = 0;
  }
}

mpint::mpint(long s, long l, Digit *d) {
  if (s == 0)
    make_zero();
  else {
    sign = s;
    size = l;
    digits = new Digit[size];
    fast_copy(digits, d, size);
  }
  normalize();
}

mpint::~mpint() {
  delete [] digits;
}

void mpint::make_zero() {
  sign = 0;
  size = 0;
  digits = 0;
}

void mpint::make_from_digit(Digit x) {
  if (x == 0)
    make_zero();
  sign = 1;
  size = 1;
  digits = new Digit[1];
  digits[0] = x;
}

void mpint::copy(mpint const &x) {
  sign = x.sign;
  size = x.size;
  digits = new Digit[size];
  fast_copy(digits, x.digits, size);
}

void mpint::move(mpint const &x) {
  sign = x.sign;
  size = x.size;
  digits = x.digits;
}

void mpint::normalize() {
  long i;
  for (i = size-1; i >= 0; i--)
    if (digits[i] != 0)
      break;
  if (i < 0) {
    delete [] digits;
    make_zero();
  }
  else
    size = i+1;
}

mpint &mpint::operator = (mpint const &x) {
  delete [] digits;
  if (x.sign == 0)
    make_zero();
  else
    copy(x);
  return *this;
}

mpint &mpint::operator = (mpint &&x) {
  delete [] digits;
  if (x.sign == 0)
    make_zero();
  else {
    move(x);
    x.digits = 0;
  }
  return *this;
}

mpint &mpint::operator /= (mpint const &x) {
  mpint_div q = div(*this, x);
  delete [] digits;
  if (q.quot.sign == 0)
    make_zero();
  else {
    move(q.quot);
    q.quot.digits = 0;
  }
  return *this;
}

mpint &mpint::operator %= (mpint const &x) {
  mpint_div q = div(*this, x);
  delete [] digits;
  if (q.rem.sign == 0)
    make_zero();
  else {
    move(q.rem);
    q.rem.digits = 0;
  }
  return *this;
}

long mpint::compare(mpint const &x) const {
  if (sign > x.sign)
    return 1;
  if (sign < x.sign)
    return -1;
  if (sign == 0)
    return 0;
  if (size > x.size)
    return sign;
  if (size < x.size)
    return -sign;
  return sign*fast_compare(digits, x.digits, size);
}

long mpint::abs_compare(mpint const &x) const {
  if (sign == 0 && x.sign == 0)
    return 0;
  if (size > x.size)
    return 1;
  if (size < x.size)
    return -1;
  return fast_compare(digits, x.digits, size);
}

mpint &mpint::negate() {
  sign = -sign;
  return *this;
}

mpint &mpint::add(mpint const &x) {
  if (x.sign != 0) {
    if (sign == 0) {
      delete [] digits;
      copy(x);
    }
    else if (sign == x.sign)
      abs_add(x);
    else {
      long t = abs_compare(x);
      if (t == 0) {
	delete [] digits;
	make_zero();
      }
      else if (t > 0)
	abs_subtract(x);
      else {
	sign = x.sign;
	abs_subtract_reverse(x);
      }
    }
  }
  return *this;
}

mpint &mpint::subtract(mpint const &x) {
  if (x.sign != 0) {
    if (sign == 0) {
      delete [] digits;
      copy(x);
      sign = -x.sign;
    }
    else if (sign != x.sign) {
      abs_add(x);
      normalize();
    }
    else {
      long t = abs_compare(x);
      if (t == 0) {
	delete [] digits;
	make_zero();
      }
      else if (t > 0) {
	abs_subtract(x);
	normalize();
      }
      else {
	sign = -sign;
	abs_subtract(x);
	normalize();
      }
    }
  }
  return *this;
}

void mpint::abs_add(mpint const &x) {
  long new_size;
  Digit *new_digits;
  
  if (size == x.size) {
    new_size = size+1;
    new_digits = new Digit[new_size];
    new_digits[size] = fast_add(new_digits, digits, x.digits, size, 0);
  }
  else if (size > x.size) {
    new_size = size+1;
    new_digits = new Digit[new_size];
    Digit c = fast_add(new_digits, digits, x.digits, x.size, 0);
    if (c)
      c = fast_carry(new_digits + x.size, digits + x.size, size - x.size);
    else
      fast_copy(new_digits + x.size, digits + x.size, size - x.size);
    new_digits[size] = c;
  }
  else {
    new_size = x.size+1;
    new_digits = new Digit[new_size];
    Digit c = fast_add(new_digits, digits, x.digits, size, 0);
    if (c)
      c = fast_carry(new_digits + size, x.digits + size, x.size - size);
    else
      fast_copy(new_digits + size, x.digits + size, x.size - size);
    new_digits[x.size] = c;
  }
  delete [] digits;
  size = new_size;
  digits = new_digits;
  normalize();
}

void mpint::abs_subtract(mpint const &x) {
  Digit c = fast_subtract(digits, digits, x.digits, x.size, 0);
  if (size != x.size)
    if (c)
      fast_borrow(digits + x.size, digits + x.size, size - x.size);
  normalize();
}
	
void mpint::abs_subtract_reverse(mpint const &x) {
  Digit *new_digits = new Digit[x.size];
  Digit c = fast_subtract(new_digits, x.digits, digits, size, 0);
  if (size != x.size) {
    if (c)
      fast_borrow(new_digits + size, x.digits + size, x.size - size);
    else
      fast_copy(new_digits + size, x.digits + size, x.size - size);
  }
  delete [] digits;
  size = x.size;
  digits = new_digits;
  normalize();
}

mpint &mpint::multiply(long x) {
  if (sign == 0)
    return *this;
  if (x == 0) {
    delete [] digits;
    make_zero();
    return *this;
  }
  unsigned long u;
  if (x > 0) {
    if (x == 1)
      return *this;
    u = x;
  }
  else { // x < 0
    sign *= -1;
    if (x == -1)
      return *this;
    u = -x;
  }
  long new_size = size+1;
  Digit *new_digits = new Digit[new_size];
  new_digits[new_size-1] = fast_multiply(new_digits, digits, size, u, 0);
  delete [] digits;
  size = new_size;
  digits = new_digits;
  normalize();
  return *this;
}

mpint &mpint::multiply(mpint const &x) {
  if (sign == 0)
    return *this;
  if (x.sign == 0) {
    delete [] digits;
    make_zero();
    return *this;
  }
  sign *= x.sign;
  if (x.size == 1 && x.digits[0] == 1) // if (x == 1)
    return *this;
  if (size == 1 && digits[0] == 1) { // if (*this == 1) {
    delete [] digits;
    copy(x);
    return *this;
  }
  long new_size = size + x.size;
  Digit *new_digits = new Digit[new_size];
  if (size == 1)
    new_digits[new_size-1] = fast_multiply(new_digits, x.digits, x.size, digits[0], 0);
  else if (x.size == 1)
    new_digits[new_size-1] = fast_multiply(new_digits, digits, size, x.digits[0], 0);
  else
    ::multiply(new_digits, digits, size, x.digits, x.size);
  delete [] digits;
  size = new_size;
  digits = new_digits;
  normalize();
  return *this;
}

mpint &mpint::divide(long x) {
  if (x == 0)
    throw std::runtime_error("division by zero");
  if (sign == 0)
    return *this;
  Digit u;
  if (x < 0) {
    u = -x;
    sign = -sign;
  }
  else
    u = x;
  fast_divide(digits, digits, size, u);
  normalize();
  return *this;
}

mpint_div div(mpint const &x, mpint const &y) {

  if (x == 0) {
    mpint_div rv = { mpint(0), mpint(0) };
    return rv;
  }
  if (y.size == 1) {
    Digit *xd = new Digit[x.size];
    Digit *yd = new Digit[1];
    
    yd[0] = fast_divide(xd, x.digits, x.size, y.digits[0]);
    mpint_div rv = { mpint(x.sign*y.sign, x.size, xd),
      mpint(x.sign*y.sign, 1, yd) };
    delete [] xd;
    delete [] yd;
    return rv;
  }
  
  // create copies of x.digits and y.digits since divide() changes them.
  
  Digit *xd = new Digit[x.size+1]; // divide_schoolboy() likes an extra digit for x
  fast_copy(xd, x.digits, x.size);
  xd[x.size] = 0;
  
  Digit *yd = new Digit[y.size];
  fast_copy(yd, y.digits, y.size);
  
  long m = x.size - y.size;
  
  Digit *q = new Digit[m + 1];

  divide_schoolboy(q, xd, yd, m, y.size);

  // quotiant is in q
  // remainder is in xd
  
  mpint_div rv = { mpint(x.sign*y.sign, m + 1, q),
                     mpint(x.sign*y.sign, y.size, xd) };
  delete [] yd;
  delete [] xd;
  delete [] q;
  
  return rv; // should use pointers? too much coping?
}

char const *dgts = "0123456789abcdefghijklmnopqrtsuvwxyz";


// convert integer x[n] to string in base base
// s points past the end of the char array!!!!!!

char *format_int(char *s, Digit *x, long n, long base) {
  *--s = '\0';
  while (!fast_is_zero(x, n)) {
    long k = fast_divide(x, x, n, base);
    if (0 <= k && k < 36)
      *--s = dgts[k];
    else
      *--s = '*';
  }
  return s;
}

void convert_to_string(char *s, unsigned long n, unsigned long base, long digits) {
  for (long i = digits-1; i >= 0; i--) {
    unsigned long k = n%base;
    n /= base;
    s[i] = dgts[k];
  }
}

unsigned long upow(unsigned long base, unsigned long exponent) {
  unsigned long rv = 1;
  while (exponent) {
    if (exponent&1)
      rv *= base;
    base *= base;
    exponent >>= 1;
  }
  return rv;
}

// convert fraction x[n] to string s[prec] in base base

void format_frac(char *s, Digit *x, long n, long base, long prec, bool trim) {
  char *sptr = s;

  long d = (long)floor(44.3614195558/log(base));
  unsigned long p = upow(base, d);
  Digit k;
  while (prec >= d) {
    k = fast_multiply(x, x, n, p, 0);
    convert_to_string(sptr, k, base, d);
    sptr += d;
    prec -= d;
  }
  if (prec) {
    p = upow(base, prec);
    k = fast_multiply(x, x, n, p, 0);
    convert_to_string(sptr, k, base, prec);
    sptr += prec;
  }
  /*
  while (prec--) {
    long k = fast_multiply(x, x, n, base, 0);
    if (0 <= k && k < 36)
      *sptr++ = dgts[k];
    else
      *sptr++ = '*';
  }
  */
  if (!trim)
    *sptr = '\0';
  else {
    while (*--sptr == '0')
      ;
    if (*sptr == '.')
      *sptr = '\0';
    else 
      *++sptr = '\0';
  }
}

std::string mpint::format(long base) const {
  if (sign == 0)
    return "0";
  if (base <= 1 || 36 < base)
    base = 10;
  long csize = (size+1)*(log(18446744073709551616.0)/log(base)) + 2.0;
  char *cstr = new char[csize];
  Digit *tdigits = new Digit[size];
  fast_copy(tdigits, digits, size);
  std::string rv(format_int(cstr+csize, tdigits, size, base));
  delete [] cstr;
  delete [] tdigits;
  return rv;
}

long mpfloat::precision = 10;

mpfloat::mpfloat() {
  make_zero();
}

mpfloat::mpfloat(int x) {
  if (x == 0)
    make_zero();
  else if (x < 0) {
    // convert to long, then negate, then convert to unsigned long
    make_from_digit((Digit)-(long)x);
    size = -1;
  }
  else
    make_from_digit(x);
}
    

mpfloat::mpfloat(long x) {
  if (x == 0)
    make_zero();
  else if (x < 0) {
    make_from_digit(-x);
    sign = -1;
  }
  else
    make_from_digit(x);
}

mpfloat::mpfloat(double x) {
  if (x == 0.0)
    make_zero();
  else
    make_from_double(x);
}

mpfloat::mpfloat(std::string const &s) {
  mpint x;
  long sgn = 1;

  auto cptr = s.begin();

  if (*cptr == '+')
    cptr++;
  else if (*cptr == '-') {
    sgn = -1;
    cptr++;
  }
  for (; cptr != s.end(); cptr++) {
    if (!isdigit(*cptr))
      break;
    x = 10*x + (*cptr - '0');
  }
  long p = 0;
  if (*cptr == '.') {
    cptr++;
    for (; cptr != s.end(); cptr++) {
      if (!isdigit(*cptr))
	break;
      x = 10*x + (*cptr - '0');
      p--;
    }
  }

  if (x == 0) {
    make_zero();
    return;
  }
  
  if (*cptr == 'e' || *cptr == 'E') {
    cptr++;
    long es = 1;
    if (*cptr == '+')
      cptr++;
    else {
      es = -1;
      cptr++;
    }
    long e = 0;
    while (isdigit(*cptr))
      e = 10*e + (*cptr++ - '0');
    p += es*e;
  }
  mpfloat y = mpfloat(x)*ipow(mpfloat(10), p);
  copy(sgn*y);
}

mpfloat::mpfloat(mpint const &x) {
  if (x == 0)
    make_zero();
  else {
    sign = x.sign;
    exponent = x.size - 1;
    size = x.size;
    digits = new Digit[size];
    fast_copy(digits, x.digits, size);
  }
}

mpfloat::mpfloat(mpfloat const &x) {
  if (x.sign == 0)
    make_zero();
  else
    copy(x);
}

mpfloat::mpfloat(mpfloat &&x) {
  if (x.sign == 0)
    make_zero();
  else {
    move(x);
    x.digits = 0;
  }
}

mpfloat::mpfloat(long sgn, long exp, long sz, Digit *d) {
  sign = sgn;
  exponent = exp;
  size = sz;
  digits = new Digit[size];
  fast_copy(digits, d, size);
}
  
mpfloat::~mpfloat() {
  delete [] digits;
}

void mpfloat::make_zero() {
  sign = 0;
  exponent = 0;
  size = 0;
  digits = 0;
}

// assume x > 0

void mpfloat::make_from_digit(Digit x) {
  sign = 1;
  exponent = 0;
  size = 1;
  digits = new Digit[1];
  digits[0] = x;
}

void mpfloat::make_from_double(double x) {
  union {
    double r;
    unsigned long u;
  } y;

  if (x > 0)
    sign = 1;
  else
    sign = -1;
  y.r = x;
  unsigned long e = ((0x7ff0000000000000&y.u)>>52) + 1;
  unsigned long f = (0xfffffffffffff&y.u)|0x10000000000000;
  exponent = (long)(e/64) - 16;
  unsigned long shift = e%64;
  if (shift < 52) {
    size = 2;
    digits = new Digit[2];
    digits[0] = f << (12 + shift);
    digits[1] = f >> (52 - shift);
  }
  else {
    size = 1;
    digits = new Digit[1];
    digits[0] = f << (shift - 52);
  }
}

void mpfloat::copy(mpfloat const &x) {
  sign = x.sign;
  exponent = x.exponent;
  size = x.size;
  digits = new Digit[size];
  fast_copy(digits, x.digits, size);
}

void mpfloat::move(mpfloat &x) {
  sign = x.sign;
  exponent = x.exponent;
  size = x.size;
  digits = x.digits;
  x.digits = 0;
}

void mpfloat::normalize() {
  while (size > 0 && digits[size-1] ==  0) {
    size--;
    exponent--;
  }
  if (size == 0) {
    delete [] digits;
    make_zero();
  }
  else {
    if (size > mpfloat::precision) {
      fast_copy(digits, &digits[size-mpfloat::precision], mpfloat::precision);
      size = mpfloat::precision;
    }
  }
}

mpfloat &mpfloat::operator = (mpfloat const &x) {
  delete [] digits;
  if (x.sign == 0)
    make_zero();
  else
    copy(x);
  return *this;
}

mpfloat &mpfloat::operator = (mpfloat &&x) {
  delete [] digits;
  if (x.sign == 0)
    make_zero();
  else {
    move(x);
    x.digits = 0;
  }
  return *this;
}

mpfloat &mpfloat::add(mpfloat const &x) {
  if (x.sign != 0) {
    if (sign == 0) {
      delete [] digits;
      copy(x);
    }
    else if (sign == x.sign) {
      abs_add(x);
      normalize();
    }
    else { // sign != x.sign
      long t = abs_compare(x);
      if (t == 0) {
	delete [] digits;
	make_zero();
      }
      else if (t > 0) {
	abs_subtract(x);
	normalize();
      }
      else {
	sign = x.sign;
	abs_subtract_reverse(x);
	normalize();
      }
    }
  }
  return *this;
}

mpfloat &mpfloat::subtract(mpfloat const &x) {
  if (x.sign != 0) {
    if (sign == 0) {
      delete [] digits;
      copy(x);
      sign = -sign;
    }
    else if (sign != x.sign) {
      abs_add(x);
      normalize();
    }
    else { // sign == x.sign
      long t = abs_compare(x);
      if (t == 0) {
	delete [] digits;
	make_zero();
      }
      else if (t > 0) {
	abs_subtract(x);
	normalize();
      }
      else {
	sign = -sign;
	abs_subtract(x);
	normalize();
      }
    }
  }
  return *this;
}

void mpfloat::abs_add(mpfloat const &x) {
  long new_exponent = std::max(exponent, x.exponent);
  long th = exponent;
  long tl = th - size;
  long xh = x.exponent;
  long xl = xh - x.size;
  
  long new_size = std::max(new_exponent - tl, new_exponent - xl);
  if (new_size > precision)
    new_size = precision;

  long nh = new_exponent;
  long nl = nh - new_size;
  
  Digit *new_digits = new Digit[new_size+1];
  fast_set(new_digits, new_size+1, 0);

  if (th > nl) {
    if (tl >= nl)
      fast_copy(new_digits + (tl - nl),  digits, size);
    else
      fast_copy(new_digits, digits + (nl - tl), th - nl);
  }
  
  if (xh > nl) {
    if (xl >= nl) {
      if (fast_add(new_digits + (xl - nl), new_digits + (xl - nl), x.digits, x.size, 0))
	fast_carry(new_digits + (xh - nl), new_digits + (xh - nl), nh - xh + 1);
    }
    else {
      if (fast_add(new_digits, new_digits, x.digits + (nl - xl), xh - nl, 0))
	fast_carry(new_digits + (xh - nl), new_digits + (xh - nl), nh - xh + 1);
    }
  }
  delete [] digits;
  exponent = new_exponent + 1;
  size = new_size + 1;
  digits = new_digits;
}

void mpfloat::abs_subtract(mpfloat const &x) {
  long ts = size;
  long xs = x.size;
  long te = exponent;
  long xe = x.exponent;
  
  if (te == xe) {
    while (digits[ts-1] == x.digits[xs-1]) {
      ts--;
      xs--;
      te--;
      xe--;
    }
  }

  long new_exponent = te;
  long th = te;
  long tl = th - ts;
  long xh = xe;
  long xl = xh - xs;

  long new_size = std::max(new_exponent - tl, new_exponent - xl);
  if (new_size > precision)
    new_size = precision;

  long nh = new_exponent;
  long nl = nh - new_size;

  Digit *new_digits = new Digit[new_size];
  fast_set(new_digits, new_size, 0);

  if (tl >= nl)
    fast_copy(new_digits + (tl - nl), digits, ts);
  else
    fast_copy(new_digits, digits + (nl - tl), th - nl);

  if (xh > nl) {
    if (xl >= nl) {
      if (fast_subtract(new_digits + (xl - nl), new_digits + (xl - nl), x.digits, xs, 0))
	fast_borrow(new_digits + (xh - nl), new_digits + (xh - nl), nh - xh);
    }
    else {
      if (fast_subtract(new_digits, new_digits, x.digits + (nl - xl), xh - nl, 0))
	fast_borrow(new_digits + (xh - nl), new_digits + (xh - nl), nh - xh);
    }
  }
  
  delete [] digits;
  exponent = new_exponent;
  size = new_size;
  digits = new_digits;
}

void mpfloat::abs_subtract_reverse(mpfloat const &x) {
  long ts = size;
  long xs = x.size;
  long te = exponent;
  long xe = x.exponent;
  
  if (te == xe) {
    while (digits[ts-1] == x.digits[xs-1]) {
      ts--;
      xs--;
      te--;
      xe--;
    }
  }

  long new_exponent = xe;
  long th = te;
  long tl = th - ts;
  long xh = xe;
  long xl = xh - xs;

  long new_size = std::max(new_exponent - tl, new_exponent - xl);
  if (new_size > precision)
    new_size = precision;

  long nh = new_exponent;
  long nl = nh - new_size;

  Digit *new_digits = new Digit[new_size];
  fast_set(new_digits, new_size, 0);

  if (xl >= nl)
    fast_copy(new_digits + (xl - nl), x.digits, xs);
  else
    fast_copy(new_digits, x.digits + (nl - xl), xh - nl);

  if (th > nl) {
    if (tl >= nl) {
      if (fast_subtract(new_digits + (tl - nl), new_digits + (tl - nl), digits, ts, 0))
	fast_borrow(new_digits + (th - nl), new_digits + (th - nl), nh - th);
    }
    else {
      if (fast_subtract(new_digits, new_digits, digits + (nl - tl), th - nl, 0))
	fast_borrow(new_digits + (th - nl), new_digits + (th - nl), nh - th);
    }
  }
  
  delete [] digits;
  exponent = new_exponent;
  size = new_size;
  digits = new_digits;
}

mpfloat &mpfloat::multiply(mpfloat const &x) {
  if (sign == 0)
    return *this;
  if (x.sign == 0) {
    delete [] digits;
    make_zero();
    return *this;
  }
  sign *= x.sign;
  if (x.size == 1 && x.digits[0] == 1) {
    exponent += x.exponent;
    return *this;
  }
  if (size == 1 && digits[0] == 1) {
    exponent += x.exponent;
    size = x.size;
    delete [] digits;
    digits = new Digit[size];
    fast_copy(digits, x.digits, size);
    return *this;
  }
  exponent += x.exponent + 1;
  long new_size = size + x.size;
  Digit *new_digits = new Digit[new_size];
  if (size == 1)
    new_digits[new_size-1] = fast_multiply(new_digits, x.digits, x.size, digits[0], 0);
  else if (x.size == 1)
    new_digits[new_size-1] = fast_multiply(new_digits, digits, size, x.digits[0], 0);
  else
    ::multiply(new_digits, digits, size, x.digits, x.size);

  delete [] digits;
  size = new_size;
  digits = new_digits;

  normalize();

  return *this;
}

mpfloat &mpfloat::divide(long x) {
  if (x == 0)
    throw std::runtime_error("division by zero");
  if (sign == 0)
    return *this;
  Digit u;
  if (x < 0) {
    sign = -sign;
    u = -x;
  }
  else
    u = x;
  if (u == 1)
    return *this;
  Digit *new_digits = new Digit[precision+1];
  if (size < precision+1) {
    Digit r = fast_divide(new_digits+precision-size+1, digits, size, u);
    fast_inverse(new_digits, r, u, precision - size + 1);
  }
  else
    fast_divide(new_digits, &digits[size-precision-1], precision+1, u);
  size = precision + 1;
  delete [] digits;
  digits = new_digits;
  normalize();
  return *this;
}

mpfloat &mpfloat::divide(mpfloat const &x) {
  if (x.sign == 0)
    throw std::runtime_error("division by zero");
  if (sign == 0)
    return *this;
  return multiply(::inverse(x));
}

long mpfloat::compare(mpfloat const &x) const {
  if (sign > x.sign)
    return 1;
  if (sign < x.sign)
    return -1;
  if (sign == 0)
    return 0;
  return sign*abs_compare(x);
}

long mpfloat::abs_compare(mpfloat const &x) const {
  if (exponent > x.exponent)
    return 1;
  if (exponent < x.exponent)
    return -1;
  if (size == x.size)
    return fast_compare(digits, x.digits, size);
  if (size > x.size) {
    long t = fast_compare(digits + (size - x.size), x.digits, x.size);
    if (t)
      return t;
    if (fast_is_zero(digits, size - x.size))
      return 0;
    return 1;
  }
  long t = fast_compare(digits, x.digits + (x.size - size), size);
  if (t)
    return t;
  if (fast_is_zero(x.digits, x.size - size))
    return 0;
  return -1;
}

double mpfloat::double_value() const {
  if (sign == 0)
    return 0.0;

  double x = digits[size-1]/base;
  if (size > 1)
    x += digits[size-2]/base2;
  return sign*ldexp(x,64*(exponent+1));
}

long mpfloat::ilog10() const {
  // don't use log10(double_value())
  //   -- double_value() may overflow even if log10(double_value()) is legit

  double x = digits[size-1]/base;
  if (size > 1)
    x += digits[size-2]/base2;
  x = log(x)/log(18446744073709551616.0) + (exponent+1);
  return (long)floor(x*log10(18446744073709551616.0));
}  

std::string mpfloat::format_fixed(long prec, char decimal, bool trim) const {
  if (sign == 0) {
    std::string s = "0";
    if (!trim && prec > 0) {
      s.push_back(decimal);
      while (prec-- > 0)
	s.push_back('0');
    }
    return s;
  }
  long csize;
  char *cstr;
  if (exponent < 0) {
    csize = prec + 3;
    cstr = new char[csize];
    cstr[0] = '0';
    if (prec <= 0)
      cstr[1] = '\0';
    else {
      cstr[1] = '.';
      Digit *tdigits = new Digit[-exponent + size - 1];
      fast_copy(tdigits, digits, size);
      if (exponent < -1)
	fast_set(tdigits, -exponent - 1, 0);
      format_frac(cstr+2, tdigits, -exponent + size - 1, 10, prec, trim);
      delete [] tdigits;
    }
  }
  else {
    csize = 20*(exponent + 1) + prec + 2;
    cstr = new char[csize];
    if (exponent >= size) {

      // ###0.0
      
      Digit *tdigits = new Digit[exponent+1];
      fast_set(tdigits, exponent - size + 1, 0);
      fast_copy(tdigits + exponent - size + 1, digits, size);
      char *tstr = format_int(cstr + csize, tdigits, exponent+1, 10);
      delete [] tdigits;
      long len = (cstr + csize) - tstr;  // length including terminating '\0'
      memmove(cstr, tstr, len); // use memmove since blocks may overlap!
      if (prec > 0) {
	cstr[len++] = '.';
	while (prec--)
	  cstr[len++] = '0';
	cstr[len] = '\0';
      }
    }
    else {
      Digit *tdigits = new Digit[size];
      fast_copy(tdigits, digits, size);

      // convert integer part
      
      char *tstr = format_int(cstr + csize, tdigits + (size - (exponent + 1)), exponent + 1, 10);
      long len = (cstr + csize) - tstr; // length including terminating '\0'
      memmove(cstr, tstr, len); // use memmove since blocks may overlap!
      tstr = cstr + len - 1;  // tstr now points to the end of cstr

      if (size == exponent + 1) {

	// XXX.0

	if (!trim && prec > 0) { // add zeros if necessary
	  *tstr++ = '.';
	  while (prec--)
	    *tstr++ = '0';
	  *tstr = '\0';
	}
      }
      else {
	*tstr++ = '.';
	format_frac(tstr, tdigits, size - (exponent + 1), 10, prec, trim);
      }
      delete [] tdigits;
    }
  }
  std::string rv(cstr);
  delete [] cstr;
  return rv;
}

std::string mpfloat::format_scientific(long prec, char decimal, bool trim) const {
  if (sign == 0) {
    std::string s("0"); // if *this == 0, hard code 0.0000e0
    if (!trim && prec > 0) {
      s.push_back('.');
      while (prec-- > 0)
	s.push_back('0');
    }
    s.push_back('e');
    s.push_back('0');
    return s;
  }
  long d = ilog10();
  mpfloat x;
  if (d != 0)
    x = (*this)/ipow(mpfloat(10), d);
  else
    x = (*this);
  if (x >= mpfloat(10)) {
    x /= 10;
    d++;
  }
  std::ostringstream s;
  s << x.format_fixed(prec, decimal, false) << 'e' << d;
  return s.str();
}

std::string mpfloat::format_default(long prec, char decimal) const {
  if (sign == 0)
    return "0";
  long d = ilog10();
  if (d > prec || d < -5)
    return format_scientific(prec, decimal, true);
  return format_fixed(prec, decimal, true);
}

mpfloat inverse(mpfloat const &x) {
  mpfloat u(1, -1, x.size, x.digits);
  mpfloat s = mpfloat(1.0/u.double_value());
  long save_precision = mpfloat::precision;
  mpfloat::precision = 4;
  s *= mpfloat(2) - s*u;
  for (; mpfloat::precision < save_precision; mpfloat::precision <<= 1)
    s *= mpfloat(2) - s*u;
  mpfloat::precision = save_precision;
  s *= mpfloat(2) - s*u;
  s *= mpfloat(2) - s*u;
  return mpfloat(x.sign, s.exponent - x.exponent - 1, s.size, s.digits);
}

mpfloat sqrt(mpfloat const &x) {
  return x*inverse_sqrt(x);
}

mpfloat inverse_sqrt(mpfloat const &x) {
  mpfloat u(1, 0 - (x.exponent&1), x.size, x.digits);
  mpfloat s = mpfloat(1.0/::sqrt(u.double_value()));
  long save_precision = mpfloat::precision;
  mpfloat::precision = 4;
  s *= (mpfloat(3) - s*s*u)/2;
  for (; mpfloat::precision < save_precision; mpfloat::precision <<= 1)
    s *= (mpfloat(3) - s*s*u)/2;
  s *= (mpfloat(3) - s*s*u)/2;
  s *= (mpfloat(3) - s*s*u)/2;
  return mpfloat(1, s.exponent - x.exponent/2, s.size, s.digits);
}


mpfloat ipow(mpfloat const &x, long y) {
  if (y == 0)
    return mpfloat(1);
  mpfloat w = 1;
  mpfloat u;
  unsigned long v;
  if (y > 0) {
    u = x;
    v = y;
  }
  else {
    u = inverse(x);
    v = -y;
  }
  while (v) {
    if (v&1)
      w *= u;
    u *= u;
    v >>= 1;
  }
  return w;
}

std::istream &operator >> (std::istream &in, mpint &x) {
  std::string s;
  int c = in.get();
  while (isspace(c))
    c = in.get();
  if (c == '+' || c == '-') {
    s.push_back((char)c);
    c = in.get();
  }
  while (isdigit(c)) {
    s.push_back((char)c);
    c = in.get();
  }
  x = mpint(s);
  in.unget();
  return in;
}

std::ostream &operator << (std::ostream &out, mpint const &x) {
  std::ios_base::fmtflags flags = out.flags();
  std::string s;
  
  int base = 10;
  if (flags&std::ios_base::oct)
    base = 8;
  else if (flags&std::ios_base::hex)
    base = 16;
  
  s = x.format(base); // no sign!
  
  std::string p;

  if (flags&std::ios_base::showbase) {
    if (flags&std::ios_base::oct) {
      if (s != "0") {
	p = "0";
      }
    }
    else if (flags&std::ios_base::hex) {
      p = "0x";
    }
  }
  
  if (x < 0)
    p = "-" + p;
  else if (flags&std::ios_base::showpos)
    p = "+" + p;
  
  if (flags&std::ios_base::uppercase)
    transform(s.begin(), s.end(), s.begin(), ::toupper);

  unsigned width = out.width(0);
  if (width <= s.size() + p.size())
    out << p << s;
  else {
    std::string fill(out.fill(), width - s.size());
    if (flags&std::ios_base::left)
      out << p << s << fill;
    else if (flags&std::ios_base::internal)
      out << p << fill << s;
    else
      out << fill << p << s;
  }
  out.width(width);
  return out;
}

std::ostream &operator << (std::ostream &out, mpfloat const &x) {
  std::ios_base::fmtflags flags = out.flags();
  char decimal_point = std::use_facet<std::numpunct<char>>(out.getloc()).decimal_point();
    
  std::string s;
  if (flags&std::ios_base::fixed)
    s = x.format_fixed(out.precision(), decimal_point, false);
  else if (flags &std::ios_base::scientific)
    s = x.format_scientific(out.precision(), decimal_point, false);
  else
    s = x.format_default(out.precision(), decimal_point);

  std::string p;
  
  if (x < 0)
    p = "-";
  else if (flags&std::ios_base::showpos)
    p = "+";

  unsigned width = out.width(0);
  if (width <= p.size() + s.size())
    out << p << s;
  else {
    std::string fill(out.fill(), width - s.size());
    if (flags&std::ios_base::left)
      out << p << s << fill;
    else if (flags&std::ios_base::internal)
      out << p << fill << s;
    else
      out << fill << p << s;
  }
  out.width(width);
  return out;
}

