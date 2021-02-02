// -*- C++ -*-

extern "C" void fast_set(Digit *, long, Digit);
extern "C" void fast_copy(Digit *, Digit *, long);
extern "C" long fast_is_zero(Digit *, long);
extern "C" long fast_compare(Digit *, Digit *, long);
extern "C" Digit fast_add(Digit *, Digit *, Digit *, long, Digit);
extern "C" void fast_add_carry(Digit *, Digit *, long, Digit);
extern "C" Digit fast_carry(Digit *, Digit *, long);
extern "C" Digit fast_subtract(Digit *, Digit *, Digit *, long, Digit);
extern "C" Digit fast_subtract_borrow(Digit *, Digit *, long, Digit);
extern "C" Digit fast_borrow(Digit *, Digit *, long);
extern "C" Digit fast_multiply(Digit *, Digit *, long, Digit, Digit);
extern "C" Digit fast_divide(Digit *, Digit *, long, Digit);
extern "C" Digit fast_inverse(Digit *, Digit, Digit, long);
