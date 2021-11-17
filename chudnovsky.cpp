#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <cstring>
#include "mp.h"

const double DIGITS_PER_TERM = 14.1816474627254776555;
const long INIT_FACS = 32;
const double DIGITS_PER_WORD = 19.2659197225;

typedef struct {
  long fac;
  long pow;
  long nxt;
} Sieve;

typedef struct {
  long max_facs;
  long num_facs;
  long *fac;
  long *pow;
} Fac;

long digits;

long top = 0;
 
mpint *pstack;
mpint *qstack;
mpint *gstack;
Fac *fpstack;
Fac *fgstack;

Sieve *sieve;

Fac ftmp;
Fac fml;
mpint gcd;

void options(int, char **);
void build_sieve(long, Sieve *);
void binary_split(long, long, bool, long);
void fac_init(Fac *);
void fac_init_size(Fac *, long);
void fac_clear(Fac *);
void fac_resize(Fac *, long);
void fac_set_bp(Fac *, long, long);
void fac_mul(Fac *, Fac *);
void fac_mul2(Fac *, Fac *, Fac *);
void fac_mul_bp(Fac *, long, long);
void fac_remove_gcd(mpint *, Fac *, mpint *, Fac *);
void fac_compact(Fac *);
void bs_mul(mpint &, long, long);
void fac_show(Fac *);

int main(int argc, char **argv) {
  options(argc, argv);
  long terms = digits/DIGITS_PER_TERM;
  long depth = 1;

  while ((1L<<depth) < terms)
    depth++;
  depth++;
  
  long sieve_size = std::max(10006L, 6*terms);
  sieve = new Sieve[sieve_size/2];
  build_sieve(sieve_size, sieve);

  pstack = new mpint[depth];
  qstack = new mpint[depth];
  gstack = new mpint[depth];
  fpstack = new Fac[depth];
  fgstack = new Fac[depth];

  for (long i = 0; i < depth; i++) {
    fac_init(&fpstack[i]);
    fac_init(&fgstack[i]);
  }
  fac_init(&ftmp);
  fac_init(&fml);

  if (terms > 0)
    binary_split(0, terms, false, 0);
  else {
    pstack[top] = 1;
    qstack[top] = 0;
    gstack[top] = 1;
  }

  delete [] sieve;
  
  gcd = 0;
  fac_clear(&ftmp);
  fac_clear(&fml);

  for (long i = 1; i < depth; i++) {
    pstack[i] = 0;
    qstack[i] = 0;
    fac_clear(&fpstack[i]);
    fac_clear(&fgstack[i]);
  }
  gstack[0] = 0;
  fac_clear(&fpstack[0]);
  fac_clear(&fgstack[0]);

  delete [] gstack;
  delete [] fpstack;
  delete [] fgstack;

  mpfloat::precision = digits/DIGITS_PER_WORD + 3;

  qstack[top] += 13591409*pstack[top];
  pstack[top] *= 53360;
  
  mpfloat pi(pstack[top]);
  mpfloat qi(qstack[top]);

  delete [] pstack;
  delete [] qstack;

  qi = pi/qi;
  pi = sqrt(mpfloat(640320));
  qi *= pi;

  std::cout << std::fixed << std::setprecision(digits) << qi << std::endl;
  
  return 0;
}

void options(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " digits" << std::endl;
    exit(1);
  }
  digits = atol(argv[1]);
  if (digits < 0 || 10000000000 < digits) {
    std::cout << "digits must be in [0..10,000,000,000]" << std::endl;
    exit(1);
  }
}
void build_sieve(long n, Sieve *s) {
  long m, i, j, k;

  m = (long)std::sqrt((double)n);
  memset(s, 0, sizeof(Sieve[n/2]));
  s[0].fac = 1;
  s[0].pow = 1;

  for (i = 3; i <= n; i += 2) {
    if (s[i/2].fac == 0) {
      s[i/2].fac = i;
      s[i/2].pow = 1;
      if (i <= m) {
	for (j = i*i, k = i/2; j <= n; j += 2*i, k++) {
	  if (s[j/2].fac == 0) {
	    s[j/2].fac = i;
	    if (s[k].fac == i) {
	      s[j/2].pow = s[k].pow + 1;
	      s[j/2].nxt = s[k].nxt;
	    }
	    else {
	      s[j/2].pow = 1;
	      s[j/2].nxt = k;
	    }
	  }
	}
      }
    }
  }
}

void binary_split(long a, long b, bool gflag, long level) {
  unsigned long i, mid;
  
  if (b-a == 1) {
    pstack[top] = ((mpint(10939058860032000)*b)*b)*b;
    gstack[top] = mpint(6*b-5)*mpint(2*b-1)*mpint(6*b-1);
    qstack[top] = (545140134*b + 13591409)*gstack[top];
    if (b&1)
      qstack[top] = -qstack[top];
    i = b;
    while ((i&1) == 0)
      i >>= 1;
    fac_set_bp(&fpstack[top], i, 3);
    fac_mul_bp(&fpstack[top], 10005, 3);
    fpstack[top].pow[0]--;
    fac_set_bp(&fgstack[top], 2*b-1, 1);
    fac_mul_bp(&fgstack[top], 6*b-1, 1);
    fac_mul_bp(&fgstack[top], 6*b-5, 1);
  }
  else {
    mid = a + (b-a)*0.641;               // 0.5224 
    binary_split(a, mid, true, level+1);
    top++;
    binary_split(mid, b, gflag, level+1);
    top--;
    if (level >= 1) // orig. 4
      fac_remove_gcd(&pstack[top+1], &fpstack[top+1], &gstack[top], &fgstack[top]);
    pstack[top] *= pstack[top+1];
    qstack[top] *= pstack[top+1];
    qstack[top+1] *= gstack[top];
    qstack[top] += qstack[top+1];
    fac_mul(&fpstack[top], &fpstack[top+1]);
    if (gflag) {
      gstack[top] *= gstack[top+1];
      fac_mul(&fgstack[top], &fgstack[top+1]);
    }
  }
}

void fac_init(Fac *f) {
  fac_init_size(f, INIT_FACS);
}

void fac_init_size(Fac *f, long s) {
  if (s < INIT_FACS)
    s = INIT_FACS;
  f->fac = new long[2*s];
  f->pow = f->fac + s;
  f->max_facs = s;
}

void fac_clear(Fac *f) {
  delete [] f->fac;
}

void fac_resize(Fac *f, long s) {
  if (f->max_facs < s) {
    fac_clear(f);
    fac_init_size(f, s);
  }
}

void fac_compact(Fac *f) {
  long i, j;

  for (i = 0, j = 0; i < f->num_facs; i++) {
    if (f->pow[i] > 0) {
      if (j < i) {
	f->fac[j] = f->fac[i];
	f->pow[j] = f->pow[i];
      }
      j++;
    }
  }
  f->num_facs = j;
}

// f = base^pow

void fac_set_bp(Fac *f, long base, long pow) {
  long i = 0;
  for (base /= 2; base > 0; i++, base = sieve[base].nxt) {
    f->fac[i] = sieve[base].fac;
    f->pow[i] = sieve[base].pow*pow;
  }
  f->num_facs = i;
}

// f *= g

void fac_mul(Fac *f, Fac *g) {
  Fac tmp;
  fac_resize(&fml, f->num_facs + g->num_facs);
  fac_mul2(&fml, f, g);
  tmp = *f;
  *f = fml;
  fml = tmp;
}

// r = f*g

void fac_mul2(Fac *r, Fac *f, Fac *g) {
  long i, j, k;

  for (i = j = k = 0; i < f->num_facs && j < g->num_facs; k++)  {
    if (f->fac[i] == g->fac[j]) {
      r->fac[k] = f->fac[i];
      r->pow[k] = f->pow[i] + g->pow[j];
      i++;
      j++;
    }
    else if (f->fac[i] < g->fac[j]) {
      r->fac[k] = f->fac[i];
      r->pow[k] = f->pow[i];
      i++;
    }
    else {
      r->fac[k] = g->fac[j];
      r->pow[k] = g->pow[j];
      j++;
    }
  }
  for (; i < f->num_facs; i++, k++) {
    r->fac[k] = f->fac[i];
    r->pow[k] = f->pow[i];
  }
  for (; j < g->num_facs; j++, k++) {
    r->fac[k] = g->fac[j];
    r->pow[k] = g->pow[j];
  }
  r->num_facs = k;
}     

// f *= base^pow

void fac_mul_bp(Fac *f, long base, long pow) {
  fac_set_bp(&ftmp, base, pow);
  fac_mul(f, &ftmp);
}

// t = gcd(f, g)
// f /= t
// g /= g

void fac_remove_gcd(mpint *p, Fac *fp, mpint *g, Fac *fg) {
  long i, j, k, c;

  fac_resize(&fml, std::min(fp->num_facs, fg->num_facs));
  for (i = j = k = 0; i < fp->num_facs && j < fg->num_facs; ) {
    if (fp->fac[i] == fg->fac[j]) {
      c = std::min(fp->pow[i], fg->pow[j]);
      fp->pow[i] -= c;
      fg->pow[j] -= c;
      fml.fac[k] = fp->fac[i];
      fml.pow[k] = c;
      i++;
      j++;
      k++;
    }
    else if (fp->fac[i] < fg->fac[j])
      i++;
    else
      j++;
  }
  fml.num_facs = k;
  if (fml.num_facs) {
    bs_mul(gcd, 0, fml.num_facs);
    *p /= gcd;
    *g /= gcd;
    fac_compact(fp);
    fac_compact(fg);
  }
}

void fac_show(Fac *f) {
  for (long i = 0; i < f->num_facs; i++)
    if (f->pow[i] == 1)
      std::cout << f->fac[i] << " ";
    else
      std::cout << f->fac[i] << "^" << f->pow[i] << " ";
  std::cout << std::endl;
}

void bs_mul(mpint &r, long a, long b) {
  long i, j;

  if (b - a <= 32) {
    r = 1;
    for (i = a; i < b; i++) {
      for (j = 0; j < fml.pow[i]; j++)
	r *= fml.fac[i];
    }
  }
  else {
    mpint r1;
    mpint r2;
    bs_mul(r1, a, (a+b)/2);
    bs_mul(r2, (a+b)/2, b);
    r = r1*r2;
  }
}

