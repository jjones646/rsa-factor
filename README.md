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

Compile the BreakRSA program to crack the encryption. This is feasible with small keypair sizes.
```
# Compile the cracking program
make BreakRSA

# Set the execute bit on the generated script
chmod +x crack-msgs.sh

# Crack the messages we encrypted previsouly
./crack-msgs.sh
```

If all goes well, you'll get something like the following. 64 bit keypairs were used here.
```
--  finding factors of 11571852336778284913
--  3301296013 x 3505245301 = 11571852336778284913	(computed)
Hello! This message will undergo RSA encryption & be broken.
--  finding factors of 13371352893245412089
--  3429980573 x 3898375693 = 13371352893245412089	(computed)
Hello! This message will undergo RSA encryption & be broken.
--  finding factors of 9310241486817166823
--  2793285769 x 3333078767 = 9310241486817166823	(computed)
Hello! This message will undergo RSA encryption & be broken.
--  finding factors of 14680102887916106827
--  3602396977 x 4075093051 = 14680102887916106827	(computed)
Hello! This message will undergo RSA encryption & be broken.
--  finding factors of 15267422673364647001
--  3861680891 x 3953569211 = 15267422673364647001	(computed)
Hello! This message will undergo RSA encryption & be broken.
--  finding factors of 10136357367441415379
--  2649952769 x 3825108691 = 10136357367441415379	(computed)
Hello! This message will undergo RSA encryption & be broken.
--  finding factors of 10823681707513822799
--  2649574777 x 4085063687 = 10823681707513822799	(computed)
Hello! This message will undergo RSA encryption & be broken.
--  finding factors of 12737896809771421061
--  3564719629 x 3573323609 = 12737896809771421061	(computed)
Hello! This message will undergo RSA encryption & be broken.
--  finding factors of 9316695256451889661
--  2298008311 x 4054247851 = 9316695256451889661	(computed)
Hello! This message will undergo RSA encryption & be broken.
--  finding factors of 10276350735744555283
--  3066685201 x 3350963683 = 10276350735744555283	(computed)
Hello! This message will undergo RSA encryption & be broken.
```
