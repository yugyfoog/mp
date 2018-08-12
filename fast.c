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

int fast_is_zero(Digit const *x, int n) {
  while (n--) {
    if (*x++ != 0)
      return 0;
  }
  return 1;
}

int fast_compare(Digit *x, Digit *y, int n) {
  x += n;
  y += n;
  while (n--) {
    if (*--x > *--y)
      return 1;
    if (*x < *y)
      return -1;
  }
  return 0;
}

Digit x_fast_add(Digit2 *z, Digit2 *x, Digit2 *y, int n, Digit k) {
  while (n--) {
    k += (Digit)*x++ + *y++;
    *z++ = k;
    if (k < 0xffffffff)
      k = 0;
    else
      k = 1;
  }
  return k;
}

Digit fast_add(Digit *z, Digit *x, Digit *y, int n, Digit k) {
  return x_fast_add((Digit2 *)z, (Digit2 *)x, (Digit2 *)y, 2*n, k);
}

Digit x_fast_carry(Digit2 *z, Digit2 *x, int n, Digit k) {
  while (n--) {
    k += (Digit)*x++;
    *z++ = k;
    if (k < 0xffffffff)
      k = 0;
    else
      k = 1;
  }
  return k;
}

Digit fast_carry(Digit *z, Digit *x, int n, Digit k) {
  return x_fast_carry((Digit2 *)z, (Digit2 *)x, 2*n, k);
}

SDigit x_fast_subtract(Digit2 *z, Digit2 *x, Digit2 *y, int n, SDigit k) {
  while (n--) {
    k += (SDigit)*x++ - *y++;
    *z++ = k;
    if (k < 0)
      k = -1;
    else
      k = 0;
  }
  return k;
}

SDigit fast_subtract(Digit *z, Digit *x, Digit *y, int n, SDigit k) {
  return x_fast_subtract((Digit2 *)z, (Digit2 *)x, (Digit2 *)y, 2*n, k);
}

SDigit x_fast_borrow(Digit2 *z, Digit2 *x, int n, SDigit k) {
  while (n--) {
    k += (SDigit)*x++;
    *z++ = k;
    if (k < 0)
      k = -1;
    else
      k = 0;
  }
  return k;
}

SDigit fast_borrow(Digit *z, Digit *x, int n, SDigit k) {
  return x_fast_borrow((Digit2 *)z, (Digit2 *)x, 2*n, k);
}

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

