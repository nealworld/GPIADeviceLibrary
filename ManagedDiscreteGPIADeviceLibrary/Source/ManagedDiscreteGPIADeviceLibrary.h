// CSharpWrapper_DiscreteGPIADeviceLibrary.h

#pragma once

#include <msclr\marshal.h>

#include "IOA\IDiscreteDevice.h"
extern "C" __declspec(dllimport) IDiscreteDevice* createDevice( void );
extern "C" __declspec(dllimport) void destroyDevice( IDiscreteDevice* aDevice );

using namespace msclr::interop;
using namespace System;
using namespace System::Runtime::InteropServices;

namespace ManagedDiscreteGPIADeviceLibrary {

	public enum class ResultWrapper
	{
		ResultSuccess = 0,
		ResultNoData = 1,
		ResultNotEnoughMemory = 2,
		ResultOverflow = 3,
		ResultInvalidParameter = 4,
		ResultNotAvailable = 5,
		ResultNotSupported = 6,
		ResultInvalidOperation = 7
	};

	public enum class DirectionWrapper
	{
		Transmit = 0,
		Receive = 1
	};

	public ref class ManagedDiscreteGPIADeviceLib
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////
		// Constructor
		//
		ManagedDiscreteGPIADeviceLib();
		////////////////////////////////////////////////////////////////////////////////////////////////
		// Desctructor
		//
		~ManagedDiscreteGPIADeviceLib();

		ResultWrapper defineDevice( unsigned int aID, String^ aConfig, int % aDeviceID );

		ResultWrapper defineBit( int aDeviceID, unsigned int aPortNumber,
                                     unsigned int aBitPosition, DirectionWrapper aDirection,
                                     int % aBitID );
		ResultWrapper open( void );
		ResultWrapper close( void );

		ResultWrapper read( int aBitId, bool % aData );
		ResultWrapper write( int aBitId, bool aData );
		ResultWrapper getLastError( String^ aBuffer, unsigned int aBufferLength );

		unsigned int getInterfaceVersion( void );

	protected:
	private:
		ResultWrapper convertResult( int aResult );
		IDiscreteDevice* mDiscreteDevice;
		bool mNeedDelete;
	};
}
