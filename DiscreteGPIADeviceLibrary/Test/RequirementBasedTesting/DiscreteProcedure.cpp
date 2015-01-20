#include "StdAfx.h"
#include "DiscreteProcedure.h"


DiscreteProcedure::DiscreteProcedure( IDiscreteDevice* aDeviceLibDll,ReqTracibility* aReqTracer )
	:DeviceProcedure( aReqTracer )
{
	mDeviceLibDll=aDeviceLibDll;
}
DiscreteProcedure::~DiscreteProcedure( )
{
	
}
/************DeviceProcedure Interface***** only the device handle is different**********/
bool DiscreteProcedure::checkResult( UInt32 aResult )
{
	bool result=true;
    static char sErrInfo[1000];
    if( aResult != IDiscreteDevice::ResultSuccess )
    {
        mReqTracer->printComments( "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  Operation Error!" );
        mDeviceLibDll->getLastError( sErrInfo, 1000u );
		mReqTracer->printComments( sErrInfo );
		mReqTracer->printErrorExit( sErrInfo );
		result=false;
    }
	else
	{
		mReqTracer->printComments( "################################### this operation succeeded!" );
	};
	return( result );
}
bool DiscreteProcedure::defineMultiDevices( UInt32 aNumber, char** aConfig )
{ 
	bool mResult = true;
	mReqTracer->printComments( "--------Start configuring devices--------" );
	for( UInt32 mDeviceID = 0; mDeviceID < aNumber ; mDeviceID++ )
	{
		mResult=checkResult ( mDeviceLibDll->defineDevice
				( mDeviceID,aConfig[mDeviceID], &mDevice[mDeviceID] ) );
		if( mResult==true )
		{
			mDeviceCount++;
		}
		else
		{
            return( false );
		};
	}
	return( mResult );
};
bool DiscreteProcedure::startIO( void )
{
    bool mResult = true;
	mResult = checkResult( mDeviceLibDll->open( ) );
    mReqTracer->printComments( "It shall activate all configured channels" );
	return mResult;
}
bool DiscreteProcedure::closeIO( void )
{
    bool mResult = true;
	mResult = checkResult( mDeviceLibDll->close( ) );
	mReqTracer->printComments( "It shall stop sending/receiving data" );
	cleanDefinitions( );
    return mResult;
}
bool DiscreteProcedure::recordStart( UInt32 aDeviceNumber, const char* aName )
{
	bool mResult = false;
	mReqTracer->printComments( "It shall Start Recording" );
	mResult = checkResult ( mDeviceLibDll->startTraceLog( mDevice[aDeviceNumber],aName ) );
	return( mResult );
};
bool DiscreteProcedure::recordEnd( UInt32 aDeviceNumber )
{
	bool mResult = false;
	mReqTracer->printComments( "It shall Stop Recording" );
	mResult = checkResult ( mDeviceLibDll->stopTraceLog( mDevice[aDeviceNumber] ) );
	return( mResult );
};
/************End DeviceProcedure Interface***************/
//The PortNUmber? 8bit bank number? there will be 4 banks? and the maximum PortNUmber shall be 4?
//Device has how many ports will be get at the dll loading ( handware send the message to it and initilize the mPortnumber value )
/***********GPIA DISCRETE UNIQUE OPERATION*************/
void DiscreteProcedure::cleanDefinitions( void )
{
	mDeviceCount=0;
	mReqTracer->printComments( ".............Memset( 0 ) all the Handles..............." );
	memset( mInBitID,0,GPIADISCRETE_DEVICETEST_NUMBER*BIT_TEST_NUMBER*sizeof( IDiscreteDevice::BitID ) );
	memset( mOutBitID,0,GPIADISCRETE_DEVICETEST_NUMBER*BIT_TEST_NUMBER*sizeof( IDiscreteDevice::BitID ) );
	memset( mDevice,0,GPIADISCRETE_DEVICETEST_NUMBER*sizeof( IDiscreteDevice::DeviceID ) );
};
bool DiscreteProcedure::defineInBitPort( UInt32 aDeviceID,
	UInt32 aPortID,UInt32 aPosition, UInt32 aInBitID )
{
	bool result=false;
	mReqTracer->printComments( "..............define a bit port IN-Channel............." );
	result = checkResult( mDeviceLibDll->defineBit( mDevice[aDeviceID], 
		aPortID,aPosition,IDiscreteDevice::Receive,&mInBitID[aDeviceID][aInBitID] ) );
	return( result );
}; 
bool DiscreteProcedure::defineOutBitPort( UInt32 aDeviceID,
	UInt32 aPortID,UInt32 aPosition, UInt32 aInBitID )
{
	bool result=false;
	mReqTracer->printComments( "..............define a bit port OUT-Channel............." );
	result = checkResult ( mDeviceLibDll->defineBit( mDevice[aDeviceID], 
		aPortID,aPosition,IDiscreteDevice::Transmit,&mOutBitID[aDeviceID][aInBitID] ) );
	return( result );
};
bool DiscreteProcedure::readBit( UInt32 aDeviceID,UInt32 aInBitID,Bool* aData )
{
	bool result=false;
	mReqTracer->printComments( "..............read a bit port from a IN-Channel............." );
	result = checkResult ( mDeviceLibDll->read( mInBitID[aDeviceID][aInBitID],aData ) );
	mReqTracer->printSingleData( result,*aData,aInBitID,false );
	return( result );
};
bool DiscreteProcedure::writeBit( UInt32 aDeviceID,UInt32 aInBitID,Bool aData )
{
	bool result=false;
	mReqTracer->printComments( "..............write a bit port To a OUT-Channel............." );
	result = checkResult ( mDeviceLibDll->write( mOutBitID[aDeviceID][aInBitID],aData ) );
	mReqTracer->printSingleData( result,aData,aInBitID,true );
	return( result );
};
 /***********END GPIA DISCRETE UNIQUE OPERATION*************/

DiscreteTestProOprt::DiscreteTestProOprt( DiscreteProcedure* aAnalogProcedure )
	:TestProOprt( aAnalogProcedure )
{
	mDeviceProcedure = ( DiscreteProcedure* )aAnalogProcedure;
};
void DiscreteTestProOprt::StepZeroInitTestData( )
{
	mRxData=false;
};
void DiscreteTestProOprt::StepEndCleanRCVbuffer( )
{
     mRxData=false;
};
void DiscreteTestProOprt::StepTwoDefineInputChannel( UInt32 aPortID,UInt32 aPoz )
{
	mReqTracer->printComments( "define an In Port" );
	mDeviceProcedure->defineInBitPort( 0,aPortID,aPoz,0 );
};
void DiscreteTestProOprt::StepThreeDefineOutputChannel( UInt32 aPortID,UInt32 aPoz )
{
	mReqTracer->printComments( "define an Out Port" );
	mDeviceProcedure->defineOutBitPort( 0,aPortID,aPoz,0 );
};

void DiscreteTestProOprt::StepSixReadInputChannel( void )
{
	
	mDeviceProcedure->readBit( 0,0,&mRxData );
};

void DiscreteTestProOprt::StepFiveWriteOutputChannel( Bool aBitVal )
{
	
	mDeviceProcedure->writeBit( 0,0,aBitVal );
};


bool DiscreteTestProOprt::StepLastMultiDevicesOperation( UInt32 aNumber ,char** aDeviceName )
{
	bool result=false;
	
	mDeviceProcedure->defineMultiDevices( aNumber,aDeviceName );

	
	result= mDeviceProcedure->defineInBitPort( 0,8,6,0 );
	
	result= mDeviceProcedure->defineOutBitPort( 0,2,6,0 );


	
	result= mDeviceProcedure->defineInBitPort( 1,8,6,0 );
	
	result= mDeviceProcedure->defineOutBitPort( 1,2,6,0 );

	StepFourOpenDevice( );

	mReqTracer->printComments( "Please Get the Register monitor In Use" );
    mReqTracer->printforset("Ready to out put false on device 0 , port 2(6); and after that ready to read on device 0, port 8(6)");
	
    mDeviceProcedure->writeBit( 0,0,false );
    Sleep(500);
	mDeviceProcedure->readBit( 0,0,&mRxData );
    mDeviceProcedure->writeBit( 0,0,true );
    Sleep(500);
	mDeviceProcedure->readBit( 0,0,&mRxData );

    mDeviceProcedure->writeBit( 0,0,false );
    Sleep(500);
	mDeviceProcedure->readBit( 0,0,&mRxData );

    mReqTracer->printforset("Ready to out put false on device 1, port 2(6); and after that ready to read on device 1, port 8(6)");
	mDeviceProcedure->writeBit( 1,0,true );	
    Sleep(500);
	mDeviceProcedure->readBit( 1,0,&mRxData );

    mDeviceProcedure->writeBit( 1,0,false );	
    Sleep(500);
	mDeviceProcedure->readBit( 1,0,&mRxData );
    mDeviceProcedure->writeBit( 1,0,true );	
    Sleep(500);
	mDeviceProcedure->readBit( 1,0,&mRxData );
    mDeviceProcedure->writeBit( 1,0,false );	
    Sleep(500);
	mDeviceProcedure->readBit( 1,0,&mRxData );
    mDeviceProcedure->writeBit( 1,0,true );	
    Sleep(500);
	mDeviceProcedure->readBit( 1,0,&mRxData );
 
	return( result );
};


bool DiscreteTestProOprt::StepTRXErrorTestTwo( )
{
	bool result=false;
    
	
	StepOneDefine1Device( "GPIA000070" );

	StepTwoDefineInputChannel( 8,6 );
	StepThreeDefineOutputChannel( 2,6 );

	mReqTracer->printComments( "Try to Write Bits that Device not opened" );
	StepFiveWriteOutputChannel( true );

	mReqTracer->printComments( "Try to Read Bits that Device not opened" );
	StepSixReadInputChannel( );

    return( result );
};
