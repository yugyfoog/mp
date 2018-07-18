#ifndef _MP_H_ // -*- C++ -*-
#define _MP_H_

#include <iostream>
#include "digits.h"


class mpint;
class mpfloat;

#include "mpint.h"
#include "mpfloat.h"

std::ostream &operator << (std::ostream &, mpint const &);

#endif
