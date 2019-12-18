#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include <ctime>

#include "BigIntegerLibrary.hh"

// Using statments for simplicity
using std::endl;
using std::string;
using std::fstream;

// Prototypes
BigInteger generate();
BigInteger gcd(BigInteger, BigInteger);
BigInteger fermat(BigInteger, BigInteger);
BigInteger probPrime(BigInteger);
void genE(BigInteger *, BigInteger);
void EEA(BigInteger, BigInteger, BigInteger *, BigInteger *);
void write(BigInteger a, BigInteger b, string file);

int main(int argc, char** argv)
{
   srand(time(0));

	try 
   {
      BigInteger p = probPrime(generate()), 
                 q = probPrime(generate()),
                 n = p * q,
                 phi_n = (p-1) * (q - 1);

      // Output probable primes to the file p_q.txt 
      write(p, q, "p_q.txt");

      // Block to find the inverse of 2 < e < phi_n
      BigInteger e = p + 2, b, d;
      genE(&e, phi_n);
      EEA(e, phi_n, &d, &b);
      d = (d%phi_n + phi_n);
      
      write(e, n, "e_n.txt");
      write(d, n, "d_n.txt");

	} catch(char const* err) {
		std::cerr << "The library threw an exception:\n"
			       << err << std::endl;
	}

	return 0;
}

/* 
   NOTE: LARGE NUMBERS SHOULD NEVER BE SEEDED BY A NON-CRYPTOGRAPHICALLY SECURE PRNG OR TRNG, AS IS DONE HERE
   Desc: This function takes in a file, deletes its contents, and then generates values for p and q (if called again) 
         to use in the RSA algorithm. The numbers, once confirmed likely prime, will be entered into the file for good.
    Pre: Function call.
   Post: The function will return a potential prime. 
 */
BigInteger generate()
{  
   // Make the large integers and store them in a file. 
   fstream p_q;
   p_q.open ("p_q.txt", std::ios::in | std::ios::out | std::ios::trunc);
   if(!p_q.is_open())
      throw "Output file failed to open in generate().";

   // Generate a random number between 512 and 1023.
   uint digitCount = rand()%154 + 154; // Generate length for prime
   // Generate prime with digits.
   p_q << (rand ()%9 + 1);
   for (size_t i = 0; i < digitCount-2; ++i)
   {
      p_q << (rand()%10);
   }
   int last = (rand()%5) * 2 + 1;
   while (last == 5)
   {
      last = (rand()%5) * 2 + 1;
   };
   p_q << last << endl;

   string temp;
   p_q.seekg(0);
   getline(p_q, temp);
   return stringToBigInteger(temp);
}

/* 
   Desc: A function to recursively find the gcd of two numbers p and a. 
    Pre: Takes in two BigIntegers, does not require that they be large enough to require the 
         use of the BigInteger container (a will likely be small, but can be large). 
   Post: Returns the gcd of the two numbers. 
 */
BigInteger gcd(BigInteger p, BigInteger a)
{
   if(p < a)
      return gcd(a, p);
   else if(p%a == 0)
      return a;
   else
      return gcd(a, p%a);
}

/* 
   Desc: A function to quickly check primality. Also called "fast exponentiation" or "square and multiply"
    Pre: Takes in an integer to test, a, and the prime p.
   Post: Returns a^(p-1)(mod p)
 */
BigInteger fermat(BigInteger p, BigInteger a) 
{ 
   BigInteger temp = 1, power = p - 1;
   while (power > 0) 
   {
      if (power%2 == 1) 
      {
         temp = (temp * a)%p;
         --power;
      }
      if(power > 0)
      {
         power = power/2;
         a = (a * a)%p;
      }
   } 
   return temp; 
}

/* 
   Desc: Takes in the first generated prime to test. If it isnt prime, it generates a new one
         within the function. The number returned passed a few tests and is probably prime. 
    Pre: Requires the call of the function with either a generated number or a number to test.
   Post: Returns a number that is probably prime by the tests.
 */
BigInteger probPrime(BigInteger p)
{
   // Check primality of p
   // The more primes you test p against, the more likely it is prime. If it passes the gcd test for
   // each one and subsequently the fermat test, there is a good chance it is prime.
   // I can supply the proof on why this works. Simple algebraic manipulation.
   BigInteger TEST1000 = stringToBigInteger("19590340644999083431262508198206381046123972390589368223882605328968666316379870661851951648789482321596229559115436019149189529725215266728292282990852649023362731392404017939142010958261393634959471483757196721672243410067118516227661133135192488848989914892157188308679896875137439519338903968094905549750386407106033836586660683539201011635917900039904495065203299749542985993134669814805318474080581207891125910"); 
   // Multiple of all primes under 1000
   
   while (true)
   {
      if(gcd(p, TEST1000) != 1){}
      else if(fermat(p, TEST1000) != 1){}
      else{return p;}
   
      p = generate();
   }
}

/* 
   Desc: Small function to find an e relatively prime to  phi_n. No return is necessary.
    Pre: Requires that a ring of size phi_n be sent though (just as an integer) and a 
         variable to hold the result (e) be sent by reference.
   Post: Will find a value in the range 2 < e < phi_n.
 */
void genE(BigInteger *e, BigInteger phi_n)
{
   // Find an e s.t. gcd(e, phi_n) = 1;
   while (true)
   {
      if(gcd(*e, phi_n) == 1)
         return;
      *e+=2;
   }
}

/* 
   Desc: Extended Euclidean Algorithm
    Pre: Put in a number to find the linear combination of and the modulus to find it under.
         d will be the value of the inverse once the function is finished and b is meant 
         to be a placeholder in main (or just before the call) and the final
         result (e's inverse) will be in d. 
   Post: Returns the inverse of input number (e) in the multiplicative ring Z_phi_n
 */
void EEA(BigInteger e, BigInteger phi_n, BigInteger *d, BigInteger *b)
{
   if (e == 0) 
   { 
       *d = 0;
       *b = 1; 
       return;
   } 
   BigInteger d_, b_;
   EEA(phi_n%e, e, &d_, &b_); 

   *d = b_ - (phi_n/e) * d_; 
   *b = d_; 
}

/* 
   Desc: Quick function to write to a specified file
    Pre: Used to write the two BigIntegers to a file by specification. Requires that two BigIntegers and a string (the file name including extension)      be passed to the function.
   Post: No return. The file will be created if necessary, if it had data in it, it will have been replaced. The file now contains the two numbers.
 */
void write(BigInteger a, BigInteger b, string file)
{
   fstream generic(file, std::ios::out | std::ios::trunc);
   if(!generic.is_open())
      throw "Output file failed to open in write.";
   generic << a << endl << b;
   generic.close();
}