// Grad student portion of the RSA assignment
// Jonathan Jones
// Fall 2015 ECE6122

#include <iostream>

#include "RSA_Algorithm.h"

using namespace std;

const mpz_class factorial(const mpz_class& num)
{
  mpz_class n = num, f = 1;
  while (n > 1)
    f *= n--;

  return f;
}

struct message_t {
  mpz_class p;
  mpz_class q;
  mpz_class msg[10];
};

int main(int argc, char** argv)
{
  // Arguments are as follows:
  // argv[1] = n;
  // argv[2] = e;  // n and e are the public key
  // argv[3] = first 6 characters of encrypted message
  // argv[4] = next 6 characters .. up to argv[12] are the lsat 6 characters
  //
  // The number of arguments will always be exacty 12, and each argument past the
  // public key contain 6 ascii characters of the encrypted message.
  // Each of the 32bit values in the argv[] array are right justified in the
  // low order 48 bits of each unsigned long.  The upper 16 bits are always
  // zero, which insures the each unsigned long is < n (64 bits) and therefore
  // the RSA encryption will work.

  // Our one and only RSA_Algorithm object
  RSA_Algorithm RSA;
  // data structure for more easily working with the message and its factors
  message_t cipher;

  // Set the keys in the rsa object
  // 1st argument is n
  RSA.n = mpz_class(argv[1]);
  // 2nd argument is e
  RSA.e = mpz_class(argv[2]);
  // The next 10 arguments contain the encrypted message
  for (size_t i = 0; i < argc - 3; ++i)
    cipher.msg[i] = atol(argv[i + 3]);

  //   The broken (computed) private key for the above is 4105243553

  // First "break" the keys by factoring n and computing d
  // RSA

  // Let the starting point for checking the factors begin
  // at the first off number of the square root of n
  mpz_class current_factor;
  cout << "--  finding factors for " << RSA.n << endl;
  mpz_sqrt(current_factor.get_mpz_t(), RSA.n.get_mpz_t());

  // Make sure we begin at an odd value
  if ( mpz_fdiv_ui(current_factor.get_mpz_t(), 2) == 0 ) {
    cout << "--  fixing starting value to be an odd number (" << current_factor << " => ";
    mpz_add_ui(current_factor.get_mpz_t(), current_factor.get_mpz_t(), 1);
    cout << current_factor << ")" << endl;
  }

  do {
    // decrement by 2 every time
    mpz_sub_ui(current_factor.get_mpz_t(), current_factor.get_mpz_t(), 2);
  } while ( mpz_divisible_p(RSA.n.get_mpz_t(), current_factor.get_mpz_t()) == 0 );

  // store our found factor
  cipher.p = current_factor;
  cout << "--  found factor of " << cipher.p << "\t(for " << RSA.n << ")" << endl;

  // divide n by our found factor to find our other factor
  mpz_fdiv_q(cipher.q.get_mpz_t(), RSA.n.get_mpz_t(), cipher.p.get_mpz_t());
  // mpz_fdiv_r(rem.get_mpz_t(), RSA.n.get_mpz_t(), cipher.p.get_mpz_t());

  // cout << "  Quotient:\t" << quo << endl;
  // cout << "  Remainder:\t" << rem << endl;
  // cipher.q = quo;


  // double check that we found the right factors for d
  mpz_class check_val;
  mpz_mul(check_val.get_mpz_t(), cipher.p.get_mpz_t(), cipher.q.get_mpz_t());
  // if we actually did, show the value of real vs what our 2 factors compute out to be
  if ( mpz_cmp(RSA.n.get_mpz_t(), check_val.get_mpz_t()) == 0 ) {
    cout << cipher.p << " x " << cipher.q << " = " << RSA.n << "\t(actual)" << endl;
    cout << cipher.p << " x " << cipher.q << " = " << check_val << "\t(computed)" << endl;
  }

  // // Set rsa.d to the calculated private key d
  // // rsa.d = mpz_class(// broken d value here)

  // for (int i = 3; i < 13; ++i)
  // { // Decrypt each set of 6 characters
  //   mpz_class c(argv[i]);
  //   mpz_class m = rsa.Decrypt(c);
  //   //  use the get_ui() method in mpz_class to get the lower 48 bits of the m
  //   unsigned long ul = m.get_ui();
  //   // Now print the 6 ascii values in variable ul.  As stated above the 6 characters
  //   // are in the low order 48 bits of ui.
  // }
  // cout << endl;

  cout << "done" << endl;
}

