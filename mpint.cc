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

mpint::mpint(unsigned x) {
  if (x == 0)
    make_zero();
  else {
    make_from_digit(x);
    sign = 1;
  }
}

mpint::mpint(long x) {
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

mpint::mpint(unsigned long x) {
  if (x == 0)
    make_zero();
  else {
    make_from_digit(x);
    sign = 1;
  }
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

void mpint::make_from_digit(Digit x) {
  size = 1;
  sign = 1;
  digits = new Digit[1];
  digits[0] = x;
}

void mpint::normalize() {
  Digit *dptr;
  
  for (dptr = digits + size; dptr != digits; dptr--) {
    if (*dptr != 0)
      break;
  }
  if (dptr == digits) {
    delete [] digits;
    make_zero();
  }
  else
    size = dptr - digits;
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
