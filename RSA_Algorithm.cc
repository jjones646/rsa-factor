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
  rng.seed(drValue);
  // No need to init n, d, or e.
}

void RSA_Algorithm::GenerateRandomKeyPair(const size_t sz)
{
  mpz_class p, q, phi_p, phi_q, phi_n, gcd;
  size_t num_bits;

  // Select 2 random prime numbers that are of size sz bits
  do {
    do {
      do {
        // find the 1st random number that's probably prime
        do {
          p = rng.get_z_bits(sz);
        } while (0 == mpz_probab_prime_p(p.get_mpz_t(), 100));
        // find the 2nd random number that's probably prime
        do {
          q = rng.get_z_bits(sz);
        } while (0 == mpz_probab_prime_p(q.get_mpz_t(), 100));
        // Make sure our 2 selected numbers aren't the same.
        // Will rarely ever happen...but, just to be ultra safe.
      } while (0 == mpz_cmp(p.get_mpz_t(), q.get_mpz_t()));

      num_bits = mpz_sizeinbase(p.get_mpz_t(), 2)
                 + mpz_sizeinbase(q.get_mpz_t(), 2);
      // repeat until we have the correct bit length
    } while ((2 * sz) != num_bits);

    // find n and phi_n
    mpz_mul(n.get_mpz_t(), p.get_mpz_t(), q.get_mpz_t());
    mpz_sub_ui(phi_p.get_mpz_t(), p.get_mpz_t(), 1);
    mpz_sub_ui(phi_q.get_mpz_t(), q.get_mpz_t(), 1);
    mpz_mul(phi_n.get_mpz_t(), phi_p.get_mpz_t(), phi_q.get_mpz_t());
    // repeat until the bit lengths match what we are trying to generate
  } while ((2 * sz) != mpz_sizeinbase(n.get_mpz_t(), 2));

  // Now, find d
  do {
    do {
      do {
        d = rng.get_z_bits(2 * sz);
        // repeat until the number of bits in d matches our desired lenght
      } while ((2 * sz) != mpz_sizeinbase(d.get_mpz_t(), 2));
      // repeat until we have a value that is less than phi_n
    } while (0 <= mpz_cmp(phi_n.get_mpz_t(), d.get_mpz_t()));
    // determine the GCD of d and phi_n
    mpz_gcd(gcd.get_mpz_t(), d.get_mpz_t(), phi_n.get_mpz_t());
    // repeat until the GCD of d and phi_n is 1
  } while (1 != gcd.get_ui());

  // Take the multiplicative inverse d and phi_n, save it as e
  mpz_invert(e.get_mpz_t(), d.get_mpz_t(), phi_n.get_mpz_t());
}

const mpz_class RSA_Algorithm::Encrypt(const mpz_class& M) const
{
  mpz_class C;
  mpz_powm(C.get_mpz_t(), M.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  return C;
}

const mpz_class RSA_Algorithm::Decrypt(const mpz_class& C) const
{
  mpz_class M;
  mpz_powm(M.get_mpz_t(), C.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
  return M;
}

const mpz_class RSA_Algorithm::GetMsg(const size_t sz)
{
  mpz_class Msg;
  Msg = rng.get_z_bits(sz);
  return Msg;
}

bool RSA_Algorithm::CheckMsgUseable(const mpz_class& M) const
{
  return (mpz_cmp(M.get_mpz_t(), n.get_mpz_t()) < 0);
}

void RSA_Algorithm::PrintND(void) const
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << endl;
}

void RSA_Algorithm::PrintNE(void) const
{ // Do not change this, right format for the grading script
  cout << "n " << n << " e " << e << endl;
}

void RSA_Algorithm::PrintNDE(void) const
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << " e " << e << endl;
}

void RSA_Algorithm::PrintM(const mpz_class& M) const
{ // Do not change this, right format for the grading script
  cout << "M " << M << endl;
}

void RSA_Algorithm::PrintC(const mpz_class& C) const
{ // Do not change this, right format for the grading script
  cout << "C " << C << endl;
}
