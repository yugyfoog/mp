#include "mp.h"

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
