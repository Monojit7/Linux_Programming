/* mblen example */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* mblen, mbtowc, wchar_t(C) */
#include <cwchar>
#include <iostream>
#include <fcntl.h>
#include <experimental/text_view>
using namespace std;

void printbuffer ( const char* pt, size_t max)
{
  int length;
  wchar_t dest;

  mblen (NULL, 0);         /* reset mblen */
  mbtowc (NULL, NULL, 0);  /* reset mbtowc */

  while (max>0) {
    length = mblen ( ( const char* )pt, max);
    cout << "[ length ] :" << length << endl;
    if (length<1) break;
    mbtowc(&dest, ( const char* )pt,length);
//   wcout << &dest << endl ;
    printf ("[%lc]",dest);
    pt+=length; max-=length;
  }
}

int main()
{

  setlocale ( LC_ALL , ""  );
   const char str [] = { 0x0,0x61,0x0,0x6a,0x0,0x69,0x0,0x72,0x0,0x61,0x0,0x6f,0x0,0x0,0x4d,0x0,0x61,0x0,0x73,0x0,0x74,0x0,0x61,0x0,0x6e,0x0,0x69,0x0,0x20,0x0,0x7c,0x0,0x20,0x0,0x53,0x0, 0x0 };

auto tv = make_text_view<char16_character_encoding>(str);

for ( auto i : tv )
    cout << i ;

//  wstring s = str ;
 // wcout << "[ size ] : " << sizeof ( str )  << str << endl;
i//  printbuffer (str,sizeof(str));

  return 0;
}
