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

#if !defined ( DEVICE_H )
    #include "Device.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////
//  file scope variables

// the port number used by GPIA controller to send UDP message.
static const USHORT fMsgPort = 8412;

//time out value for receive message. Unit - ms.
static const int fTimeOut = 100;

////////////////////////////////////////////////////////////////////////////////////////////////////
// functions

////////////////////////////////////////////////////////////////////////////////////////////////
// constructor
//
Device::Device( const std::string& aHostName, unsigned long aIPAddress,
            unsigned int aPortCount )
    :mIsActivated( false ),
    mIsOpened( false ),
    mHostName( aHostName ),
    mIPAddress( aIPAddress ),
    mPortCount( aPortCount )
{
    return;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//  destructor
//      release port objects associated with this device.
Device::~Device( void )
{
    for ( PortMap::iterator lIter = mPorts.begin(); lIter != mPorts.end(); lIter++ )
    {
        if ( lIter->second != NULL )
        {
            delete lIter->second;
            lIter->second = NULL;
        }
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  activate
//
void Device::activate( void )
{
    mIsActivated = true;
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  openDevice
//      create an UDP socket, set status to open.
//
bool Device::openDevice( void )
{
    bool lResult = false;

    if ( mIsOpened == false )
    {
        if ( mMsgSocket.open() )
        {
            mMsgSocket.config( false, true );
            mIsOpened = true;
            lResult = true;
        }
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  closeDevice
//      close the UDP socket, set status to close.
//
bool Device::closeDevice( void )
{
    bool lResult = false;

    if ( mIsOpened == true )
    {
        if ( mMsgSocket.close() )
        {
            mIsOpened = false;
            lResult = true;
        }
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  read
//      fill a command message, send it and parse the result message to return read data.
//
bool Device::read( DiscreteGPIADeviceLibrary::PortID aPortID,
                    IDiscreteDevice::BitID aBitID, IOA::UInt8& aData )
{
    bool lResult = false;
    if ( mIsOpened )
    {
        //check if the port ID is valid
        if ( mPorts.find( aPortID ) != mPorts.end() )
        {
            GPIAMessage lCmdMsg;
            GPIAMessage lResultMsg;

            if ( mPorts[aPortID]->fillMessage( aBitID, lCmdMsg ) )
            {
                lCmdMsg.setData( aData );
                lCmdMsg.setCode( GPIAMessage::Read );

                //send command message and get result back
                if ( accessGPIA( lCmdMsg, lResultMsg ) )
                {
                    //check result
                    if ( ( lCmdMsg.getPortNumber() == lResultMsg.getPortNumber() )
                        && ( lResultMsg.getStatus() == GPIAMessage::Normal ) )
                    {
                        aData = lResultMsg.getData();
                        lResult = true;
                    }
                }
            }
        }
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  write
//      fill a command message, send it and parse the result message to return write result.
//
bool Device::write( DiscreteGPIADeviceLibrary::PortID aPortID,
                    IDiscreteDevice::BitID aBitID, IOA::UInt8 aData )
{
    bool lResult = false;
    if ( mIsOpened )
    {
        //check if the port ID is valid
        if ( mPorts.find( aPortID ) != mPorts.end() )
        {
            GPIAMessage lCmdMsg;
            GPIAMessage lResultMsg;
            if ( mPorts[aPortID]->fillMessage( aBitID, lCmdMsg ) )
            {
                lCmdMsg.setData( aData );
                lCmdMsg.setCode( GPIAMessage::Write );

                //send command message and get result back
                if ( accessGPIA( lCmdMsg, lResultMsg ) )
                {
                    //check result
                    if ( ( lCmdMsg.getPortNumber() == lResultMsg.getPortNumber() )
                            && ( lResultMsg.getStatus() == GPIAMessage::Normal ) )
                    {
                        lResult = true;
                    }
                }
            }
        }
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  addBitGroup
//      add a bitGroup in the given port.
//
void Device::addBitGroup( IOA::UInt32 aPortNumber,
                    IOA::UInt8 aBitMask,
                    IDiscreteDevice::BitID* aBitID,
                    DiscreteGPIADeviceLibrary::PortID* aPortID )
{
    //get the portID of a port that matches given port number
    DiscreteGPIADeviceLibrary::PortID lPortID = getPort( aPortNumber );

    IDiscreteDevice::BitID lBitID = NULL;

    //add the bit with given bit position in the port
    mPorts[lPortID]->addBitGroup( aBitMask, &lBitID );

    *aBitID = lBitID;
    *aPortID = lPortID;

    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// getPort
//  return the port ID that matches the given port number. If port has not been defined, create it
//  and return its ID.
//
DiscreteGPIADeviceLibrary::PortID Device::getPort( IOA::UInt32 aPortNumber )
{
    DiscreteGPIADeviceLibrary::PortID lPortID = NULL;
    bool lIsPortDefined = false;

    //search port map to see if this port number has beel defined already
    for ( PortMap::iterator lIter = mPorts.begin();
          ( lIter != mPorts.end() ) && ( !lIsPortDefined ); lIter++ )
    {
        //if port already defined, return its ID
        if ( lIter->second->getPortNumber() == aPortNumber )
        {
            lPortID = lIter->first;
            lIsPortDefined = true;
        }
    }
    //if port not defined, create one and return its ID
    if ( !lIsPortDefined )
    {
        Port* lNewPort = new Port( aPortNumber );
        lPortID = lNewPort;
        mPorts[lPortID] = lNewPort;
    }
    return lPortID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  accessGPIA
//      send command message to the GPIA device, and then read result message back .
//
bool Device::accessGPIA( const GPIAMessage& aCmdMsg, GPIAMessage& aResultMsg )
{
    bool lResult = false;

    char* lRecvBuff = new char[GPIAMessage::getBufferSize()];

    //send command to GPIA
    if ( mMsgSocket.sendTo( aCmdMsg.getMessageBuffer(), GPIAMessage::getBufferSize(),
                            mIPAddress, fMsgPort ) )
    {
        unsigned int lRecvLength = 0;

        //receive result message from GPIA
        if ( mMsgSocket.receive( lRecvBuff, GPIAMessage::getBufferSize(), fTimeOut, &lRecvLength ) )
        {
            aResultMsg.setContent( lRecvBuff, GPIAMessage::getBufferSize() );
            delete[] lRecvBuff;
            lRecvBuff = NULL;
            lResult = true;
        }
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  getHostName
//      returns the host name of this GPIA device.
//
//      Return values:
//          The host name of this GPIA Device.
//
std::string Device::getHostName( void ) const
{
    return mHostName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  getIPAddressStr
//
std::string Device::getIPAddressStr( void ) const
{
    return OperatingSystem::WSAUDPSocket::IPToString( mMsgSocket.getClientIPAddr() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  isActivated
//
bool Device::isActivated( void ) const
{
    return mIsActivated;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  getPortCount
//
unsigned int Device::getPortCount( void ) const
{
    return mPortCount;
}