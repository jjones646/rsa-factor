# RSA

An example program showing how one can implement the RSA algorithm in C++ using the [GNU multiple precision arithmetic library](https://gmplib.org/manual/).

## Usage

Compile the RSA encryption program using the `make` command.
```
# Compile program for encrypting messages
make RSA

# This will create a `crack-msgs.sh` script in the working directory
./RSA
```

Compile the BreakRSA program to crack the encryption. This is feasible since the RSA program only encrypts the original message with a small sized private key.
```
# Compile the cracking program
make BreakRSA

# Set the execute bit on the generated script
chmod +x crack-msgs.sh

# Crack the messages we encrypted previsouly
./crack-msgs.sh
```
