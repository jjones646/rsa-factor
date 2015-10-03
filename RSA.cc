// ECE4122/6122 RSA Encryption/Decryption assignment
// Jonathan Jones
// Fall Semester 2015

#include <iostream>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "RSA_Algorithm.h"

#define LOG_MSG \
      if (debug_disabled) {} \
      else clog

using namespace std;

static const size_t min_bits = 32;
static const size_t max_bits = 1024;
static const size_t keypairs_per_sz = 10;
static const size_t num_msgs = 10;

int main(void)
{
  bool debug_disabled = true;

  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;

  // Loop from sz = 32 to 1024 inclusive
  // track the program's total error as it runs
  size_t total_err = 0;
  for (size_t sz = min_bits; sz <= max_bits; sz = 2 * sz) {
    LOG_MSG << "--  computing " << keypairs_per_sz << " keypairs with " << sz << " bits" << endl;
    gmp_randclass rng(gmp_randinit_default);

    // for each size choose 10 different key pairs
    for (size_t i = 0; i < keypairs_per_sz; i++) {
      RSA.GenerateRandomKeyPair(sz);
      // show what our keypair values are
      RSA.PrintNDE();

      // keep track of our accumulated errors
      size_t accum_err = 0;
      for ( size_t j = 0; j < num_msgs; j++ ) {
        mpz_class Msg, MsgCmpt, Ciph;

        do {
          Msg = RSA.GetMsg(2 * sz);
          // keep generating a message until we find one
          // that will work
        } while ( RSA.CheckMsgUseable(Msg) == false );

        // show our message
        RSA.PrintM(Msg);

        // For eacm message encrypt it using the public key (n,e).
        Ciph = RSA.Encrypt(Msg);
        // show what the ciphertext message looks like
        RSA.PrintC(Ciph);
        // After encryption, decrypt the ciphertext using the private
        // key (n,d) and verify it matches the original message.
        MsgCmpt = RSA.Decrypt(Ciph);
        // sum up our errors
        accum_err += abs(mpz_cmp(Msg.get_mpz_t(), MsgCmpt.get_mpz_t()));
      }

      total_err += accum_err;

      LOG_MSG << "\ttrial " << i + 1
              << " results: " << (accum_err == 0 ? "PASS" :  "FAIL") << " ("
              << num_msgs << " messages)" << endl;
    }
  }

  LOG_MSG << endl
          << "\t======================" << endl
          << "\tTest Result:\t" << (total_err == 0 ? "PASSED" : "FAILED") << endl
          << "\t======================" << endl;

}
