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
      if (debug_disabled == true) {} \
      else clog

using namespace std;

static const size_t min_bits = 32;
static const size_t max_bits = 1024;
static const size_t keypairs_per_sz = 10;
static const size_t num_msgs = 10;
static const std::string msg_dict = "abcdefghijklmnopqrstuvwxyz";
// Max number of words in a sentence
static const size_t sen_max = 10;

std::string getRandStr(size_t length)
{
  const size_t num_words = rand() % length;
  std::string msg;
  // track how long the current sentence is
  size_t sen_len = 0;
  bool sentence_begin = true;

  msg.push_back('\t');

  do {
    // determine randomly how long this word will be
    size_t w_len = rand() % 10 + 2;
    // append random bytes together to form a 'word'
    for (size_t j = 0; j < w_len; j++)
      msg.push_back(msg_dict.at(rand() % msg_dict.length()));

    if (sentence_begin)
      msg.at(msg.length() - w_len) = std::toupper(msg.at(msg.length() - w_len));

    // add a space after every word, sometimes also ending
    // the current sentence
    if ( sen_len > sen_max ) {
      msg.push_back('.');
      sen_len = 0;
      sentence_begin = true;
    } else {
      sen_len += 1;
      sentence_begin = false;
    }
    msg.push_back(' ');
    // Keep adding random char words until we reach
    // at least our desired length.
  } while ( msg.length() <= length );

  // Make our first character an upper case one
  msg.at(0) = std::toupper(msg.at(0));
  // Replace the last character with a period
  msg.at(msg.length() - 1) = '.';

  return msg;
}

int main(void)
{
  bool debug_disabled = true;
  // srand(time(0));
  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;

  // for (size_t i = 0; i < 10; i++) {
  //   std::string msgg =  getRandStr(3000);
  //   cout << msgg << endl << endl;
  // }

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
      // RSA.PrintNDE();

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
        // RSA.PrintM(Msg);

        // For eacm message encrypt it using the public key (n,e).
        Ciph = RSA.Encrypt(Msg);
        // show what the ciphertext message looks like
        // RSA.PrintC(Ciph);
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
