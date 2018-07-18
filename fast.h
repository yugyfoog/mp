#ifndef _FAST_H_ // -*- C++ -*-
#define _FAST_H_

#ifdef __cplusplus
extern "C" {
#endif

  void fast_copy(Digit *, Digit const *, int);
  Digit fast_divide(Digit *, Digit const *, Digit, int);
  
#ifdef __cplusplus
};
#endif
#endif
