#include <iostream>

using namespace std;

void setBit ( uint16_t& value , int pos )
{
    value |= 1 << pos ;

}

void resetBit ( uint16_t& value, int pos )
{
    value &=~(1<<pos);
}


uint16_t CreateBitMask( int start, int length )
{
     uint16_t mask = 0xffff;
     mask >>= start;

     return mask;
}

int main ()
{
    uint16_t value = 234;

    resetBit ( value, 6);
    resetBit ( value, 7 );
    setBit ( value, 8);

    value = value >> 6;

    cout << "the value is" << value << endl;

    bool status =  ( value ^ 4 ) ? false : true;

    cout << " status is " << status << endl;

    return 0;
}





