#pragma once
#include"ReqTracibility.h"
#include "Types.h"
using namespace IOA;

#include "IDiscreteDevice.h"
#include "IAnalogDevice.h"
#include "IA825Device.h"
#include "IA429Device.h"

#ifndef _WINDOWS_
#include <Windows.h>
#endif

#ifndef createDev
typedef IDevice* ( WINAPI * createDev )( void );
#endif



class TestProOprt;
class DeviceProcedure
{
protected:
	ReqTracibility* mReqTracer;
	UInt32 mDeviceCount;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // cleanDefinitions
    //   memeset all the handles definition
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    void
    //	
	virtual void cleanDefinitions( void )=0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // checkResult
    //   check the operation result
    //
    //   Parameters:
    //    alResult - The operation returns value
    //
    //   Return Values:
    //    void
    //	
	virtual bool checkResult( UInt32 alResult )=0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // startIO
    //   open the device
    //
    //   Parameters:
    //    void 
    //
    //   Return Values:
    //     bool 
    //	
	virtual bool startIO( void )=0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // closeIO
    //   close the device
    //
    //   Parameters:
    //    void 
    //
    //   Return Values:
    //     bool 
    //	
	virtual bool closeIO( void )=0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // defineMultiDevices
    //   define multi-Devices 
    //
    //   Parameters:
    //    aNumber - number of devices to define
    //    aConfig - configurations 
    //
    //   Return Values:
    //     bool 
    //	
	virtual bool defineMultiDevices( UInt32 aNumber, char** aConfig )=0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // recordStart
    //   recording Start
    //
    //   Parameters:
    //    aDeviceID - device id to define
    //    aName - configuration for the device
    //
    //   Return Values:
    //     bool 
    //	
	virtual bool recordStart( UInt32 aDeviceID, const char* aName )=0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // recordEnd
    //   recording End
    //
    //   Parameters:
    //    aDeviceID - device id to define
    //    aName - configuration for the device
    //
    //   Return Values:
    //     bool 
    //	
	virtual bool recordEnd( UInt32 aDeviceID )=0;
public:	
	DeviceProcedure( ReqTracibility* aReqTracer )
	{
        mDeviceCount=0;
		if( aReqTracer==NULL )
		{
			printf( "ReqTracibility Handle is NULL! Keybord input and exit! " );
			char lCh;
            lCh=_getch( );
            exit( 1 );		
		}
		mReqTracer=aReqTracer;
	};
	friend class TestProOprt;
	~DeviceProcedure( void ){};
};

class TestProOprt
{
protected:
	DeviceProcedure* mDeviceProcedure;
public:
	ReqTracibility* mReqTracer;
	TestProOprt( DeviceProcedure* aDeviceProcedure )
	{
	    mReqTracer=aDeviceProcedure->mReqTracer;
		mDeviceProcedure=aDeviceProcedure;
	};
///////////////////////////////////////////////////////////////////////////////////////////////
    // StepZeroInitTestData
    //   initialize the data for the test cases
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    void
    //	
  virtual void StepZeroInitTestData( void )=0;
  ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepOneDefine1Device
    //   define one device 
    //
    //   Parameters:
    //    aDeviceName - device configuration
    //
    //   Return Values:
    //    void
    //
  virtual bool StepOneDefine1Device( char* aDeviceName )
  {
    bool lResult=false;
	mReqTracer->printComments( "Define One device with deviceID 0" );
	char* lDeviceName[]={aDeviceName};
	lResult= mDeviceProcedure->defineMultiDevices( 1,lDeviceName ) ;
    return( lResult );
  };

   ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepTwoDefineInputChannels
    //   define InputChannels 
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    void
    //
  virtual bool StepTwoDefineInputChannels( void ){ return( false );};

   ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepThreeDefineOutputChannels
    //   define OnputChannels 
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    bool
    //
  virtual bool StepThreeDefineOutputChannels( void ){ return( false );};
     ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepFourOpenDevice
    //   Open device  
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    bool
    //
  virtual bool StepFourOpenDevice( void )
  {
  	bool lResult=false;
	mReqTracer->printComments( "Open device( after the definition )" );
	lResult=mDeviceProcedure->startIO( );
    return( lResult ); 
   };

  ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepFiveWriteOutputChannels
    //   StepFive Write OutputChannels  
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    bool
    //
  virtual bool StepFiveWriteOutputChannels( void ){ return( false );};
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepSixReadInputChannels
    //   StepSix Read InputChannels 
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    bool
    //
  virtual bool StepSixReadInputChannels( void ){ return( false );};
      ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepSevenDisableChannel
    //   StepSeven Disable Channel  
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    bool
    //
  virtual bool StepSevenDisableChannel( void ){return( false );};
        ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepEightEnableChannel
    //   StepEight Enable Channel  
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    bool
    //
  virtual bool StepEightEnableChannel( void ){return( false );};

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepLastMultiDevicesOperation
    //   StepLast MultiDevices Tests  
    //
    //   Parameters:
    //    aNumber - number of devices (usually use 2 devices)
    //    aDeviceName - Devices configurations
    //
    //   Return Values:
    //    bool
    //
  virtual bool StepLastMultiDevicesOperation( UInt32 aNumber ,char** aDeviceName )=0;
  
  ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepEndCleanRCVbuffer
    //   StepEnd Clean Received buffer
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    void
    //
  virtual void StepEndCleanRCVbuffer( void )=0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepSevenCloseDevice
    //   StepSeven Close the Device
    //
    //   Parameters:
    //    void
    //
    //   Return Values:
    //    bool
    //
  virtual bool StepSevenCloseDevice( void )
  {
  	mReqTracer->printComments( "Close the device" );
	bool lResult=false;
    lResult=mDeviceProcedure->closeIO( );
    mReqTracer->printComments( "Memset the RCV buffer" );
    StepEndCleanRCVbuffer( );
    return( lResult );
  };

   ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepDynamicRecordData
    //   Step of beginning Recording Data 
    //
    //   Parameters:
    //    aName - File name of the records
    //    aDeviceID -The device to record of data TX/RX.
    //
    //   Return Values:
    //   bool
    //
  virtual bool StepDynamicRecordData( const char* aName,UInt32 aDeviceID )
  {
    bool lResult=false;
	mReqTracer->printComments( "Set up the data recording for the device with the name " );
	mReqTracer->printComments( aName );
	lResult= mDeviceProcedure->recordStart( aDeviceID,aName ) ;
    return( lResult );
  };
  
   ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepDynamicRecordDataStop
    //   Step of Stop Recording Data 
    //
    //   Parameters:
    //    aDeviceID -The device recorded of data TX/RX.
    //
    //   Return Values:
    //   bool
    //
  virtual bool StepDynamicRecordDataStop( UInt32 aDeviceID )
  {
    bool lResult=false;
	mReqTracer->printComments( "Stop the data recording for the device" );
	lResult= mDeviceProcedure->recordEnd( aDeviceID ) ;
    return( lResult );
  };

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepOpenCloseErrorTestOne
    //   Step of Error test : open and close operation 
    //
    //   Parameters:
    //   void
    //
    //   Return Values:
    //   bool
    //
  virtual bool StepOpenCloseErrorTestOne( void )
  {
    bool lResult=false;
	mReqTracer->printComments( "Try to Open a device not correctly defined with wrong Name/Numbers" );
	char* lDeviceName[]={"ERROR_DEVICE"};
	lResult= mDeviceProcedure->defineMultiDevices( 1,lDeviceName ) ;
	mDeviceProcedure->startIO( );
	mReqTracer->printComments( "Try to Close a device with wrong name" );
	mDeviceProcedure->closeIO( );
	return( lResult );
  };
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepTRXErrorTestTwo
    //   Step of Error test : TX and RX data error
    //
    //   Parameters:
    //   void
    //
    //   Return Values:
    //   bool
    //
  virtual bool StepTRXErrorTestTwo( void )=0;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // StepRecordErrorTestThree
    //   Step of Error test : recording data error
    //
    //   Parameters:
    //   void
    //
    //   Return Values:
    //   bool
    //
  virtual bool StepRecordErrorTestThree( void )
 {    
    bool lResult=false;
    mReqTracer->printComments( "Try to Set up and began record When device not opened" );
	StepSevenCloseDevice( );
	StepDynamicRecordData( "Error.csv",0 );
    return( lResult );
 };

};
