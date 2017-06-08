
#include "Sha256.h"

static const int buffSize = 32768;	//32kb

string to_hex(byte s) 
{
    stringstream ss;
    	
	ss << hex << setw(2) << setfill('0') << (int) s;
    return ss.str();
}   

int sha256_hash_string(const byte *hash, string &output)
{
   	int retval = -1;

	if (hash) 
    {
		
		output.clear();

		for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) 
        {
			output += to_hex(hash[i]);
		}
		retval = 0;
	}
    
	return retval;
}

/** gets the sha256 hash value encoded in 64 bytes of the input file.
 *  Param(IN)  : path - path of the file whose hash needs to be computed.
 *  Param(OUT) : outputBuffer - the SHA256 has value encoded in 64 bytes.
 *  Return val : -1 => FAILURE, 0 => SUCCESS.
*/

int sha256_file(const char *path, string &outputBuffer)
{
    int retval = -1;
	
	FILE *file = fopen(path, "rb");
    if ( file != NULL )
    {
		//Init SHA 256 context.
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		
		byte *buffer = ( byte* ) calloc (sizeof(byte), buffSize);
		if ( buffer != NULL )
		{
			
			byte hash[SHA256_DIGEST_LENGTH];
			int bytesRead = 0;
			while ( (bytesRead = fread(buffer, 1, buffSize, file) ) )
			{
				SHA256_Update(&sha256, buffer, bytesRead);
			}
			
			SHA256_Final(hash, &sha256);
			retval = sha256_hash_string(hash, outputBuffer);
			
			free(buffer);
			buffer = NULL;
		}
		fclose(file);
	}
	
	return retval;
}
