// ECE4122/6122 RSA Encryption/Decryption assignment
// Fall Semester 2015

#include <iostream>
#include "RSA_Algorithm.h"

using namespace std;

static const size_t min_bits = 32;
static const size_t max_bits = 1024;
static const size_t keypairs_per_sz = 10;

int main()
{
  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;

  // Loop from sz = 32 to 1024 inclusive
  for (size_t sz = min_bits; sz <= max_bits; sz = 2 * sz) {

    clog << "===== SIZE " << sz << " ======" << endl;

    // for each size choose 10 different key pairs
    for (size_t i = 0; i < keypairs_per_sz; i++) {
      RSA.GenerateRandomKeyPair(sz);
      //RSA.PrintNDE();
    }

    clog << "--  generated 10 keypairs at " << sz << " bits" << endl;

    // For each key pair choose 10 differnt plaintext
    // messages making sure it is smaller than n.
    for (size_t i = 0; i < keypairs_per_sz; i++) {
      // If not smaller then n then choose another

      // For eacm message encrypt it using the public key (n,e).

      // After encryption, decrypt the ciphertext using the private

      // key (n,d) and verify it matches the original message.
    }
  }

  // Show that we're done
  clog << "done" << endl;
}






