#pragma once
#include <iostream>
#include <fstream>
#include <conio.h>
#include <time.h> 
#include <iomanip>
#ifndef _WINDOWS_
#include <Windows.h>
#endif


class ReqTracibility
{
private :
    std::ofstream*  mFileOut; 
    HANDLE mHConsole;
    char mValidate;
    char mTestName[500]; 
    int mCurrentOption;
    char **mTestProcedures;
    unsigned int mTestProNumb;
   
   // DeviceProcedure
 ///////////////////////////////////////////////////////////////////////////////////////////////
    // sigReplace
    //   Local Time change to correct file name type
    //
    //   Parameters:
    //     aStr - The return(to be changed) string pointer
    //     aBegan - The begin position 
    //     aEnd - The end position 
    //     aOld - The old char(to be changed) 
    //     aNew - The new char(to replace the old one) 
    //
    //   Return Values:
    //    void
    //	  
	void sigReplace( char* aStr ,unsigned int aBegan, unsigned int aEnd,char aOld,char aNew );
///////////////////////////////////////////////////////////////////////////////////////////////
    // createFileName
    //   To create a new file name contains the TestProcedure name and local time
    //
    //   Parameters:
    //     aFileName - The return(to be changed) string pointer
    //
    //   Return Values:
    //    void
    //	  
	void createFileName( char* aFileName );
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // setConsoleColor
    //   To change the console text color 
    //
    //   Parameters:
    //     wAttributes - The acttribute of the text color setting
    //
    //   Return Values:
    //     BOOL - True when setting operation succeed 
    //	 
    BOOL setConsoleColor( WORD wAttributes );
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // kbCheck
    //   Wait for keyboard input for next step opertion or validation 
    //
    //   Parameters:
    //   void
    //
    //   Return Values:
    //   void
    //	 
    char kbCheck( );
   
public:

  
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // printErrorExit
    //   print the Error generated  
    //
    //   Parameters:
    //   aComments - Error string
    //
    //   Return Values:
    //    void
    //	
    void printErrorExit( const char* aComments );
     ///////////////////////////////////////////////////////////////////////////////////////////////
    // getFileName
    //   get the File name for csv file naming
    //
    //   Parameters:
    //   void 
    //
    //   Return Values:
    //   char*
    //	
    char* getFileName( void )
    {
        return(mTestName);
    };
       ///////////////////////////////////////////////////////////////////////////////////////////////
    // ReqTracibility
    //   Constructor
    //
    //   Parameters:
    //   aTitle - Test Procedure Name 
    //   aTPOptions - Test Procedure Functions' names 
    //   aTPNumb - Number of Test Procedure Functions 
    //
    //
    //   Return Values:
    //   void
    //	
	ReqTracibility( const char* aTitle, char** aTPOptions,unsigned int aTPNumb );
      ///////////////////////////////////////////////////////////////////////////////////////////////
    // ~ReqTracibility
    //   Distructor
    //
    //   Parameters:
    //   void
    //
    //
    //   Return Values:
    //   void
    //	
    ~ReqTracibility( void );

  ///////////////////////////////////////////////////////////////////////////////////////////////
    // printOptions
    //   print the test progress Options  
    //
    //   Parameters:
    //    
    //
    //   Return Values:
    //    void
    //	
    unsigned int printOptions( );

///////////////////////////////////////////////////////////////////////////////////////////////
    // printComments
    //   print the Comments or Operation Steps 
    //
    //   Parameters:
    //     aOpertion - Operations contend or results
    //
    //   Return Values:
    //    void
    //	
	void printComments( const char* aOpertion );
///////////////////////////////////////////////////////////////////////////////////////////////
    // printComments
    //   print the Comments or Operation Steps 
    //
    //   Parameters:
    //     aOpertion - Operations contend or results
    //     aValue - Operations additional parameter
    //
    //   Return Values:
    //    void
    //	
    void printComments( const char* aOpertion, __int32 aValue );

  ///////////////////////////////////////////////////////////////////////////////////////////////
    // printHeadline
    //   print the headline of formal test procedures
    //
    //   Parameters:
    //     aOpertion - Operations as the headline
    //
    //   Return Values:
    //    void
    //	
    void printHeadline( const char* aOpertion );
  ///////////////////////////////////////////////////////////////////////////////////////////////
    // printHeadline
    //   print the headline of formal test procedures
    //
    //   Parameters:
    //     aHead - Operations as the headline
    //     aOpertion - Operations as the headline
    //     aIndex - The index of the test procedure in one test procedure group.
    //
    //   Return Values:
    //    void
    //	
	void printHeadline( const char* aHead,const char* aOpertion,__int32 aIndex );
///////////////////////////////////////////////////////////////////////////////////////////////
    // printHeadline
    //   print the headline of formal test procedures
    //
    //   Parameters:
    //     aHead - Operations as the headline
    //     aOpertion - Operations as the headline
    //     aValue - Additional parameter
    //     aIndex - The index of the test procedure in one test procedure group.
    //
    //   Return Values:
    //    void
    //	
	void printHeadline( const char* aHead,const char* aOpertion, __int32 aValue, __int32 aIndex );

  ///////////////////////////////////////////////////////////////////////////////////////////////
    // verify
    //   wait for tester to verify the test result
    //
    //   Parameters:
    //     aReqID - The requirement ID in char* type
    //     aComments - Additional comments
    //     aPartriallyCoverred - Works when the test procedure(test case) just partially covers the requirement.
    //
    //   Return Values:
    //    void
    //	
	void verify( const char* aReqID, const char*  aComments ,unsigned int aPartriallyCoverred );

 ///////////////////////////////////////////////////////////////////////////////////////////////
    // verify
    //   wait for tester to verify the test result
    //
    //   Parameters:
    //     aReqID - The requirement ID in char* type
    //     aComments - Additional comments
    //     int aParam - Additioanl Parameters need to list
    //     aPartriallyCoverred - Works when the test procedure(test case) just partially covers the requirement.
    //
    //   Return Values:
    //    void
    //
	void verify( const char* aReqID, const char*  aComments ,int aParam,unsigned int aPartriallyCoverred );
	

    
///////////////////////////////////////////////////////////////////////////////////////////////
    // printforset
    //   wait for tester to check the environment and move to next steps
    //
    //   Parameters:
    //     aConstuction - The constructions for tester to check
    //     aData - Additional parameter 
    //
    //   Return Values:
    //    void
    //	
    template <class PARAM>
    void printforset( const char* aConstuction,PARAM aData )
    {
        setConsoleColor( FOREGROUND_RED|FOREGROUND_INTENSITY|BACKGROUND_BLUE );
       
        std::cout<<aConstuction<<"Param {"<<std::dec<<aData<<"}"<<std::endl<<std::flush;
        *mFileOut<<aConstuction<<"Param {"<<std::dec<<aData<<"}"<<std::endl<<std::flush;
       
        mValidate=kbCheck( );
        if( mValidate =='N' || mValidate =='n' )
        {
            std::cout<<"Warning! Not set the equipment or software for test!"<<std::endl<<std::flush;
            *mFileOut<<"Warning! Not set the equipment or software for test!"<<std::endl<<std::flush;      
        }
        else
        {
            std::cout<<"Yes, All Ready and Continue!"<<std::endl<<std::flush;
            *mFileOut<<"Yes, All Ready and Continue!"<<std::endl<<std::flush;    
        }
        setConsoleColor( BACKGROUND_INTENSITY );

    };
///////////////////////////////////////////////////////////////////////////////////////////////
    // printforset
    //   wait for tester to check the environment and move to next steps
    //
    //   Parameters:
    //     aConstuction - The constructions for tester to check
    //
    //   Return Values:
    //    void
    //	
    void printforset( const char* aConstuction)
    {
        setConsoleColor( FOREGROUND_RED|FOREGROUND_INTENSITY|BACKGROUND_BLUE );
        std::cout<<aConstuction<<std::endl<<std::flush;
        *mFileOut<<aConstuction<<std::endl<<std::flush;
        mValidate=kbCheck( );
        if( mValidate =='N' || mValidate =='n' )
        {
            std::cout<<"Warning! Not set the equipment or software for test!"<<std::endl<<std::flush;
            *mFileOut<<"Warning! Not set the equipment or software for test!"<<std::endl<<std::flush;      
        }
        else
        {
            std::cout<<"Yes, All Ready and Continue!"<<std::endl<<std::flush;
            *mFileOut<<"Yes, All Ready and Continue!"<<std::endl<<std::flush;    
        }
        setConsoleColor( BACKGROUND_INTENSITY );
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // printBuffer
    //   print the data(buffers) RX or TX 
    //
    //   Parameters:
    //     aSuccess - For checking wether the TX/RX operation is successed
    //     aBuffer  - Data Buffer  
    //     aHandleId- Handle(Channel or others) ID
    //     aBufferLength - Real Buffer( data to send or received data ) size ,not the maximum length of the buffer
    //     aDataOutDirection - Trur means data out, false means data in.
    //
    //   Return Values:
    //    void
    //	
    template <class BUFFERTYPE>
    void printBuffer( bool aSuccess,BUFFERTYPE* aBuffer,int aHandleId,
	unsigned int aBufferLength,bool aDataOutDirection )
    {
         setConsoleColor( FOREGROUND_RED |FOREGROUND_INTENSITY );
        int lBufferLength= aBufferLength;
        int lHandleId= aHandleId;
        if( aSuccess==false )
        {
            std::cout<<"Error in TX/RX data"<<std::endl<<std::flush;
            *mFileOut<<"Error in TX/RX data"<<std::endl<<std::flush;
        }
        else
        {
            if( aDataOutDirection==true )
            {
                std::cout<< "TX "<<lBufferLength <<" number of Data on Channel { "<< lHandleId << " }"<<std::endl<<std::flush;
                *mFileOut<< "TX "<<lBufferLength <<" number of Data on Channel { "<< lHandleId << " }"<<std::endl<<std::flush;
            }
            else
            {
                std::cout<< "RX "<<lBufferLength <<" number of Data on Channel { "<< lHandleId << " }"<<std::endl<<std::flush;
                *mFileOut<< "RX "<<lBufferLength <<" number of Data on Channel { "<< lHandleId << " }"<<std::endl<<std::flush;
            }
            if( lBufferLength >0 )
            {
                for( int lDataIndex=0;lDataIndex<lBufferLength-1; lDataIndex++ )
                {
                    std::cout<<std::hex<<(int)aBuffer[lDataIndex]<<' '<<std::flush;
                    *mFileOut<<std::hex<<(int)aBuffer[lDataIndex]<<' '<<std::flush;
                };
                std::cout<<std::hex<<(int)aBuffer[lBufferLength-1]<<std::endl<<std::flush;
                *mFileOut<<std::hex<<(int)aBuffer[lBufferLength-1]<<std::endl<<std::flush;

            };
        }
        setConsoleColor( BACKGROUND_INTENSITY );
    
    };
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // printSingleData
    //   print the data RX or TX 
    //
    //   Parameters:
    //     aSuccess - For checking wether the TX/RX operation is successed
    //     aData  - Data   
    //     aHandleId- Handle(Channel or others) ID
    //     aDataOutDirection - Trur means data out, false means data in.
    //
    //   Return Values:
    //    void
    //	
    template <class DATATYPE>
    void printSingleData( bool aSuccess,DATATYPE aData,int aHandleId,
	bool aDataOutDirection )
    {
        setConsoleColor( FOREGROUND_RED |FOREGROUND_INTENSITY );
        int lHandleId= aHandleId;
        if( aSuccess == false )
        {
            std::cout<<"Error in TX/RX data"<<std::endl<<std::flush;
            *mFileOut<<"Error in TX/RX data"<<std::endl<<std::flush;
        }
        else
        {
            if( aDataOutDirection==true )
            {
                std::cout<<aData<< "(One data) send out on the channel "<< lHandleId <<std::endl<<std::flush;
                *mFileOut<<aData<< "(One data) send out on the channel "<< lHandleId <<std::endl<<std::flush;
            }
            else
            {
                std::cout<<aData<< "(One data)Received on the channel "<< lHandleId <<std::endl<<std::flush;
                *mFileOut<<aData<< "(One data)Received on the channel "<< lHandleId <<std::endl<<std::flush;
            }
        }
        setConsoleColor( BACKGROUND_INTENSITY );    
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // printInitData
    //   print the initialized data  
    //
    //   Parameters:
    //     aBuffer - Buffer pointer 
    //     aBufferLength  - Buffer size   
    //     aComments- Additional comments
    //     
    //   Return Values:
    //    void
    //
    template <class INITDATATYPE>
	void printInitData( INITDATATYPE* aBuffer,
	unsigned int aBufferLength,const char* aComments )
    {
       std::cout<<aComments<<std::endl<<std::flush;
       *mFileOut<<aComments<<std::endl<<std::flush;
     if( aBufferLength>0 )
     {
      for( int lDataIndex=0;lDataIndex<( int )(aBufferLength-1); lDataIndex++ )
      {
          std::cout<<std::hex<<(int)aBuffer[lDataIndex]<<"   "<<std::flush;
         *mFileOut<<std::hex<<(int)aBuffer[lDataIndex]<<"   "<<std::flush;
      };
      std::cout<<std::hex<<(int)aBuffer[aBufferLength-1]<<std::endl<<std::flush;
      *mFileOut<<std::hex<<(int)aBuffer[aBufferLength-1]<<std::endl<<std::flush;
     }
      std::cout<<"Totoal length : "<<std::dec<<(int)aBufferLength<<std::endl<<std::flush;
      *mFileOut<<"Totoal length : "<<std::dec<<(int)aBufferLength<<std::endl<<std::flush;
    };

};

