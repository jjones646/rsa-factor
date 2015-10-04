// ECE4122/6122 RSA Encryption/Decryption assignment
// Jonathan Jones
// Fall Semester 2015

#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "RSA_Algorithm.h"

#define LOG_MSG \
      if (debug_disabled == true) {} \
      else clog

#define MSG_FILE \
      if (debug_disabled == true) {} \
      else msg_file

using namespace std;

static const size_t min_bits = 32;
static const size_t max_bits = 1024;
static const size_t keypairs_per_sz = 10;
static const size_t num_msgs = 10;

// used if debug_disabled is set to false
static const std::string test_string = "Hello! This message will undergo RSA encryption & be broken.";
std::ofstream msg_file;
// this will pack 6 chars into a mpz_class type object for encryption
mpz_class getTestMsg(const size_t msg_id)
{
  unsigned long ul = 0;
  size_t starting_index = msg_id * 6;
  for ( size_t i = 0; i < 6; ++i) {
    // shift up each byte as we iterate through joining every char
    ul <<= 8;
    unsigned long byte = static_cast<unsigned long>(test_string.at(starting_index + i));
    ul |= byte;
  }
  return mpz_class(ul);
}

// our main program
int main(void)
{
  // set to false to show debugging information
  bool debug_disabled = true;

  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;

  // Loop until we reach the max number of bits for our keypair generation
  // track the program's total error as it runs
  size_t total_err = 0;
  // if debug is enabled, only do the 32 bit keypairs
  const size_t bits_max = debug_disabled ? max_bits : 32;
  for (size_t sz = min_bits; sz <= bits_max; sz = 2 * sz) {
    LOG_MSG << "--  computing " << keypairs_per_sz << " keypairs with " << sz << " bits" << endl;
    gmp_randclass rng(gmp_randinit_default);

    // write out a bash script that we can run directly if debugging is enabled
    MSG_FILE.open("generated-msgs.sh");
    MSG_FILE << "#!/bin/bash" << endl << endl;

    // for each size choose 10 different key pairs
    for (size_t i = 0; i < keypairs_per_sz; i++) {
      // generate a keypair!
      RSA.GenerateRandomKeyPair(sz);
      // show what our keypair values are
      if ( debug_disabled == true )
        RSA.PrintNDE();

      // write to the bash script the first 2 arguments that we pass
      // into the BreakRSA program
      MSG_FILE << "# ===== Keypair " << i + 1 << " =====" << endl;
      MSG_FILE << "./BreakRSA " << RSA.n << ' ' << RSA.e;

      // keep track of our accumulated errors
      size_t accum_err = 0;
      for ( size_t j = 0; j < num_msgs; j++ ) {
        mpz_class Msg, MsgCmpt, Ciph;

        if ( debug_disabled == true ) {
          do {
            Msg = RSA.GetMsg(2 * sz);
            // keep generating a message until we find one
            // that will work
          } while ( RSA.CheckMsgUseable(Msg) == false );
        }
        else {
          // generate our testbench message for passing into our cracking program later
          Msg = getTestMsg(j);
        }

        // show our message
        if ( debug_disabled == true )
          RSA.PrintM(Msg);

        // For eacm message encrypt it using the public key (n,e).
        Ciph = RSA.Encrypt(Msg);

        if ( debug_disabled == true ) {
          // show what the ciphertext message looks like
          RSA.PrintC(Ciph);
        } else {
          // write the ciphertext message as the next argument in the bash script
          MSG_FILE << ' ' << Ciph;
        }

        // After encryption, decrypt the ciphertext using the private
        // key (n,d) and verify it matches the original message.
        MsgCmpt = RSA.Decrypt(Ciph);

        // sum up our errors
        accum_err += abs(mpz_cmp(Msg.get_mpz_t(), MsgCmpt.get_mpz_t()));

        // == nothing below this line affects the program's keypair generation and/or encryption/decryption ==
      }
      // accumulate the total program's error
      total_err += accum_err;
      // write the error to the bash script & setup for our next line
      MSG_FILE << endl << "# Total errors in keypair " << i + 1 << " generation detected:\t" << total_err << endl << endl;

      LOG_MSG << "\ttrial " << i + 1
              << " results: " << (accum_err == 0 ? "PASS" :  "FAIL") << " ("
              << num_msgs << " messages)" << endl;
    }
    // close our bash script if debugging is eanbled
    MSG_FILE.close();
  }

  LOG_MSG << endl
          << "\t======================" << endl
          << "\tTest Result:\t" << (total_err == 0 ? "PASSED" : "FAILED") << endl
          << "\t======================" << endl;

}
