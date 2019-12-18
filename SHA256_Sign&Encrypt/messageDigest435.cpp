/***
   prepared for CS435 Project 1 part 2
**/

#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

using std::string;
using std::cout;
using std::endl;

// Prototypes
BigInteger pow16(BigInteger a, BigInteger b);
void hexToDec(BigInteger *hashInt, string hash);
BigInteger modexp(BigInteger hash, BigInteger d, BigInteger n);

 
int main(int argc, char *argv[])
{
   /* //demonstrating how sha256 works
   std::string input = "testing";
   std::string output1 = sha256(input);
   std::cout << "sha256('"<< input << "'):" << output1 << "\n"; */

   //Second part of your project starts here
   if (argc != 3 || (argv[1][0]!='s' && argv[1][0]!='v')) 
      std::cout << "wrong format! should be \"a.exe s filename\"";
   else 
   {
      std::string filename = argv[2];
      
      //read the file
      std::streampos begin,end;
      std::ifstream myfile (filename.c_str(), std::ios::binary);
      begin = myfile.tellg();
      myfile.seekg (0, std::ios::end);
      end = myfile.tellg();
      std::streampos size = end-begin;
      //std::cout << "size of the file: " << size << " bytes.\n"; //size of the file
      
      myfile.seekg (0, std::ios::beg);
      char * memblock = new char[size];
      myfile.read (memblock, size); //read file; it's saved in the char array memblock
      myfile.close();
      
      std::string copyOFfile = filename+".Copy"; 
      std::ofstream myfile2 (copyOFfile.c_str(), std::ios::binary);
      myfile2.write (memblock, size); //write to a file
      myfile2.close();

      //std::cout << memblock << std::endl;

      if (argv[1][0]=='s') 
      {
         BigInteger hashInt = 0;
         hexToDec(&hashInt, sha256(memblock));

         std::ifstream dFile("d_n.txt", std::ios::in);
         string tmpStr; 
         getline(dFile, tmpStr);
         BigInteger d = stringToBigInteger(tmpStr);
         getline(dFile, tmpStr);
         BigInteger n = stringToBigInteger(tmpStr);
         dFile.close();

         BigInteger dHash = modexp(hashInt, d, n);
         std::ofstream output ("file.txt.signature", std::ios::out | std::ios::trunc);
         output << std::hex << dHash;
         output.close();
      }
      else 
      {
         BigInteger hashInt = 0;
         hexToDec(&hashInt, sha256(memblock));

         std::ifstream eFile("e_n.txt", std::ios::in);
         string tmpStr;
         getline(eFile, tmpStr);
         BigInteger e = stringToBigInteger(tmpStr);
         getline(eFile, tmpStr);
         BigInteger n = stringToBigInteger(tmpStr);
         eFile.close();

         std::ifstream hashFile("file.txt.signature", std::ios::in);
         getline(hashFile, tmpStr);
         hashFile.close();
         BigInteger dHash = 0;
         hexToDec(&dHash, tmpStr);

         BigInteger signHash = modexp(dHash, e, n);
         if (signHash == hashInt)
            cout << "Document is authentic." << endl;
         else
            cout << "The document has been modified." << endl;
      }
      delete[] memblock;
    }
    return 0;
}

BigInteger pow16(BigInteger a, BigInteger b)
{
   BigInteger result = a;
   for (BigInteger i = 0; i < b; ++i)
   {
      result *= 16;
   }
   return result;
}

void hexToDec(BigInteger *hashInt, string hash)
{
   for (size_t i = 0; i < hash.length(); ++i)
   {
      if(std::isdigit(hash[i]))
         *hashInt += pow16(hash[i] - '0', hash.length() - i - 1);
      else
      {
         switch (hash[i])
         {
         case 'a':
         case 'A':
            *hashInt += pow16(10, hash.length() - i - 1);
            break;
         case 'b':
         case 'B':
            *hashInt += pow16(11, hash.length() - i - 1);
            break;
         case 'c':
         case 'C':
            *hashInt += pow16(12, hash.length() - i - 1);
            break;
         case 'd':
         case 'D':
            *hashInt += pow16(13, hash.length() - i - 1);
            break;
         case 'e':
         case 'E':
            *hashInt += pow16(14, hash.length() - i - 1);
            break;
         case 'f':
         case 'F':
            *hashInt += pow16(15, hash.length() - i - 1);
            break;
         }
      }
   }
   return;
}

BigInteger modexp(BigInteger hash, BigInteger d, BigInteger n) 
{ 
   BigInteger temp = 1;
   while (d > 0) 
   {
      if (d%2 == 1) 
      {
         temp = (temp * hash)%n;
         --d;
      }
      if(d > 0)
      {
         d = d/2;
         hash = (hash * hash)%n;
      }
   } 
   return temp; 
}