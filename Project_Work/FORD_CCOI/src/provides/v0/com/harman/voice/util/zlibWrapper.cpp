
#include "zlibWrapper.h"

//namespace declaratives
using namespace std ;
using namespace v0::com::harman::voice ;

//STATIC FUNCTIONS

// internal function to call the incremental inflate of zlib during the decompression process.
static bool gzipInflate( const string& compressedBytes, string& uncompressedBytes ) 
{  
	if ( compressedBytes.size() == 0 ) 
    {  
    		uncompressedBytes = compressedBytes ;  
    		return true ;  
  	}  
  	
	//clear old contents if any
  	uncompressedBytes.clear() ;  
  
  	unsigned fullLength = compressedBytes.size() ;  
  	unsigned incrementLength = compressedBytes.size() / 2;  
  	unsigned uncompLength = fullLength ;
	
  	char* uncomp = (char*) calloc( sizeof(char), uncompLength );  
	if (uncomp != NULL)
	{
		z_stream strm;  
		strm.next_in = (Bytef *) compressedBytes.c_str();  
		strm.avail_in = compressedBytes.size() ;  
		strm.total_out = 0;  
		strm.zalloc = Z_NULL;  
		strm.zfree = Z_NULL;  
	  
		if (inflateInit2(&strm, (16+MAX_WBITS)) != Z_OK) 
		{  
			free( uncomp );
			uncomp = NULL;
			return false;  
		}
		
		bool done = false;
		while (!done) 
		{  
			// If our output buffer is too small  
			if (strm.total_out >= uncompLength ) 
			{  
				// Increase size of output buffer  
				char* uncomp2 = (char*) calloc( sizeof(char), uncompLength + incrementLength);  
				if (uncomp2 != NULL)
				{
					memcpy( uncomp2, uncomp, uncompLength );  
					uncompLength += incrementLength;
					free( uncomp );
					uncomp = uncomp2;
				}
				else
				{
					free( uncomp );
					uncomp = NULL;
					CcoiLog.write ( CCOILog::eLogzlibWrapper, CCOILog::LOG_ERROR, "Not able to allocate memory through calloc for zlib data");
					return false;
				}
			}		  
			strm.next_out = (Bytef *) (uncomp + strm.total_out);  
			strm.avail_out = uncompLength - strm.total_out;  
	  
			// Inflate another chunk.  
			int err = inflate (&strm, Z_SYNC_FLUSH);  
			if (err == Z_STREAM_END)
			{
				done = true;  
			}
			else if (err != Z_OK)  
			{  
				break;  
			}	  
		}  
	  
		if (inflateEnd (&strm) != Z_OK) 
		{  
			free( uncomp );  
			uncomp = NULL;
			return false;  
		}  
	  
		for (size_t i = 0; i < strm.total_out; ++i ) 
		{  
			uncompressedBytes += uncomp[ i ];  
		}
		free( uncomp );
		uncomp = NULL;
	}
	else
	{
		CcoiLog.write ( CCOILog::eLogzlibWrapper, CCOILog::LOG_ERROR, "Not able to allocate memory through calloc for zlib data");
		return false;
	}
	
	return true ; 
}

// Returns the file size in no. of bytes in the file  
static ifstream::streampos zfileSize(const string& filename) 
{

	// Opens the file in binary mode & position at the end.	
	ifstream file(filename.c_str(), ios::ate | ios::binary);
	
	return (file)? (int)file.tellg() : 0;
}

// Reads the zip file into memory.
static int zloadFile( const string& filename, string& contents ) 
{  
  
	int retval = -1;
	int fSize = zfileSize(filename);
  
  	if (fSize) 
    {
		// clear the old contents if any..
		contents.clear();
		// reserve the output buffer to hold the file size contents
		contents.reserve(fSize);
	  
  		// Open the gzip file in binary mode  
		FILE* f = fopen( filename.c_str(), "rb" );  
  		if ( f != NULL ) 
        { 
			// Read all the bytes in the file into out param contents 
			int c = fgetc( f );  
			while ( c != EOF ) 
            {  
			    contents +=  (char) c ;  
			    c = fgetc( f ); 
			}  
			fclose (f);  
			retval = 0;
		}
	}
	
	return retval;
}

// EXTERN FUNCTIONS

/**
 * decompresses the src file using libz library functions
 * @param[IN]  - srcFile: source file which is to be decompressed.
 * @param[OUT] - destFile: destination file path to which the decompressed output bytes need to be written into.
 * returns: 0 on SUCCESS.
            -1 on failure.
*/
int zlibUnzip(const char *srcFile, const char *destFile) 
{  
	if(!srcFile || !destFile) 
    {
		CcoiLog.write ( CCOILog::eLogzlibWrapper, CCOILog::LOG_ERROR, " Invalid input, check the file path names pointers\n" ) ;
		return -1;
	}

  	// Read the gzip file data into memory  
  	string fileData;
  	if ( zloadFile( srcFile, fileData ) == -1) 
    {
        CcoiLog.write ( CCOILog::eLogzlibWrapper, CCOILog::LOG_ERROR, " Error loading input file\n" ) ; 
    	return -1;
  	}  
  
  	// Uncompress the file data.
  	string data;
  	if ( !gzipInflate( fileData, data ) ) 
    {  
    		CcoiLog.write ( CCOILog::eLogzlibWrapper, CCOILog::LOG_ERROR, " Error decompressing file\n" ) ;
    		return -1;
  	}  
 	
	//write the inflated data into the dest file. 
	FILE *fp = fopen(destFile, "wb");
  	if (fp) 
    {
  		fwrite(&data[0], sizeof(char), data.size (), fp);
        fclose(fp);

        CcoiLog.write ( CCOILog::eLogzlibWrapper, CCOILog::LOG_INFO, "%s:%d compressed bytes %d uncompressed bytes %d", __FUNCTION__, __LINE__, fileData. size (), data. size () ) ;
	}
	else 
    {
		CcoiLog.write ( CCOILog::eLogzlibWrapper, CCOILog::LOG_ERROR, " CCOI:unable to open destination file for write - check the file path\n" ) ; 
		return -1;
	}
  
  	return 0;
}

