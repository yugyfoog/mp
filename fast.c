#include "digits.h"
#include "fast.h"

typedef unsigned Digit2;

void fast_clear(Digit *x, int n) {
  while (n--)
    *x++ = 0;
}

void fast_copy(Digit *y, Digit const *x, int n) {
  while (n--)
    *y++ = *x++;
}

int fast_is_zero(Digit *x, int n) {
  while (n--) {
    if (*x++ != 0)
      return 0;
  }
  return 1;
}

// y[] = x*y[] + k

Digit2 x_fast_multiply_add(Digit2 *y, int n, Digit2 x, Digit k) {
  while (n--) {
    k += (Digit)x * *y;
    *y++ = k;
    k >>= 32;
  }
  return k;
}

Digit fast_multiply_add(Digit *y, int n, Digit x, Digit k) {
  return x_fast_multiply_add((Digit2 *)y, 2*n, x, k);
}

// y /= x

Digit2 x_fast_divide(Digit2 *y, int n, Digit2 x) {
  Digit k = 0;
  for (y += n; n--; k %= x) {
    k = (k << 32) + *--y;
    *y = k/x;
  }
  return k % x;
}

Digit fast_divide(Digit *y, int n, Digit x) {
  return x_fast_divide((Digit2 *)y, 2*n, x);
}

