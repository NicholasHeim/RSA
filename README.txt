Part 1: 
   The program automatically generates multiple numbers:
   p, q: two, most likely, prime numbers. Generated randomly and tested against the product of the 
         first 1000 primes using Fermat's Little Theorem. This gives it a very high chance of being a prime number. 
   e, d: e is the secret key, used when encrypting to confirm a document is from you. The reciever can decrypt 
         the message sent with the shared key. d is the shared key. 
   phi_n: This is the ring in which e and d are found such that gcd(e, phi_n) = gcd(d, phi_n) = 1 and that e*d(mod phi_n) = 1;
   n:    p*q: it is very hard to factor. 
   Essentially, this is to find the numbers required to run the RSA algorithm. 
Part 2: 
   When launching the program it takes two addition arguments:
      "./messageDigest s file.txt"
      "./messageDigest v file.txt"
   From how it is programmed, these are the two necessary inputs. If it is encrypted and decrypted with this program, 
   the filename will be consistant and not need to be entered manually. Thus, the fourth argument (which I believe that was 
   mentioned as optional in class) was avoided for simplicity. All files must be within the same directory unless you wish
   to edit the Makefile. The program will output whether or not the file was edited. 