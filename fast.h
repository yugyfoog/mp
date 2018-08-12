#ifndef _FAST_H_ // -*- C++ -*-
#define _FAST_H_

#ifdef __cplusplus
extern "C" {
#endif

  void fast_clear(Digit *, int);
  void fast_copy(Digit *, Digit const *, int);
  int fast_is_zero(Digit const *, int);
  int fast_compare(Digit *, Digit *, int);
  Digit fast_add(Digit *, Digit *, Digit *, int, Digit);
  Digit fast_carry(Digit *, Digit *, int, Digit);
  SDigit fast_subtract(Digit *, Digit *, Digit *, int, SDigit);
  SDigit fast_borrow(Digit *, Digit *, int, SDigit);
  Digit fast_multiply_add(Digit *, int, Digit, Digit);
  Digit fast_divide(Digit *, int, Digit);
  
#ifdef __cplusplus
};
#endif
#endif
