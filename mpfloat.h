#ifndef _MPFLOAT_H_ // -*- C++ -*-
#define _MPFLOAT_H_

class mpfloat {
public:
  mpfloat();
  mpfloat(long);
  mpfloat(unsigned long);
  mpfloat(double);
  mpfloat(char const *);
  mpfloat(mpint const &);
  mpfloat(mpfloat const &);
  ~mpfloat();
  mpfloat &operator = (long);
  mpfloat &operator = (unsigned long);
  mpfloat &operator = (mpint const &);
  mpfloat &operator = (mpfloat const &);
  mpfloat &operator += (long);
  mpfloat &operator += (unsigned long);
  mpfloat &operator += (mpint const &);
  mpfloat &operator += (mpfloat const &);
  mpfloat &operator -= (long);
  mpfloat &operator -= (unsigned long);
  mpfloat &operator -= (mpint const &);
  mpfloat &operator -= (mpfloat const &);
  mpfloat &operator *= (long);
  mpfloat &operator *= (unsigned long);
  mpfloat &operator *= (mpint const &);
  mpfloat &operator *= (mpfloat const &);
  mpfloat &operator /= (long);
  mpfloat &operator /= (unsigned long);
  mpfloat &operator /= (mpint const &);
  mpfloat &operator /= (mpfloat const &);
  mpfloat &operator %= (long);
  mpfloat &operator %= (unsigned long);
  mpfloat &operator %= (mpint const &);
  mpfloat &operator %= (mpfloat const &);
  mpfloat &operator - ();
};

mpfloat &operator + (mpint const &, mpfloat const &);
mpfloat &operator + (mpfloat const &, mpint const &);
mpfloat &operator + (mpfloat const &, mpfloat const &);

#endif
