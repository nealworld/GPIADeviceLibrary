///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2012 GE-AVIC Civil Avionics Systems Company Limited
// ------------------------------------------------------------------------------------------------
//                      GE-AVIC Civil Avionics Systems Company Limited  PROPRIETARY INFORMATION
// The information contained in this document is GE-AVIC Civil Avionics Systems Company Limited
// proprietary information and is disclosed in confidence.  It is the property of GE-AVIC Civil
// Avionics Systems Company Limited and shall not be used, disclosed to others or reproduced
// without the express written consent of GE-AVIC Civil Avionics Systems Company Limited,
// including, but without limitation, it is not to be used in the creation,
// manufacture, development, or derivation of any repairs, modifications, spare parts, designs, or
// configuration changes or to obtain FAA or any other government or regulatory approval to do so.
// If consent is given for reproduction in whole or in part, this notice and the notice set forth
// on each page of this document shall appear in any such reproduction in whole or in part.
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <sstream>
#include <iomanip>
#if !defined( DISCRETEGPIADEVICELIBRARY_H )
    #include "DiscreteGPIADeviceLibrary.h"
#endif

#if !defined ( TOOL_STRINGUTILITY_H )
    #include "Tool/StringUtility.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
//  file scope variables

//Upper bound of a BitPosition value
static unsigned int fMaxBitPosition = 7u;

//command value for detecting GPIA devices.
static const char* fDetectCmd = "GPIA_DETECT";

//ethernet port number to send the detect command.
static const unsigned int fDetectPort = 30303u;

// the port number used by GPIA controller to send UDP message.
static const USHORT fMsgPort = 8412;

//time out value for receive message. Unit - ms.
static const int fTimeOut = 200;

//token separator for parsing device information
static const std::string fTokenSeparator = ";";

//Prefix of GPIA device information, used for validate device information string
static const std::string fPrefix = "GPIA";

//count of token that contained in device information string
static const int fTokenCount = 7;

//indice of different token in device information string
static const int fPrefixIndex = 0;
static const int fPartNumberIndex = 1;
static const int fHostNameIndex = 2;
static const int fPortCountIndex = 6;

static const int fInvalidHandle = -1;

static const int fLoopPeriod = 100;
////////////////////////////////////////////////////////////////////////////////////////////////////
//  Default Constructor
//      Initial member vairables, create a broadcast UDP socket to detect GPIA devices.
DiscreteGPIADeviceLibrary::DiscreteGPIADeviceLibrary( void )
:   InheritedType(),
    mIsOpen( false ),
    mLastError( "" ),
    mUDPSocket(),
    mCurrentDeviceIndex( 0 ),
    mCurrentBitIndex( 0 )
{
    //startup WSA to use socket functions.
    if ( OperatingSystem::WSAUDPSocket::startup() )
    {
        //create UDPSocket, set it to broadcast and non-block
        if ( mUDPSocket.open() )
        {
            if ( mUDPSocket.config( false, true ) )
            {
                //detectDevice();
            }
        }
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Destructor
//      release all devices and unload WSA socket
DiscreteGPIADeviceLibrary::~DiscreteGPIADeviceLibrary( void )
{
    mUDPSocket.close();
    //Unload WSA socket
    OperatingSystem::WSAUDPSocket::cleanup();
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// defineDevice
//   find the device that matches given host name, and mark it as activated.
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::defineDevice( IOA::UInt32 aID,
                                                    const IOA::Char* aConfig, DeviceID* aDeviceID )
{
    Result lResult( ResultSuccess );
    *aDeviceID = NULL;

    if ( !mIsOpen )
    {
        int lDeviceIndex = detectDevice( aConfig );
        if ( lDeviceIndex != fInvalidHandle )
        {
            *aDeviceID = reinterpret_cast<DeviceID>( lDeviceIndex );
            mDevices[ lDeviceIndex ].mIsActivated = true;
        }
        else
        {
            lResult = ResultNotAvailable;
            std::stringstream lErrMsg;
            lErrMsg << "Device not found. IPAddress = " << aConfig;
            mLastError = lErrMsg.str();
        }
    }
    else
    {
        lResult = ResultInvalidOperation;
        std::stringstream lErrMsg;
        lErrMsg << "The devices associated with this device library have already opened,"
                << " no more definition allowed.";
        mLastError = lErrMsg.str();
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// defineBit
//   defined a bit, add it to bit map. This function actually treate a single bit as a special
//   BitGroup of which the bit mask has only one bit set to 1.
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::defineBit( DeviceID aDeviceID,
                                                IOA::UInt32 aPortNumber, IOA::UInt32 aBitPosition,
                                                Direction aDirection, BitID* aBitID )
{
    Result lResult( ResultSuccess );
    *aBitID = NULL;

    //check if bit position is valid
    if ( aBitPosition <= fMaxBitPosition )
    {
        //generate single bit mask from bit position
        IOA::UInt8 lBitMask = 0x01 << aBitPosition;

        //actually define a BitGroup
        lResult = defineBitGroup( aDeviceID, aPortNumber, lBitMask, aDirection, aBitID );
    }
    else
    {
        lResult = ResultInvalidParameter;
        std::stringstream lErrMsg;
        lErrMsg << "BitPosition should not exceed " << fMaxBitPosition
                << ". DeviceID = " << aDeviceID
                << "PortNumber = " << aPortNumber
                << "BitPosition = " << aBitPosition;
        mLastError = lErrMsg.str();
    }

    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// defineBitGroup
//   defined a BitGroup, add it to bit map.
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::defineBitGroup( DeviceID aDeviceID,
                                                IOA::UInt32 aPortNumber, IOA::UInt8 aBitMask,
                                                Direction aDirection, BitID* aBitID )
{
    Result lResult( ResultSuccess );
    *aBitID = NULL;

    if ( !mIsOpen )
    {
        //check if deviceID is valid
        int lDeviceIndex = reinterpret_cast<int>( aDeviceID );
        if ( lDeviceIndex > fInvalidHandle && lDeviceIndex < mCurrentDeviceIndex )
        {
            //check if this device has already defined for use.
            if ( mDevices[lDeviceIndex].mIsActivated )
            {
                if ( aPortNumber < mDevices[lDeviceIndex].mPortCount )
                {
                    BitGroup lNewBitGroup;
                    lNewBitGroup.mBitMask = aBitMask;
                    lNewBitGroup.mDirection = aDirection;
                    lNewBitGroup.mPortNumber = aPortNumber;

                    mBitGroups.push_back( lNewBitGroup );
                    mDevices[ lDeviceIndex ].mBitGroups.push_back( mCurrentBitIndex );
                    mDevices[ lDeviceIndex ].mDefinedPort.insert( aPortNumber );
                    mBitGroupMap[ mCurrentBitIndex ] = lDeviceIndex;
                    *aBitID = reinterpret_cast<BitID>( mCurrentBitIndex );

                    mCurrentBitIndex++;
                }
                else
                {
                    lResult = ResultInvalidParameter;
                    std::stringstream lErrMsg;
                    lErrMsg << "PortNumber should not exceed "
                            << mDevices[lDeviceIndex].mPortCount << "on this device."
                            << ". DeviceID = " << aDeviceID
                            << "PortNumber = " << aPortNumber
                            << "BitMask = " << aBitMask;
                    mLastError = lErrMsg.str();
                }
            }
            else
            {
                lResult = ResultInvalidParameter;
                std::stringstream lErrMsg;
                lErrMsg << "Device has not been defined yet. DeviceID = " << aDeviceID
                        << "PortNumber = " << aPortNumber
                        << "BitMask = " << aBitMask;
                mLastError = lErrMsg.str();
            }
        }
        else
        {
            lResult = ResultInvalidParameter;
            std::stringstream lErrMsg;
            lErrMsg << "Invalid DeviceID. DeviceID = " << aDeviceID;
            mLastError = lErrMsg.str();
        }
    }
    else
    {
        lResult = ResultInvalidOperation;
        std::stringstream lErrMsg;
        lErrMsg << "The devices associated with this device library have already opened,"
                << " no more definition allowed.";
        mLastError = lErrMsg.str();
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  open
//      open every activated device.
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::open( void )
{
    Result lResult( ResultSuccess );
    std::stringstream lStream;

    for ( int lIndex = 0; lIndex < mCurrentDeviceIndex; lIndex++ )
    {
        Device& lDevice = mDevices[ lIndex ];
        if ( lDevice.mIsActivated )
        {
            if ( lDevice.mMsgSocket.open() && lDevice.mReadSocket.open())
            {
                lDevice.mMsgSocket.config( false, true );
				lDevice.mReadSocket.config(false, true);
            }
            else
            {
                lResult = ResultNotAvailable;
                lStream << "Could not open UDP socket. HostName = " << lDevice.mHostName
                       << ", IP Address = " << lDevice.mIPAddress << "." << std::endl;
                mLastError = lStream.str();
            }
        }
    }
    if ( lResult == ResultSuccess )
    {
		for ( int lIndex = 0; lIndex < mCurrentDeviceIndex; lIndex++ )
		{
			Device& lDevice = mDevices[ lIndex ];
			lDevice.mThread = std::tr1::shared_ptr<boost::thread>(
				new boost::thread(
				boost::bind(
				&DiscreteGPIADeviceLibrary::readData, this,lIndex ) ) );
		}
		mIsOpen = true;
    }

    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// close
//   Closes all activated devices.
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::close( void )
{
    Result lResult( ResultSuccess );
    std::stringstream lStream;
    if ( mIsOpen )
    {
        //shutdown reading thread
        mIsOpen = false;

        for ( int lIndex = 0; lIndex < mCurrentDeviceIndex; lIndex++ )
        {
            Device& lDevice = mDevices[ lIndex ];
            if ( lDevice.mIsActivated )
            {
				lDevice.mThread->join();
				lDevice.mThread.reset();
				if ( lDevice.mMsgSocket.close() && lDevice.mReadSocket.close())
                {
                    lDevice.mIsActivated = false;
                    lDevice.mBitGroups.clear();
                    lDevice.mDefinedPort.clear();
                }
                else
                {
                    lResult = ResultNotAvailable;
                    lStream << "Could not close UDP socket. HostName = " << lDevice.mHostName
                           << ", IP Address = " << lDevice.mIPAddress << "." << std::endl;
                    mLastError = lStream.str();
                }
            }
        }

        //reset state variables
        mBitGroups.clear();
        mBitGroupMap.clear();
        mCurrentBitIndex = 0;
#if defined( _DEBUG )
    mErrorLogFile.flush();
    mErrorLogFile.close();
#endif
    }
    return lResult;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// read
//   call readBitGroup todo the actual writting and turn read result to boolean value.
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::read( BitID aBitID, IOA::Bool* aData )
{
    IOA::UInt8 lData = 0;
    Result lResult = readBitGroup( aBitID, &lData );

    if ( lResult == ResultSuccess )
    {
        *aData = ( lData != 0 );
    }

    return lResult;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// write
//   turn boolean data to UInt8 and call writeBitGroup to do the actual writting.
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::write( BitID aBitID,
                                                                    const IOA::Bool aData )
{
    //turn boolean data to UInt8, the true value is set to 0xFF thus we don't need to know the
    //bit position of the target Bit, the bit mask will make sure the correct bit is operated.
    IOA::UInt8 lData = aData ? 0xFF : 0x00;
    return writeBitGroup( aBitID, lData );
}

///////////////////////////////////////////////////////////////////////////////////////////////
// readBitGroup
//  find the corresponding device and port, call read function of found device to read data
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::readBitGroup( BitID aBitID,
                                                                        IOA::UInt8* aData )
{
    Result lResult( ResultSuccess );
    if ( mIsOpen )
    {
        //check if the bit ID is valid.
        int lBitIndex = reinterpret_cast<int>( aBitID );
        if ( lBitIndex > fInvalidHandle && lBitIndex < mCurrentBitIndex )
        {
            BitGroup& lBitGroup = mBitGroups[ lBitIndex ];
            if ( lBitGroup.mDirection == Receive )
            {
                int lDeviceIndex = mBitGroupMap[ lBitIndex ];

                mDevices[ lDeviceIndex ].mReadMutex.lock();
                //check if the data is valid
                if ( mDevices[ lDeviceIndex ].mPortCache[ lBitGroup.mPortNumber ].second )
                {
                    *aData = mDevices[ lDeviceIndex ].mPortCache[ lBitGroup.mPortNumber ].first & lBitGroup.mBitMask;
                }
                else
                {
                    lResult = ResultInvalidOperation;
                    std::stringstream lErrMsg;
                    lErrMsg << "Read Failed. BitID = " << aBitID;
                    mLastError = lErrMsg.str();
                }
                mDevices[ lDeviceIndex ].mReadMutex.unlock();
            }
        }
        else
        {
            lResult = ResultInvalidParameter;
            std::stringstream lErrMsg;
            lErrMsg << "Invalid BitID. BitID = " << aBitID;
            mLastError = lErrMsg.str();
        }
    }
    else
    {
        lResult = ResultInvalidOperation;
        mLastError = "The devices associated with this device library are not opened yet.";
    }
    return lResult;
}
///////////////////////////////////////////////////////////////////////////////////////////////
// writeBitGroup
//  find the corresponding device and port, call write function of found device to write data
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::writeBitGroup( BitID aBitID, const IOA::UInt8 aData )
{
    Result lResult( ResultSuccess );
    if ( mIsOpen )
    {
        //check if the bit ID is valid
        int lBitIndex = reinterpret_cast<int>( aBitID );
        if ( lBitIndex > fInvalidHandle && lBitIndex < mCurrentBitIndex )
        {
            GPIAMessage lCmdMsg;

            lCmdMsg.setPortNumber( mBitGroups[ lBitIndex ].mPortNumber );
            lCmdMsg.setBitMask( mBitGroups[ lBitIndex ].mBitMask );
            lCmdMsg.setData( aData );
            lCmdMsg.setCode( GPIAMessage::Write );

            GPIAMessage lResultMsg;
            //send command message and get result back
            if ( accessGPIA( mDevices[ mBitGroupMap[ lBitIndex ] ], lCmdMsg, lResultMsg ) )
            {
                //check result
                if ( ( lCmdMsg.getPortNumber() != lResultMsg.getPortNumber() )
                        || ( lResultMsg.getStatus() != GPIAMessage::Normal ) )
                {
                    lResult = ResultInvalidOperation;
                    std::stringstream lErrMsg;
                    lErrMsg << "Failed to write data. HostName = " << mDevices[ mBitGroupMap[ lBitIndex ] ].mHostName
                        << ", PortNumber = " << mBitGroups[ lBitIndex ].mPortNumber;
                    mLastError = lErrMsg.str();
                }
            }
            else
            {
                lResult = ResultInvalidOperation;
                std::stringstream lErrMsg;
                lErrMsg << "Cannot receive response from GPIA. HostName = " << mDevices[ mBitGroupMap[ lBitIndex ] ].mHostName;
                mLastError = lErrMsg.str();
            }
        }
        else
        {
            lResult = ResultInvalidParameter;
            std::stringstream lErrMsg;
            lErrMsg << "invalid BitID. BitID = " << aBitID;
            mLastError = lErrMsg.str();
        }
    }
    else
    {
        lResult = ResultInvalidOperation;
        mLastError = "The devices associated with this device library are not opened yet.";
    }
    return lResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////
// getLastError
//   Gets the last error message created by the Discrete device driver.
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::getLastError( IOA::Char* aBuffer,
                                                                    IOA::UInt32 aBufferLength )
{
    Result lResult( ResultSuccess );
    if ( aBuffer != NULL )
    {
        strncpy_s( aBuffer, aBufferLength, mLastError.c_str(), _TRUNCATE );
    }
    else
    {
        lResult = ResultInvalidParameter;
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// getRuntimeFeatureStatus
//   Gets the status of the specified feature.
//  The content of enum RuntimeFeature is TBD, so return false directly.
IOA::Bool IOACall DiscreteGPIADeviceLibrary::getFeatureStatus( RuntimeFeature aFeature )
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  startTraceLog
//      start recording received protocol data of a device. Recorded data will be stored in a
//      .csv file.
//
//      Parameters:
//          aDeviceID - handle of the device to start data recording.
//          aFileName - determines name and full path of the record csv file.
//
//      Return Values:
//          ResultSuccess - Successfully started data recording.
//          ResultInvalidParameter - One-or-more parameters is an invalid value.
//          ResultInvalidOperation - The devices associated with the class are not open.
//          ResultNotAvailable - failed to start data recording.
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::startTraceLog( IDiscreteDevice::DeviceID aDeviceID, const IOA::Char* aFileName )
{
    Result lResult = ResultSuccess;
    if ( mIsOpen )
    {
        int lDeviceIndex = reinterpret_cast<int>( aDeviceID );
        if ( lDeviceIndex > fInvalidHandle && lDeviceIndex < mCurrentDeviceIndex )
        {
            Device& lDevice = mDevices[ lDeviceIndex ];
            lDevice.mRecordMutex.lock();
            if ( !lDevice.mIsRecording )
            {
                std::ofstream& lStream = lDevice.mRecordingStream;
                lStream.clear();
                //let the stream throw an exception if any error
                lStream.exceptions( std::ios_base::badbit | std::ios_base::failbit );
                try
                {
                    lStream.open( aFileName );
                    //output the column headers
                    lStream << std::left
                            << std::setw( 32 ) << "Timestamp,"
                            << std::setw( 16 ) << "PortNumber,"
                            << std::setw( 16 ) << "Data,"
                            << std::endl;

                    lDevice.mIsRecording = true;
                }
                catch( std::ios::failure& )
                {
                    lStream.clear();
                    lStream.close();
                    lResult = ResultNotAvailable;
                    std::stringstream lErrMsg;
                    lErrMsg << "Failed to create log file. DeviceID = " << lDeviceIndex
                            << ", FileName = " << aFileName;
                    mLastError = lErrMsg.str();
                }
            }
            else
            {
                lResult = ResultNotAvailable;
                std::stringstream lErrMsg;
                lErrMsg << "This device is currently recording data. DeviceID = " << lDeviceIndex;
                mLastError = lErrMsg.str();
            }
            lDevice.mRecordMutex.unlock();
        }
        else
        {
            lResult = ResultInvalidParameter;
            std::stringstream lErrMsg;
            lErrMsg << "Invalid DeviceID. DeviceID = " << lDeviceIndex;
            mLastError = lErrMsg.str();
        }
    }
    else
    {
        lResult = ResultInvalidOperation;
        mLastError = "The devices associated with this device library are not opened yet.";
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  stopTraceLog
//      stop recording received protocol data of a device.
//
//      Parameters:
//          aDeviceID - handle of the device to stop data recording.
//
//      Return Values:
//          ResultSuccess - Successfully stopped data recording.
//          ResultInvalidParameter - One-or-more parameters is an invalid value.
//          ResultInvalidOperation - The devices associated with the class are not open.
//          ResultNotAvailable - failed to stop data recording.
//
IDiscreteDevice::Result IOACall DiscreteGPIADeviceLibrary::stopTraceLog( IDiscreteDevice::DeviceID aDeviceID )
{
    Result lResult = ResultSuccess;
    if ( mIsOpen )
    {
        int lDevice = reinterpret_cast<int>( aDeviceID );
        if ( lDevice > fInvalidHandle && lDevice < mCurrentDeviceIndex )
        {
            //lock for modification
            mDevices[ lDevice ].mRecordMutex.lock();
            if ( mDevices[ lDevice ].mIsRecording )
            {
                mDevices[ lDevice ].mIsRecording = false;
                std::ofstream& lStream = mDevices[ lDevice ].mRecordingStream;
                lStream.close();
            }
            else
            {
                lResult = ResultNotAvailable;
                std::stringstream lErrMsg;
                lErrMsg << "This device is not currently recording data. DeviceID = " << lDevice;
                mLastError = lErrMsg.str();
            }
            mDevices[ lDevice ].mRecordMutex.unlock();
        }
        else
        {
            lResult = ResultInvalidParameter;
            std::stringstream lErrMsg;
            lErrMsg << "Invalid DeviceID. DeviceID = " << lDevice;
            mLastError = lErrMsg.str();
        }
    }
    else
    {
        lResult = ResultInvalidOperation;
        mLastError = "The devices associated with this device library are not opened yet.";
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  detectDevice
//      send detect command and get response from GPIA devices. If new device detected, add it to
//      mDevices.
int DiscreteGPIADeviceLibrary::detectDevice( std::string aIPString )
{
    static char lRecvBuff[100];
    int lDeviceIndex = fInvalidHandle;
    unsigned long lIPAddress = OperatingSystem::WSAUDPSocket::stringToIP( aIPString );
    //send detect command
    if ( mUDPSocket.sendTo( fDetectCmd, strlen(fDetectCmd), lIPAddress, fDetectPort ) )
    {
        unsigned int lReceiveLength = 0;
        Sleep( 200 );//wait for response
        //process all response we received
        while ( mUDPSocket.receive( lRecvBuff, sizeof(lRecvBuff), 1, &lReceiveLength ) )
        {
            if ( lIPAddress == mUDPSocket.getClientIPAddr() )
            {
                std::vector< std::string > lTokenList;
                //make sure the device information string is in correct format
                if ( Tool::StringUtility::parseTokenString( lRecvBuff, fTokenSeparator, lTokenList )
                        == fTokenCount )
                {
                    //make sure the string comes from a GPIA device
                    if ( lTokenList[fPrefixIndex].find( fPrefix ) == 0 )
                    {
                        Device* lDevice = NULL;
                        //if IPAddress not in device list yet, create a new device
                        if ( mDeviceMap.find( lIPAddress ) == mDeviceMap.end() )
                        {
                            Device lNewDevice;
                            mDevices.push_back( lNewDevice );
                            mDeviceMap[ lIPAddress ] = mCurrentDeviceIndex;
                            mCurrentDeviceIndex++;
                        }
                        lDeviceIndex = mDeviceMap[ lIPAddress ];
                        lDevice = &mDevices[ lDeviceIndex ];
                        unsigned int lPortCount = atoi( lTokenList[ fPortCountIndex ].c_str() );
                        lDevice->mHostName = lTokenList[ fHostNameIndex ];
                        lDevice->mPartNumber = lTokenList[ fPartNumberIndex ];
                        lDevice->mIPAddress = mUDPSocket.getClientIPAddr();
                        lDevice->mPortCount = lPortCount;
                        lDevice->mPortCache.resize( lPortCount );
                    }
                }
            }
        }
    }
    return lDeviceIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  accessGPIA
//      send command message to the GPIA device, and then read result message back .
//
bool DiscreteGPIADeviceLibrary::accessGPIA( Device& aDevice, const GPIAMessage& aCmdMsg, GPIAMessage& aResultMsg )
{
    bool lResult = false;
    aDevice.mSocketMutex.lock();

    const char* lCommand = aCmdMsg.getMessageBuffer();
    unsigned int lBufferSize = GPIAMessage::getBufferSize();
    char* lRecvBuff = new char[ lBufferSize ];
    //send command to GPIA
    if ( aDevice.mMsgSocket.sendTo( aCmdMsg.getMessageBuffer(), GPIAMessage::getBufferSize(),
                            aDevice.mIPAddress, fMsgPort ) )
    {
        unsigned int lRecvLength = 0;

        //receive result message from GPIA
        if ( aDevice.mMsgSocket.receive( lRecvBuff, GPIAMessage::getBufferSize(), fTimeOut, &lRecvLength ) )
        {
            aResultMsg.setContent( lRecvBuff, GPIAMessage::getBufferSize() );
            lResult = true;
            #if defined( _DEBUG )
            if ( aResultMsg.getStatus() != GPIAMessage::Normal )
            {
                mErrorLogFile << "<<accessGPIA FUN>> Operation failed on GPIA. Device = " << aDevice.mHostName
                    << ", IP = " << aDevice.mMsgSocket.IPToString( aDevice.mIPAddress )
                    << ", Command = " << std::hex;
                for ( int lIndex = 0; lIndex < lBufferSize; lIndex++ )
                {
                    mErrorLogFile << (unsigned int)(unsigned char)lCommand[ lIndex ] << " ";
                }
                mErrorLogFile << ", Result = ";
                for ( int lIndex = 0; lIndex < lBufferSize; lIndex++ )
                {
                    mErrorLogFile << (unsigned int)(unsigned char)lRecvBuff[ lIndex ] << " ";
                }
                mErrorLogFile << std::endl;
            }
            #endif
        }
        else
        {
            #if defined( _DEBUG )
            mErrorLogFile << "<<accessGPIA FUN>> Failed to receive reponse from GPIA. Device = " << aDevice.mHostName
                << ", IP = " << aDevice.mMsgSocket.IPToString( aDevice.mIPAddress )
                << ", Command = " << std::hex;
            for ( int lIndex = 0; lIndex < lBufferSize; lIndex++ )
            {
                mErrorLogFile << (unsigned int)(unsigned char)lCommand[ lIndex ] << " ";
            }
            mErrorLogFile << std::endl;
            #endif
        }
    }
    else
    {
        #if defined( _DEBUG )
        mErrorLogFile << "<<accessGPIA FUN>> Failed to send command to GPIA. Device = " << aDevice.mHostName
            << ", IP = " << aDevice.mMsgSocket.IPToString( aDevice.mIPAddress )
            << ", Command = " << std::hex;
        for ( int lIndex = 0; lIndex < lBufferSize; lIndex++ )
        {
            mErrorLogFile << (unsigned int)(unsigned char)lCommand[ lIndex ] << " ";
        }
        mErrorLogFile << std::endl;
        #endif
    }
    aDevice.mSocketMutex.unlock();
    delete[] lRecvBuff;
    lRecvBuff = NULL;
    return lResult;
}
/*
void DiscreteGPIADeviceLibrary::readData( void )
{
    GPIAMessage lCmdMsg;
    lCmdMsg.setCode( GPIAMessage::Read );
    lCmdMsg.setBitMask( 0xFF );
    GPIAMessage lResultMsg;
    while ( mIsOpen )
    {
        for ( int lIndex = 0; lIndex < mCurrentDeviceIndex; lIndex++ )
        {
            Device& lDevice = mDevices[ lIndex ];
            if ( lDevice.mIsActivated )
            {
                std::set<int>::iterator lPortIter = lDevice.mDefinedPort.begin();
                for ( ; lPortIter != lDevice.mDefinedPort.end(); lPortIter++ )
                {
                    int lPort = *lPortIter;
                    lCmdMsg.setPortNumber( lPort );
                    if ( accessGPIA( lDevice, lCmdMsg, lResultMsg ) )
                    {
                        lDevice.mReadMutex.lock();
                        lDevice.mPortCache[ lPort ].first = lResultMsg.getData();
                        //only if the status is normal, set the validity to true
                        lDevice.mPortCache[ lPort ].second = ( lResultMsg.getStatus() == GPIAMessage::Normal );
                        lDevice.mReadMutex.unlock();

                        lDevice.mRecordMutex.lock();
                        if ( lDevice.mIsRecording )
                        {
                            if ( !saveData( lIndex, lPort, lResultMsg.getData() ) )
                            {
                                lDevice.mIsRecording = false;
                                lDevice.mRecordingStream.close();
                            }
                        }
                        lDevice.mRecordMutex.unlock();
                    }
                    else
                    {
                        lDevice.mReadMutex.lock();
                        //if no response from GPIA, set the validity of the data to false
                        lDevice.mPortCache[ lPort ].second = false;
                        lDevice.mReadMutex.unlock();
                    }
                }
            }
        }
        Sleep( fLoopPeriod );
    }
    return;
}
*/

void DiscreteGPIADeviceLibrary::sendAllCmds(Device& aDevice, std::map<int, bool>& aSendCmdResult_map, 
	std::map<int, GPIAMessage>& aSendCmd_map)
{

	GPIAMessage lCmdMsg;
    lCmdMsg.setCode( GPIAMessage::Read );
    lCmdMsg.setBitMask( 0xFF );
	std::set<int>::iterator lPortIter = aDevice.mDefinedPort.begin();
    for ( ; lPortIter != aDevice.mDefinedPort.end(); lPortIter++ )
    {
        int lPort = *lPortIter;
        lCmdMsg.setPortNumber( lPort );

#if defined( _DEBUG )
		const char* lCommand = lCmdMsg.getMessageBuffer();
		aDevice.mlog << "<<sendAllCmds FUN>> send command to GPIA. Device = " << aDevice.mHostName
				<< ", IP = " << aDevice.mReadSocket.IPToString(aDevice.mIPAddress)
				<< ", Command = " << std::hex;
		for (int lIndex = 0; lIndex < GPIAMessage::getBufferSize(); lIndex++)
		{
			aDevice.mlog << (unsigned int)(unsigned char)lCommand[lIndex] << " ";
		}
#endif
		//send command to GPIA
		if (aDevice.mReadSocket.sendTo(lCmdMsg.getMessageBuffer(), GPIAMessage::getBufferSize(),
			aDevice.mIPAddress, fMsgPort))
		{
			aSendCmdResult_map[lPort] = true;
			aSendCmd_map[lPort] = lCmdMsg;

#if defined( _DEBUG )
			aDevice.mlog << " success "<<std::endl;
#endif
		}
		else{
			aSendCmdResult_map[lPort] = false;

#if defined( _DEBUG )
			aDevice.mlog << " false "<<std::endl;
#endif
		}
    }
#if defined( _DEBUG )
	aDevice.mlog.flush();
#endif
}

void DiscreteGPIADeviceLibrary::recvAll(int aCurrentDeviceIndex, std::map<int, bool>& aSendCmdResult_map, 
	std::map<int, GPIAMessage>& aSendCmd_map)
{
	char lRecvBuff[4];
	GPIAMessage lRecvMsg;
	Device& lDevice = this->mDevices[aCurrentDeviceIndex];
	std::set<int>::iterator lPortIter = lDevice.mDefinedPort.begin();
	for ( ; lPortIter != lDevice.mDefinedPort.end(); lPortIter++ )
    {
        int lPort = *lPortIter;
		const char* lCommand = aSendCmd_map[lPort].getMessageBuffer();
		if(aSendCmdResult_map[lPort]){
			unsigned int lRecvLength = 0;
			if ( lDevice.mReadSocket.receive( lRecvBuff, GPIAMessage::getBufferSize(), 0, &lRecvLength ) )
			{
				lRecvMsg.setContent( lRecvBuff, GPIAMessage::getBufferSize() );
#if defined( _DEBUG )
				lDevice.mlog << "<<recvAll fun>>Device = " << lDevice.mHostName
						<< ", IP = " << lDevice.mReadSocket.IPToString( lDevice.mIPAddress )
						<< ", Command = " << std::hex;
				for ( int lIndex = 0; lIndex < GPIAMessage::getBufferSize(); lIndex++ )
				{
					lDevice.mlog << (unsigned int)(unsigned char)lCommand[ lIndex ] << " ";
				}
				lDevice.mlog << ", Result = ";
				for ( int lIndex = 0; lIndex < GPIAMessage::getBufferSize(); lIndex++ )
				{
					lDevice.mlog << (unsigned int)(unsigned char)lRecvBuff[ lIndex ] << " ";
				}
				
				if ( lRecvMsg.getStatus() != GPIAMessage::Normal || lRecvMsg.getPortNumber() != lPort)
				{
					lDevice.mlog << "***************************ERROR!!!!!";
				}
				lDevice.mlog << std::endl;
#endif
				lDevice.mReadMutex.lock();
				lDevice.mPortCache[ lPort ].first = lRecvMsg.getData();
				//only if the status is normal, set the validity to true
				lDevice.mPortCache[ lPort ].second = ( lRecvMsg.getStatus() == GPIAMessage::Normal && lRecvMsg.getPortNumber() == lPort);
				lDevice.mReadMutex.unlock();

				lDevice.mRecordMutex.lock();
				if ( lDevice.mIsRecording )
				{
					if ( !saveData( aCurrentDeviceIndex, lPort, lRecvMsg.getData() ) )
					{
						lDevice.mIsRecording = false;
						lDevice.mRecordingStream.close();
					}
				}
				lDevice.mRecordMutex.unlock();
			}
			else
			{
				lDevice.mReadMutex.lock();
				//if no response from GPIA, set the validity of the data to false
				lDevice.mPortCache[ lPort ].second = false;
				lDevice.mReadMutex.unlock();

#if defined( _DEBUG )
					lDevice.mlog << "<<recvAll FUN>>Failed to receive reponse from GPIA. Device = " << lDevice.mHostName
						<< ", IP = " << lDevice.mReadSocket.IPToString( lDevice.mIPAddress )
						<< ", Command = " << std::hex;
					for ( int lIndex = 0; lIndex < GPIAMessage::getBufferSize(); lIndex++ )
					{
						lDevice.mlog << (unsigned int)(unsigned char)lCommand[ lIndex ] << " ";
					}
					lDevice.mlog << std::endl;
#endif
			}
		}
		else
		{
			lDevice.mReadMutex.lock();
            //if no response from GPIA, set the validity of the data to false
            lDevice.mPortCache[ lPort ].second = false;
            lDevice.mReadMutex.unlock();
		}
	}
}
void DiscreteGPIADeviceLibrary::readData( int aDeviceIndex )
{
	std::map<int, bool> lSendCmdResult_map;
	std::map<int, GPIAMessage> lSendCmd_map;

#if defined( _DEBUG )
	mDevices[ aDeviceIndex ].mlog.open( mDevices[ aDeviceIndex ].mReadSocket.
		IPToString( mDevices[ aDeviceIndex ].mIPAddress ) + "_readDatalog.txt" , std::ios::app);
#endif

    while ( mIsOpen )
    {
        if ( mDevices[ aDeviceIndex ].mIsActivated )
        {
			sendAllCmds(mDevices[ aDeviceIndex ], lSendCmdResult_map, lSendCmd_map);

			Sleep( fLoopPeriod );

			recvAll(aDeviceIndex, lSendCmdResult_map, lSendCmd_map);
		}
    }

#if defined( _DEBUG )
	mDevices[ aDeviceIndex ].mlog.flush();
	mDevices[ aDeviceIndex ].mlog.close();
#endif
    return;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// saveData
//
bool DiscreteGPIADeviceLibrary::saveData( int aDeviceIndex, unsigned int aPortNumber, unsigned char aData )
{
    bool lResult = true;
    const char lComma = ',';
    const char lColon = ':';

    SYSTEMTIME lTimestamp;
    GetLocalTime( &lTimestamp );

    try
    {
        unsigned int lData = aData;
        std::stringstream lTimeString;
        lTimeString << lTimestamp.wHour << lColon << lTimestamp.wMinute << lColon
                    << lTimestamp.wSecond << lColon << lTimestamp.wMilliseconds;
        mDevices[ aDeviceIndex ].mRecordingStream
            << std::dec << std::left << std::setw( 32 ) << lTimeString.str() << lComma//output timestamp
            << std::setw( 16 ) << aPortNumber << lComma //output channel id
            << std::setw( 16 ) << std::hex << std::showbase << lData << std::endl;
    }
    catch( std::ios_base::failure& )
    {
        lResult = false;
    }
    return lResult;
}

DiscreteGPIADeviceLibrary::Device::Device()
    :mIsActivated( false ),
    mPartNumber( "" ),
    mHostName( "" ),
    mIPAddress( 0 ),
    mPortCount( 0 ),
    mIsRecording( false )
{
    return;
}

DiscreteGPIADeviceLibrary::Device::Device( const Device& aDevice )
    :mIsActivated( aDevice.mIsActivated ),
    mPartNumber( aDevice.mPartNumber ),
    mHostName( aDevice.mHostName ),
    mIPAddress( aDevice.mIPAddress ),
    mPortCount( aDevice.mPortCount ),
    mIsRecording( aDevice.mIsRecording ),
    mBitGroups( aDevice.mBitGroups ),
    mDefinedPort( aDevice.mDefinedPort ),
    mPortCache( aDevice.mPortCache ),
    mMsgSocket(),
    mRecordingStream(),
	mReadSocket(),
	mThread(aDevice.mThread),
	count(0)
{
    return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// createDevice
//
extern "C"  IDiscreteDevice* createDevice( void )
{
    return new DiscreteGPIADeviceLibrary();
}

extern "C" void destroyDevice( IDiscreteDevice* aDevice )
{
    if ( aDevice != NULL )
    {
        delete ( DiscreteGPIADeviceLibrary* )( aDevice );
    }
}