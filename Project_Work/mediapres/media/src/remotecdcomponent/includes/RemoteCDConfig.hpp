/**********************************************************************************************************
 * 
 * @fileName : RemoteCDConfig.hpp
 * @author   : Monojit Chatterjee
 * @job      : This files contains CRemoteCDConfig which does the configuration proposed by FORD.
 *             Functionality incudes disabling and enabling several functionality
 * 
 * @CopyRighted by Harman Internation pvt Limited 
 * 
 * 
 *********************************************************************************************************
*/


#include <iostream>
#include <RemoteCDCore.hpp>
#include <RemoteCDLog.hpp>
#include <RemoteCDTypes.hpp>

class CRemoteCDConfig
{
   public :
          // constructor
           CRemoteCDConfig ();
           
           // destructor 
           ~CRemoteCDConfig ();
           
           // name implied it tells about presence of Ext CD 
           
           bool CheckIfExtCDPresent () { return mExtCDPresent_;  }
           
           // name implied it tells about warning popUp need to be displayed or not 
           
           bool CheckIfWarningPopupRequired () { return mWarningPopup_ ; };
           
           // When driver operate CD while driving need to process Eject through this function
           
           void ProcessEjectRequest ();
           
           //getEOLData from settings
           eEOLResult getFordEOLData () {   return eEOLResult_ ;       }
           
           //setEOLData from settings
           void      setEOLData ( eEOLResult eEOLResult ) {  eEOLResult_ =  eEOLResult ;  }
           

  private :
    
            // variable implied presence of Ext CD 
            bool mExtCDPresent_;
      
           // variable implied Warning popup status
           bool mWarningPopup_;
      
           // buffer which stores EOL data
         //  unsigned char* buf_byte;
      
           // Setting mExtCDPresent_ if Ext cd present and Warning popup required
           void setExtCDPresentWithWarningPopup ();
      
           // Retriving the EOL data
            bool setEOLData ( unsigned long sysSetID , unsigned long sysIDSize );
      
           
           // Eol Received Status flag
           bool mEOLReceived_; 
           
           // Result of EOL data
           
           unsigned char Result_;
           
           // Get EOL Data which already Received
           
           unsigned char getEOLData () { return Result_ ; }
           
           // Enumeration for REMOTECD EOL requirement
           eEOLResult eEOLResult_ ;
           

           
};

