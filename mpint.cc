#include <algorithm>
#include <sstream>
#include "digits.h"
#include "fast.h"
#include "mp.h"

using namespace std;

mpint::mpint() {
  make_zero();
};

mpint::~mpint() {
  delete [] digits;
}

void mpint::make_zero() {
  sign = 0;
  size = 0;
  digits = 0;
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
  int xsize = size;
  stringstream ssr(stringstream::out);

  while (xsize) {
    int c = fast_divide(xdigits, xdigits, 10, xsize);
    ssr << char(c + '0');
    if (xdigits[xsize - 1] == 0)
      xsize--;
  }
  delete [] xdigits;
  string s = ssr.str();
  reverse(s.begin(), s.end());
  ss << s;
  return ss.str();
}
