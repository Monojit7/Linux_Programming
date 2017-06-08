#ifndef _CSHA256_H_
#define _CSHA256_H_

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include "openssl/sha.h"

using namespace std;

//TYPEDEFS, ENUMS etc,.

typedef unsigned char byte;

string to_hex(byte s);
int sha256_hash_string(const byte *hash, string &output);

//EXTERN FUNCTIONS

/** gets the sha256 hash value encoded in 32 bytes of the input file.
 *  Param(IN)  : path - path of the file whose hash needs to be computed.
 *  Param(OUT) : outputBuffer - the SHA256 has value encoded in 64 bytes.
 *  Return val : -1 => FAILURE, 0 => SUCCESS.
*/
extern int sha256_file(const char *path, string &outputBuffer);

#endif

