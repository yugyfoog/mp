#ifndef _MPINT_H_ // -*- C++ -*-
#define _MPINT_H_

class mpint {

private:
  int sign;
  int size;
  Digit *digits;
  
public:
  mpint();
  mpint(int);
  mpint(unsigned);
  mpint(long);
  mpint(unsigned long);
  mpint(std::string const &);
  mpint(mpint const &);
  ~mpint();
  mpint &operator = (int);
  mpint &operator = (unsigned);
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
  std::string to_hex() const;
private:
  void copy(mpint const &);
  void make_zero();
  void make_from_int(int);
  void make_from_unsigned(unsigned);
  void make_from_long(long);
  void make_from_unsigned_long(unsigned long);
  void make_from_digit(Digit);
  void normalize();
};

#endif
