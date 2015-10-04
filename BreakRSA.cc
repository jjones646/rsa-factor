// Grad student portion of the RSA assignment
// Jonathan Jones
// Fall 2015 ECE6122

#include <iostream>

#include "RSA_Algorithm.h"

using namespace std;

static const size_t chars_per_msg = 6;
static const mpz_class private_key = 4105243553;

// data structure for helping keep track of the params
struct message_t {
  mpz_class p;
  mpz_class q;
  mpz_class phi_p;
  mpz_class phi_q;
  mpz_class phi_n;
  mpz_class msg[10];
};

// our main program
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
    cipher.msg[i] = mpz_class(argv[i + 3]);

  /*
   *  First "break" the keys by factoring n and computing d.
   *
   *  Let the starting point for checking the factors begin
   *  at the first off number of the square root of n
   */
  mpz_class current_factor;
  clog << "--  finding factors for " << RSA.n << endl;
  mpz_sqrt(current_factor.get_mpz_t(), RSA.n.get_mpz_t());

  // Make sure we begin at an odd value
  if ( mpz_fdiv_ui(current_factor.get_mpz_t(), 2) == 0 )
    mpz_add_ui(current_factor.get_mpz_t(), current_factor.get_mpz_t(), 1);

  do {
    // decrement by 2 every time
    mpz_sub_ui(current_factor.get_mpz_t(), current_factor.get_mpz_t(), 2);
  } while ( mpz_divisible_p(RSA.n.get_mpz_t(), current_factor.get_mpz_t()) == 0 );

  // store our found factor
  cipher.p = current_factor;
  // divide n by our found factor to find our other factor
  mpz_fdiv_q(cipher.q.get_mpz_t(), RSA.n.get_mpz_t(), cipher.p.get_mpz_t());

  // double check that we found the right factors for d
  mpz_class check_val;
  mpz_mul(check_val.get_mpz_t(), cipher.p.get_mpz_t(), cipher.q.get_mpz_t());
  // if we actually did, show the value of real vs what our 2 factors compute out to be
  if ( mpz_cmp(RSA.n.get_mpz_t(), check_val.get_mpz_t()) == 0 )
    clog << "--  " << cipher.p << " x " << cipher.q << " = " << check_val << "\t(computed)" << endl;

  // now we compute the private key from our 2 found factors
  // we do this by first finding phi_p & phi_q, then use those to find phi_n
  mpz_sub_ui(cipher.phi_p.get_mpz_t(), cipher.p.get_mpz_t(), 1);
  mpz_sub_ui(cipher.phi_q.get_mpz_t(), cipher.q.get_mpz_t(), 1);
  // find phi_n with the above
  mpz_mul(cipher.phi_n.get_mpz_t(), cipher.phi_p.get_mpz_t(), cipher.phi_q.get_mpz_t());
  // we need to take the multiplicative inverse of d and phi_n to finally get the private key
  mpz_invert(RSA.d.get_mpz_t(), RSA.e.get_mpz_t(), cipher.phi_n.get_mpz_t());

  for (int i = 0; i < argc - 3; ++i) {
    // Decrypt each set of 6 characters
    mpz_class M = RSA.Decrypt(cipher.msg[i]);

    //  use the get_ui() method in mpz_class to get the lower 48 bits of the message
    unsigned long ul = M.get_ui() & 0xffffffffffff;
    mpz_class MM(ul);

    // Now print the 6 ascii values in variable ul.  As stated above the 6 characters
    // are in the low order 48 bits of ui.
    size_t ary_sz = mpz_sizeinbase(MM.get_mpz_t(), 62) + 2;
    char decoded_msg[ary_sz];
    memset(decoded_msg, 0, ary_sz);

    mpz_get_str(decoded_msg, 62, MM.get_mpz_t());

    for ( int j = (chars_per_msg - 1); j >= 0; --j )
      cout << static_cast<char>((ul >> (8 * j)) & 0xff);
  }
  cout << endl;
}

