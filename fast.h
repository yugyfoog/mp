#ifndef _FAST_H_ // -*- C++ -*-
#define _FAST_H_

#ifdef __cplusplus
extern "C" {
#endif

  void fast_clear(Digit *, int);
  void fast_copy(Digit *, Digit const *, int);
  int fast_is_zero(Digit *, int);
  Digit fast_multiply_add(Digit *, int, Digit, Digit);
  Digit fast_divide(Digit *, int, Digit);
  
#ifdef __cplusplus
};
#endif
#endif
