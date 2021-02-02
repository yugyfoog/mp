#include <string>
#include "mp.h"

extern "C" void multiply_schoolboy(Digit *, Digit *, long, Digit *, long);

const long sblimit = 16; // tune
const long klimit = 16; // tune

void split(Digit *, Digit *, long, Digit *, long);
void karatsuba(Digit *, Digit *, Digit *, long, Digit *);

void multiply(Digit *W, Digit *U, long m, Digit *V, long n) {
  if (m <= sblimit)
    multiply_schoolboy(W, U, m, V, n);
  else if (n <= sblimit)
    multiply_schoolboy(W, U, m, V, n);
  else {
    long pn, pm;
    
    if (m >= n) {
      pn = 1;
      while (pn < n)
	pn <<= 1;
      pm = pn;
      while (pm < m)
	pm += pn;
    }
    else {
      pm = 1;
      while (pm < m)
	pm <<= 1;
      pn = pm;
      while (pn < n)
	pn += pm;
    }
    
    Digit *U0 = new Digit[pm];
    fast_copy(U0, U, m);
    if (pm > m)
      fast_set(U0+m, pm - m, 0);
    
    Digit *V0 = new Digit[pn];
    fast_copy(V0, V, n);
    if (pn > n)
      fast_set(V0+n, pn - n, 0);
    
    Digit *W0 = new Digit[pm+pn];
    fast_set(W0, pm+pn, 0);

    if (pm  > pn)
      split(W0, U0, pm, V0, pn);
    else if (pm < pn)
      split(W0, V0, pn, U0, pm);
    else {
      Digit *Work = new Digit[4*pm];
      karatsuba(W0, U0, V0, pm, Work);
      delete [] Work;
    }

    fast_copy(W, W0, m+n);

    delete [] U0;
    delete [] V0;
    delete [] W0;
  }
}

void split(Digit *W, Digit *U, long m, Digit *V, long n) {
  long k = m/n;
  
  Digit *P = new Digit[2*n];
  Digit *U0 = U;
  Digit *W0 = W;
  Digit *Work = new Digit[4*n];
  while (k--) {
    fast_set(P, 2*n, 0);
    karatsuba(P, U0, V, n, Work);
    fast_add_carry(W0, P, 2*n, 0);
    U0 += n;
    W0 += n;
  }
  delete [] Work;
  delete [] P;
}

void karatsuba(Digit *W, Digit *U, Digit *V, long n, Digit *Work) {
  if (n <= klimit)
    multiply_schoolboy(W, U, n, V, n);
  else {
    Digit *U0 = U;
    Digit *U1 = U + n/2;
    Digit *V0 = V;
    Digit *V1 = V + n/2;
    Digit *W0 = W;
    Digit *W1 = W + n/2;
    Digit *W2 = W + n;

    Digit *T0 = Work;
    Digit *S0 = T0 + n;
    Digit *S1 = S0 + n/2;
    Work = S1 + n/2;
   
    fast_set(T0, n, 0);
    karatsuba(T0, U0, V0, n/2, Work);
    fast_add_carry(W0, T0, n, 0);
    fast_add_carry(W1, T0, n, 0); 

    fast_set(T0, n, 0);
    karatsuba(T0, U1, V1, n/2, Work);
    fast_add_carry(W1, T0, n, 0);
    fast_add_carry(W2, T0, n, 0);
    
    long t0 = fast_compare(U1, U0, n/2);
    if (t0) {
      long t1 = fast_compare(V1, V0, n/2);
      if (t1) {

	if (t0 > 0)
	  fast_subtract(S0, U1, U0, n/2, 0);
	else
	  fast_subtract(S0, U0, U1, n/2, 0);
	
	if (t1 > 0)
	  fast_subtract(S1, V1, V0, n/2, 0);
	else
	  fast_subtract(S1, V0, V1, n/2, 0);

	fast_set(T0, n, 0);
	karatsuba(T0, S0, S1, n/2, Work);
	
	if ((t0 < 0 && t1 < 0) || (t0 > 0 && t1 > 0))
	  fast_subtract_borrow(W1, T0, n, 0);
	else
	  fast_add_carry(W1, T0, n, 0);
      }
    }
  }
}
