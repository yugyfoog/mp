#include <iostream>
#include "mp.h"

using namespace std;

ostream &operator << (ostream &o, mpint const &x) {
  ios_base::fmtflags ff = o.flags();
  int adjustfield = ff & ios_base::adjustfield;

  size_t field_width = o.width();
  char fill_char = o.fill();

  string s;

  s = x.to_string();
  if (s.length() > field_width)
    o << s;
  else {
    if (adjustfield == ios_base::left) {
      o << s;
      for (size_t i = s.length(); i < field_width; i++)
	o.put(fill_char);
    }
    else {
      for (size_t i = s.length(); i < field_width; i++)
	o.put(fill_char);
      o << s;
    }
  }
  return o;
}
