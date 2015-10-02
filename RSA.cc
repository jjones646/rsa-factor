// ECE4122/6122 RSA Encryption/Decryption assignment
// Jonathan Jones
// Fall Semester 2015

#include <iostream>
#include <string>

#include "RSA_Algorithm.h"

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
  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;

  // Used for generating the random string messages
  srand(time(0));

  // Loop from sz = 32 to 1024 inclusive
  for (size_t sz = min_bits; sz <= max_bits; sz = 2 * sz) {

    //clog << "===== SIZE " << sz << " ======" << endl;

    // for each size choose 10 different key pairs
    for (size_t i = 0; i < keypairs_per_sz; i++) {
      RSA.GenerateRandomKeyPair(sz);
      //RSA.PrintNDE();

      // For each key pair choose 10 differnt plaintext
      // messages making sure it is smaller than n.
      for (size_t j = 0; j < num_msgs; j++) {
        mpz_class M, D;
        // call up a random string that takes up at least the number
        // of bits that are used for 'n' in our algorithm
        std::string msg = getRandStr((2 * sz) / sizeof(' '));

        RSA.PrintNDE();

        // set the message into an mpz_t variable
        mpz_import(M.get_mpz_t(), msg.length(), 1, sizeof(' '), 0, 0, msg.c_str());

        // For eacm message encrypt it using the public key (n,e).
        RSA.Encrypt(M);
        // After encryption, decrypt the ciphertext using the private
        RSA.Decrypt(D);
        // key (n,d) and verify it matches the original message.
      }
    }
  }

  // Show that we're done
  clog << "done" << endl;
}
