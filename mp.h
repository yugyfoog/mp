#define XXX() undefined(__FUNCTION__,__FILE__,__LINE__)
  
typedef unsigned long Digit;
  
// #include "mynew.h"
#include "fast.h"
#include "multiply.h"
#include "schoolboy.h"

class mpint;
class mpfloat;

static double const base = 18446744073709551616.0;
static double const base2 = 340282366920938463463374607431768211456.0;

class mpint {
public:
  mpint();
  mpint(long);
  mpint(std::string const &);
  mpint(mpint const &);
  mpint(mpint &&);
  mpint(long, long, Digit *);
  
  ~mpint();

  mpint &operator = (mpint const &);
  mpint &operator = (mpint &&);
  mpint &operator += (mpint const &x) { return add(x); }
  mpint &operator -= (mpint const &x) { return subtract(x); }
  mpint &operator *= (long x) { return multiply(x); }
  mpint &operator *= (mpint const &x) { return multiply(x); }
  mpint &operator /= (long x) { return divide(x); }
  mpint &operator /= (mpint const &);
  mpint &operator %= (mpint const &);
    
  long compare(mpint const &) const;

  mpint &negate();
  mpint &add(mpint const &);
  mpint &subtract(mpint const &);
  mpint &multiply(long);
  mpint &multiply(mpint const &);
  mpint &divide(long);
  long remainder(long) const;
  mpint &shift_left(long);
  mpint &shift_right(long);
  
  std::string format(long) const;
  
private:

  long sign;
  long size;
  Digit *digits;
  
  void make_zero();
  void make_from_digit(Digit);
  void copy(mpint const &);
  void move(mpint const &);
  void normalize();

  long abs_compare(mpint const &) const;
  void abs_add(mpint const &);
  void abs_subtract(mpint const &);
  void abs_subtract_reverse(mpint const &);
  
  friend struct mpint_div div(mpint const &, mpint const &);
  friend class mpfloat;
};

typedef struct mpint_div {
  mpint quot;
  mpint rem;
} mpint_div;

mpint_div div(mpint const &, mpint const &);

inline mpint operator + (mpint const &x) {
  mpint y(x);
  return y;
}

inline mpint operator - (mpint const &x) {
  mpint y(x);
  return y.negate();
}

inline mpint operator + (mpint const &x, mpint const &y) {
  mpint z(x);
  return z.add(y);
}

inline mpint operator - (mpint const &x, mpint const &y) {
  mpint z(x);
  return z.subtract(y);
}

inline mpint operator * (long x, mpint const &y) {
  mpint z(y);
  return z.multiply(x);
}

inline mpint operator * (mpint const &x, long y) {
  mpint z(x);
  return z.multiply(y);
}

inline mpint operator * (mpint const &x, mpint const &y) {
  mpint z(x);
  return z.multiply(y);
}

inline mpint operator / (mpint const &x, long y) {
  mpint z(x);
  return z.divide(y);
}

inline mpint operator / (mpint const &x, mpint const &y) {
  return div(x, y).quot;
}

inline mpint operator % (mpint const &x, mpint const &y) {
  return div(x, y).rem;
}

inline bool operator == (mpint const &x, mpint const &y) {
  return x.compare(y) == 0;
}

inline bool operator != (mpint const &x, mpint const &y) {
  return x.compare(y) != 0;
}

inline bool operator < (mpint const &x, mpint const &y) {
  return x.compare(y) < 0;
}

inline bool operator <= (mpint const &x, mpint const &y) {
  return x.compare(y) <= 0;
}

inline bool operator > (mpint const &x, mpint const &y) {
  return x.compare(y) > 0;
}

inline bool operator >= (mpint const &x, mpint const &y) {
  return x.compare(y) >= 0;
}

void div(mpint &, mpint &, mpint const &, mpint const &);

std::istream &operator >> (std::istream &, mpint &);
std::ostream &operator << (std::ostream &, mpint const &);

class mpfloat {
public:
  
  mpfloat();
  mpfloat(int);
  mpfloat(long);
  mpfloat(double);
  mpfloat(std::string const &);
  mpfloat(mpint const &);
  mpfloat(mpfloat const &);
  mpfloat(mpfloat &&);
  ~mpfloat();
  
  mpfloat &operator = (mpfloat const &);
  mpfloat &operator = (mpfloat &&);
  mpfloat &operator += (mpfloat const &x) { add(x); return *this; }
  mpfloat &operator -= (mpfloat const &x) { subtract(x); return *this; }
  mpfloat &operator *= (mpfloat const &x) { multiply(x); return *this; }
  mpfloat &operator /= (long x) { divide(x); return *this; }
  mpfloat &operator /= (mpfloat const &x) { divide(x); return *this; }
  mpfloat &add(mpfloat const &);
  mpfloat &subtract(mpfloat const &);
  mpfloat &multiply(mpfloat const &);
  mpfloat &inverse();
  mpfloat &divide(long);
  mpfloat &divide(mpfloat const &);
  
  long compare(mpfloat const &) const;
  
  double double_value() const;

  static long precision;

  std::string format_default(long, char) const;
  std::string format_fixed(long, char, bool) const;
  std::string format_scientific(long, char, bool) const;
  
private:

  long sign;
  long exponent;
  long size;
  Digit *digits;

  mpfloat(long, long, long, Digit *);
  
  void make_zero();
  void make_from_digit(Digit);
  void make_from_double(double x);
  void copy(mpfloat const &);
  void move(mpfloat &);
  void normalize();
  
  long abs_compare(mpfloat const &) const;
  void abs_add(mpfloat const &);
  void abs_subtract(mpfloat const &);
  void abs_subtract_reverse(mpfloat const &);

  long ilog10() const;

  friend mpfloat inverse(mpfloat const &);
  friend mpfloat inverse_sqrt(mpfloat const &);

};

mpfloat ipow(mpfloat const &, long);
  
inline bool operator == (mpfloat const &x, mpfloat const &y) {
  return x.compare(y) == 0;
}

inline bool operator != (mpfloat const &x, mpfloat const &y) {
  return x.compare(y) != 0;
}

inline bool operator < (mpfloat const &x, mpfloat const &y) {
  return x.compare(y) < 0;
}

inline bool operator <= (mpfloat const &x, mpfloat const &y) {
  return x.compare(y) <= 0;
}

inline bool operator > (mpfloat const &x, mpfloat const &y) {
  return x.compare(y) > 0;
}

inline bool operator >= (mpfloat const &x, mpfloat const &y) {
  return x.compare(y) >= 0;
}

inline mpfloat operator + (mpfloat const &x, mpfloat const &y) {
  mpfloat z(x);
  return z += y;
}

inline mpfloat operator - (mpfloat const &x, mpfloat const &y) {
  mpfloat z(x);
  return z -= y;
}

inline mpfloat operator * (mpfloat const &x, mpfloat const &y) {
  mpfloat z(x);
  return z *= y;
}

inline mpfloat operator / (mpfloat const &x, long y) {
  mpfloat z(x);
  return z /= y;
}

inline mpfloat operator / (mpfloat const &x, mpfloat const &y) {
  mpfloat z(x);
  return z /= y;
}

mpfloat inverse(mpfloat const &);
mpfloat sqrt(mpfloat const &);
mpfloat inverse_sqrt(mpfloat const &);

std::istream &operator >> (std::istream &, mpfloat &);
std::ostream &operator << (std::ostream &, mpfloat const &);

void undefined(char const *, char const *, int);
void mperror(std::string const &);

