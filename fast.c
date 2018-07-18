#include "digits.h"
#include "fast.h"

typedef unsigned Digit2;

void fast_copy(Digit *y, Digit const *x, int n) {
  while (n--)
    *y++ = *x++;
}

Digit2 x_fast_divide(Digit2 *z, Digit2 const *x, Digit2 y, int n) {
  Digit k = 0;
  for (x += n, z += n; n--; k %= y) {
    k = (k << 32) + *--x;
    *--z = k/y;
  }
  return k % y;
}

Digit fast_divide(Digit *z, Digit const *x, Digit y, int n) {
  return x_fast_divide((Digit2 *)z, (Digit2 const *)x, y, 2*n);
}

