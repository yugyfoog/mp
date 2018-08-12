#include <algorithm>
#include <sstream>
#include <iomanip>
#include "digits.h"
#include "fast.h"
#include "mp.h"

using namespace std;

mpint::mpint() {
  make_zero();
};

mpint::mpint(int x) {
  make_from_int(x);
}

mpint::mpint(unsigned x) {
  make_from_unsigned(x);
}

mpint::mpint(long x) {
  make_from_long(x);
}

mpint::mpint(unsigned long x) {
  make_from_unsigned_long(x);
}

mpint::mpint(string const &x) {
  string::const_iterator xp = x.begin();
  if (*xp == '-') {
    sign = -1;
    xp++;
  }
  else
    sign = 1;
  size = x.length()/DIGITS_PER_WORD + 2;
  digits = new Digit[size];
  fast_clear(digits, size);
  while (isdigit(*xp)) {
    fast_multiply_add(digits, size, 10, *xp - '0');
    xp++;
  }
  normalize();
}

mpint::mpint(mpint const &x) {
  copy(x);
}

void mpint::copy(mpint const &x) {
  if (x.sign == 0)
    make_zero();
  else {
    sign = x.sign;
    size = x.size;
    digits = new Digit[size];
    fast_copy(digits, x.digits, size);
  }
}

mpint::~mpint() {
  delete [] digits;
}

void mpint::make_zero() {
  sign = 0;
  size = 0;
  digits = 0;
}

void mpint::make_from_int(int x) {
  if (x == 0)
    make_zero();
  else if (x < 0) {
    make_from_digit(-(long)x);
    sign = -1;
  }
  else {
    make_from_digit(x);
    sign = 1;
  }
}

void mpint::make_from_unsigned(unsigned x) {
  if (x == 0)
    make_zero();
  else {
    make_from_digit(x);
    sign = 1;
  }
}

void mpint::make_from_long(long x) {
  if (x == 0)
    make_zero();
  else if (x < 0) {
    make_from_digit(-x);
    sign = -1;
  }
  else {
    make_from_digit(x);
    sign = 1;
  }
}

void mpint::make_from_unsigned_long(unsigned long x) {
  if (x == 0)
    make_zero();
  else {
    make_from_digit(x);
    sign = 1;
  }
}

void mpint::make_from_digit(Digit x) {
  size = 1;
  sign = 1;
  digits = new Digit[1];
  digits[0] = x;
}

void mpint::normalize() {
  Digit *dptr;
  if (size) {
    for (dptr = digits + size - 1; dptr >= digits; dptr--) {
      if (*dptr != 0)
	break;
    }
    if (dptr < digits) {
      delete [] digits;
      make_zero();
    }
    else
      size = dptr - digits + 1;
  }
}

mpint &mpint::operator = (int x) {
  delete [] digits;
  make_from_int(x);
  return *this;
}

mpint &mpint::operator = (unsigned x) {
  delete [] digits;
  make_from_unsigned(x);
  return *this;
}

mpint &mpint::operator = (long x) {
  delete [] digits;
  make_from_long(x);
  return *this;
}


mpint &mpint::operator = (unsigned long x) {
  delete [] digits;
  make_from_unsigned_long(x);
  return *this;
}

mpint &mpint::operator = (mpint const &x) {
  if (this != &x) {
    delete [] digits;
    copy(x);
  }
  return *this;
}

string mpint::to_string() const {
  stringstream ss(stringstream::out);
  if (sign == 0) {
    ss << "0";
    return ss.str();
  }
  if (sign < 0)
    ss << '-';
  Digit *xdigits = new Digit[size];
  fast_copy(xdigits, digits, size);
  stringstream ssr(stringstream::out);
  while (!fast_is_zero(xdigits, size)) {
    int c = fast_divide(xdigits, size, 10);
    ssr << char(c + '0');
  }
  delete [] xdigits;
  string s = ssr.str();
  reverse(s.begin(), s.end());
  ss << s;
  return ss.str();
}

string mpint::to_hex() const {
  stringstream ss(stringstream::out);
  ss << "sign = " << sign << endl;
  ss << "size = " << size << endl;
  ss << "digits = ";
  for (int i = 0; i < size; i++)
    ss << setfill('0') << setw(16) << hex << digits[i] << ' ';
  ss << endl;
  return ss.str();
}

bool mpint::iszero() const {
  return sign == 0;
}

mpint &mpint::add(mpint const &x) {
  if (x.iszero())
    ;
  else if (iszero()) {
    delete [] digits;
    copy(x);
  }
  else if (sign == x.sign)
    abs_add(x);
  else {
    int t = abs_compare(x);
    if (t == 0) {
      make_zero();
      return *this;
    }
    else if (t < 0) { /* this < x */
      abs_subtract_reverse(x);
      sign = x.sign;
    }
    else
      abs_subtract(x);
  }
  normalize();
  return *this;
}

int mpint::abs_compare(mpint const &x) const {
  if (size < x.size)
    return -1;
  if (size > x.size)
    return 1;
  return fast_compare(digits, x.digits, size);
}

mpint &mpint::abs_add(mpint const &x) {
  Digit *new_digits;

  cout << "size = " << size << ", " << x.size << endl;
  
  if (size >= x.size) {
    new_digits = new Digit[size+1];
    Digit k = fast_add(new_digits, digits, x.digits, x.size, 0);
    new_digits[size] = fast_carry(new_digits+x.size, digits+x.size, size - x.size, k);
    size++;
  }
  else {
    new_digits = new Digit[x.size+1];
    Digit k = fast_add(new_digits, digits, x.digits, size, 0);
    new_digits[x.size] = fast_carry(new_digits+size, x.digits+size, x.size - size, k);
    size = x.size+1;
  }
  delete [] digits;
  digits = new_digits;
  return *this;
}
      
mpint &mpint::abs_subtract(mpint const &x) {
  Digit k = fast_subtract(digits, digits, x.digits, x.size, 0);
  fast_borrow(digits+x.size, digits+x.size, size - x.size, k);
  return *this;
}
  
mpint &mpint::abs_subtract_reverse(mpint const &x) {
  Digit *new_digits = new Digit[x.size];
  SDigit k = fast_subtract(new_digits, x.digits, digits, size, 0);
  fast_borrow(new_digits+size, x.digits+size, x.size - size, k);
  delete [] digits;
  digits = new_digits;
  size = x.size;
  return *this;
}

  
