#pragma once

// RSA Assignment for ECE4122/6122 Fall 2015
// There should be no need to edit this file at all, but change if you feel the need.

#include <gmpxx.h>

class RSA_Algorithm
{
public:
  // Default Constructor
  RSA_Algorithm();

  // Generate key
  void GenerateRandomKeyPair(const size_t sz);

  // Encrypt plaintext message M with key  pair n/e
  // By convention, we will make the encryption key e the public key
  // and the decryption key d the private key.
  const mpz_class Encrypt(const mpz_class& M) const;

  // Decrypt ciphertext message C with key pair n/d
  const mpz_class Decrypt(const mpz_class& C) const;

  const mpz_class GetMsg(const size_t sz);

  // Determine if an encrypted message would be valid.
  // Returns true if message can be encrypted, false otherwise
  bool CheckMsgUseable(const mpz_class& M) const;

  // Debug printing
  // Print n and d
  void PrintND(void) const;

  // print n and e
  void PrintNE(void) const;

  // Print n, d, and e
  void PrintNDE(void) const;

  // Print plaintext message
  void PrintM(const mpz_class& M) const;

  // Print Ciphertext message
  void PrintC(const mpz_class& C) const;

public:
  // Member variables
  mpz_class n;  // p * q
  mpz_class e;  // Public encryption key
  mpz_class d;  // Private decryption key

  // Public Random number generator
public:
  gmp_randclass rng;
};
