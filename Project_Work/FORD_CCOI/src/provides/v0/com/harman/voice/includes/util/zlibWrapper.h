#ifndef ZLIBWRAPPER_HPP
#define ZLIBWRAPPER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include <cassert>
#include "zlib.h"
#include "zconf.h"
#include "CCOILog.hpp"

using namespace std;  


// EXTERN FUNCTIONS  

/**
 * decompresses the src file using libz library functions
 * @param[IN]  - srcFile: source file which is to be decompressed.
 * @param[OUT] - destFile: destination file path to which the decompressed output bytes need to be written into.
 * returns: 0 on SUCCESS.
            -1 on failure.
*/
extern int zlibUnzip(const char *srcFile, const char *destFile); 

#endif 

