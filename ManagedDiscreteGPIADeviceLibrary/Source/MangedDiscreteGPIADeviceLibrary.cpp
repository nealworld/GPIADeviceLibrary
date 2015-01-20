// This is the main DLL file.

#include "stdafx.h"

#include "ManagedDiscreteGPIADeviceLibrary.h"

namespace ManagedDiscreteGPIADeviceLibrary {

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Constructor
	//
	ManagedDiscreteGPIADeviceLib::ManagedDiscreteGPIADeviceLib()
	{
		mDiscreteDevice = createDevice();
		mNeedDelete = true;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Desctructor
	//
	ManagedDiscreteGPIADeviceLib::~ManagedDiscreteGPIADeviceLib()
	{
		destroyDevice( mDiscreteDevice );
		mDiscreteDevice = nullptr;
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// defineDevice
	//
	ResultWrapper ManagedDiscreteGPIADeviceLib::defineDevice( unsigned int aID, String^ aConfig, int % aDeviceID )
	{
		ResultWrapper lResult = ResultWrapper::ResultSuccess;
		IntPtr p = Marshal::StringToHGlobalAnsi( aConfig );
		char* pConfig = static_cast<char*>(p.ToPointer()); 

		IDiscreteDevice::DeviceID lDeviceID = (void*)( -1 );
		int lIntResult = ( int ) mDiscreteDevice->defineDevice( aID, pConfig, &lDeviceID );
		aDeviceID = (int)lDeviceID;
		Marshal::FreeHGlobal(p); 

		lResult = convertResult( lIntResult );
		return lResult;
	};

		
	////////////////////////////////////////////////////////////////////////////////////////////////
	// defineBit
	//
	ResultWrapper ManagedDiscreteGPIADeviceLib::defineBit( int aDeviceID, unsigned int aPortNumber,
		unsigned int aBitPosition, DirectionWrapper aDirection,
		int% aBitID )
	{
		ResultWrapper lResult = ResultWrapper::ResultSuccess;
		IDiscreteDevice::Direction lDirection = ( IDiscreteDevice_V1::Direction )aDirection; 
		IDiscreteDevice::DeviceID lDeviceID = ( IDiscreteDevice::DeviceID )aDeviceID;
		IDiscreteDevice::BitID lBitID = ( void* )( -1 );
		int lIntResult = ( int )( mDiscreteDevice->defineBit( lDeviceID, aPortNumber, aBitPosition, lDirection, &lBitID ) );
		aBitID = ( int )lBitID;
		lResult = convertResult( lIntResult );
		return lResult;
	};

		
	////////////////////////////////////////////////////////////////////////////////////////////////
	// open
	//
	ResultWrapper ManagedDiscreteGPIADeviceLib::open( void )
	{
		ResultWrapper lResult = ResultWrapper::ResultSuccess;
		int lIntResult = ( int )mDiscreteDevice->open();
		lResult = convertResult( lIntResult );
		return lResult;
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// close
	//
	ResultWrapper ManagedDiscreteGPIADeviceLib::close( void )
	{
		ResultWrapper lResult = ResultWrapper::ResultSuccess;
		int lIntResult = ( int )mDiscreteDevice->close();
		lResult = convertResult( lIntResult );
		return lResult;
	};
		
	////////////////////////////////////////////////////////////////////////////////////////////////
	// read
	//
	ResultWrapper ManagedDiscreteGPIADeviceLib::read( int aBitId, bool % aData )
	{
		ResultWrapper lResult = ResultWrapper::ResultSuccess;
		void* lBitID = ( void* )aBitId;
		IOA::Bool lData = true;
		int lIntResult = ( int )mDiscreteDevice->read( ( IDiscreteDevice_V1::BitID )lBitID, &lData );
		aData = (bool)lData;
		lResult = convertResult( lIntResult );
		return lResult;
	};
		
	////////////////////////////////////////////////////////////////////////////////////////////////
	// write
	//
	ResultWrapper ManagedDiscreteGPIADeviceLib::write( int aBitId, bool aData )
	{
		ResultWrapper lResult = ResultWrapper::ResultSuccess;
		void* lBitID = ( void* )aBitId;
		int lIntResult = ( int )mDiscreteDevice->write( ( IDiscreteDevice_V1::BitID )lBitID, ( const IOA::Bool )aData );
		lResult = convertResult( lIntResult );
		return lResult;
	};
		
	////////////////////////////////////////////////////////////////////////////////////////////////
	// getLastError
	//
	ResultWrapper ManagedDiscreteGPIADeviceLib::getLastError( String^ aBuffer, unsigned int aBufferLength )
	{
		ResultWrapper lResult = ResultWrapper::ResultSuccess;
		IntPtr p = Marshal::StringToHGlobalAnsi( aBuffer );
		char* pBuffer = static_cast<char*>(p.ToPointer()); 

		int lIntResult = ( int )mDiscreteDevice->getLastError( pBuffer, aBufferLength );

		lResult = convertResult( lIntResult );
		Marshal::FreeHGlobal(p); 
		return lResult;
	};
		
	////////////////////////////////////////////////////////////////////////////////////////////////
	// convertResult
	//
	ResultWrapper ManagedDiscreteGPIADeviceLib::convertResult( int aResult )
	{
		ResultWrapper lResult = ResultWrapper::ResultSuccess;
		switch( aResult )
		{
		case 0:
			lResult = ResultWrapper::ResultSuccess;
			break;
		case 1:
			lResult = ResultWrapper::ResultNoData;
			break;
		case 2:
			lResult = ResultWrapper::ResultNotEnoughMemory;
			break;
		case 3:
			lResult = ResultWrapper::ResultOverflow;
			break;
		case 4:
			lResult = ResultWrapper::ResultInvalidParameter;
			break;
		case 5:
			lResult = ResultWrapper::ResultNotAvailable;
			break;
		case 6:
			lResult = ResultWrapper::ResultNotSupported;
			break;
		case 7:
			lResult = ResultWrapper::ResultInvalidOperation; 
			break;
		default:
			break;
		}
		return lResult;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// getInterfaceVersion
	//
    unsigned int ManagedDiscreteGPIADeviceLib::getInterfaceVersion( void )
    {
        return mDiscreteDevice->getInterfaceVersion();
    }
}