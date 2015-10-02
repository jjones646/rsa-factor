// RSA Assignment for ECE4122/6122 Fall 2015

#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "RSA_Algorithm.h"

using namespace std;

// Implement the RSA_Algorithm methods here

// Constructor
RSA_Algorithm::RSA_Algorithm()
  : rng(gmp_randinit_default)
{
  unsigned long drValue;

  // get a random seed for the random number generator
  int dr = open("/dev/random", O_RDONLY);

  if (dr < 0) {
    cout << "Can't open /dev/random, exiting" << endl;
    exit(0);
  }

  read(dr, (char*)&drValue, sizeof(drValue));
  //cout << "drValue " << drValue << endl;
  rng.seed(drValue);
// No need to init n, d, or e.
}

void RSA_Algorithm::GenerateRandomKeyPair(size_t sz)
{
  mpz_class p, q, phi_n, gcd;

  // Select 2 random prime numbers that are of size sz bits
  do {
    p = rng.get_z_bits(sz);
  } while (0 == mpz_probab_prime_p(p.get_mpz_t(), 100));

  do {
    q = rng.get_z_bits(sz);
  } while (0 == mpz_probab_prime_p(q.get_mpz_t(), 100));

  n = p * q;
  phi_n = (p - 1) * (q - 1);

  do {
    do {
      d = rng.get_z_bits(2 * sz);
      // repeat until we have a value that is less than phi_n
    } while (phi_n <= d);
    //
    mpz_gcd(gcd.get_mpz_t(), d.get_mpz_t(), phi_n.get_mpz_t());

    // repeat until the GCD of d and phi_n is 1
  } while (1 != gcd.get_ui());

  // take the multiplicative inverse d % phi_n
  mpz_invert(e.get_mpz_t(), d.get_mpz_t(), phi_n.get_mpz_t());
}

mpz_class RSA_Algorithm::Encrypt(const mpz_class& M)
{
  mpz_class C;

  mpz_powm(C.get_mpz_t(), M.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());

  return C;
}

mpz_class RSA_Algorithm::Decrypt(const mpz_class& C)
{
  mpz_class M;

  mpz_powm(M.get_mpz_t(), C.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());

  return M;
}

void RSA_Algorithm::PrintND()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << endl;
}

void RSA_Algorithm::PrintNE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " e " << e << endl;
}

void RSA_Algorithm::PrintNDE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << " e " << e << endl;
}

void RSA_Algorithm::PrintM(const mpz_class& M)
{ // Do not change this, right format for the grading script
  cout << "M " << M << endl;
}

void RSA_Algorithm::PrintC(const mpz_class& C)
{ // Do not change this, right format for the grading script
  cout << "C " << C << endl;
}




