#ifndef _MPINT_H_ // -*- C++ -*-
#define _MPINT_H_

class mpint {

private:
  int sign;
  int size;
  Digit *digits;
  
public:
  mpint();
  mpint(long);
  mpint(unsigned long);
  mpint(double);
  mpint(char const *);
  mpint(mpint const &);
  ~mpint();
  mpint &operator = (long);
  mpint &operator = (unsigned long);
  mpint &operator = (mpint const &);
  mpint &operator += (long);
  mpint &operator += (unsigned long);
  mpint &operator += (mpint const &);
  mpint &operator -= (long);
  mpint &operator -= (unsigned long);
  mpint &operator -= (mpint const &);
  mpint &operator *= (long);
  mpint &operator *= (unsigned long);
  mpint &operator *= (mpint const &);
  mpint &operator /= (long);
  mpint &operator /= (unsigned long);
  mpint &operator /= (mpint const &);
  mpint &operator %= (long);
  mpint &operator %= (unsigned long);
  mpint &operator %= (mpint const &);

  std::string to_string() const;
  
private:
  void make_zero();
};

#endif
