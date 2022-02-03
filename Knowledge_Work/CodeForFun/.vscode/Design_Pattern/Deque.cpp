#include <iostream>

using namespace std;

#define MAX 5

struct s_Dequue 
{
  int front ;
  
  int rear ;
  
  int size ;
  
  int items [MAX] ;
  
};


void initDeque ( s_Dequue* sDq)
{
    sDq->front = -1 ;
    sDq->rear  = -1 ;
    sDq->size  = MAX;
}

bool isFull ( s_Dequue* sDq )
{
    return (( sDq->front == 0 && sDq->rear == ( sDq->size - 1 ) ) || ( sDq->front == sDq->rear + 1 ));
}

bool isEmpty ( s_Dequue* sDq )
{
    return ( sDq->front == -1 );
}


void insertAtFront ( s_Dequue* sDq, int element )
{
    
    if ( isFull (sDq ) )
    {
        cout << "Queue is Full hence can not add new element " << element << endl;
    }
    
    else
    {
        if ( sDq->front == -1 )
        {
            sDq->front = 0;
            sDq->rear = 0;
        }
        else if ( sDq->front == 0 )
        {
            sDq->front = sDq->size - 1 ;
        }
        else
        {
            sDq->front = sDq->front - 1 ; 
        }
        
        cout << " element " << element << " added at position -> " << sDq->front << endl;
        sDq->items [ sDq->front ] = element;
    }
    
}

void insertAtRear ( s_Dequue* sDq, int element  )
{
    if ( isFull ( sDq ))
    {
        cout << " Queue is Full hence can not add new element " << element;
    }
    else
    {
        if ( sDq->front == -1 )
        {
            sDq->front = 0;
            sDq->rear = 0;
        }
        else if ( sDq->rear == sDq->size - 1 )
        {
            sDq->rear = 0;
        }
        else
        {
            sDq->rear = sDq->rear + 1 ;
        }
        
        cout << " element " << element << " added at position " << sDq->rear << endl;
        sDq->items [ sDq->rear ] = element;
    }
}

void deleteFront ( s_Dequue* sDq )
{
    if ( isEmpty (sDq ))
    {
        cout << "Queue is empty " << endl;
    }
    else
    {
        cout << " Deleted element is " << sDq->items [sDq->front ] << " at position "  << sDq->front << endl;
        if ( sDq->front == sDq->rear )
        {
            sDq->front = -1;
            sDq->rear = -1;
        }
        else if ( sDq->front == sDq->size - 1 )
        {
            sDq->front = 0;
        }
        else
        {
            sDq->front = sDq->front + 1 ;
        }
        
    }
}

void deleteRear ( s_Dequue* sDq )
{
    if ( isEmpty ( sDq ))
    {
        cout << " Queue is empty " << endl;
    }
    else
    {
        cout << "Deleted element is " << sDq->items [ sDq->rear ] << " at position " << sDq->rear << endl;
        if ( sDq->front == sDq->rear )
        {
            sDq->front = -1 ;
            sDq->rear = -1;
        }
        else if ( sDq->rear == 0 )
        {
            sDq->rear = sDq->size -1 ;
        }
        else
        {
            sDq->rear = sDq->rear - 1 ;
        }
    }
}

int getFront ( s_Dequue* sDq)
{
    int value = -1;
    if ( isEmpty ( sDq ))
   cout << " Queue is empty" << endl;
    else
    {
        cout << " front is " << sDq->front << " item is " << sDq->items [ sDq->front ] << endl;
        value =  sDq->items [ sDq->front ];
    }
    
    return value;
}

int getRear ( s_Dequue* sDq )
{
    int value = -1 ;
    if ( isEmpty (sDq ))
    {
        cout << "Queue is empty " << endl;
    }
    else
    {
        cout << " rear is " << sDq->rear << " item is " << sDq->items [ sDq->rear ] << endl;
        value = sDq->items [ sDq->rear ];
    }
    
    return value;
}

int main ()
{
    s_Dequue* sDq = (s_Dequue*) malloc ( sizeof ( s_Dequue ));
    initDeque ( sDq );
    insertAtFront ( sDq, 10 );
    insertAtFront ( sDq, 20 );
    insertAtRear ( sDq, 30 );
    insertAtRear ( sDq, 40 );
    
    cout << " front is " <<  getFront ( sDq ) << endl ;
    
     cout << " rear is " <<  getRear ( sDq ) << endl ;
     
     deleteRear ( sDq );
     
         cout << " front is " <<  getFront ( sDq ) << endl ;
    
     cout << " rear is " <<  getRear ( sDq ) << endl ;
     
          deleteRear ( sDq );
          insertAtFront ( sDq, 80 );
     
         cout << " front is " <<  getFront ( sDq ) << endl ;
    
     cout << " rear is " <<  getRear ( sDq ) << endl ;
    
    
}