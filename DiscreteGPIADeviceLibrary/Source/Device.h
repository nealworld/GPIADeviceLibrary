#if !defined ( DEVICE_H )
#define DEVICE_H
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
#include <string>
#include <map>

#if !defined ( IOA_TYPES_H )
    #include "IOA/Types.h"
#endif

#if !defined( DISCRETEGPIADEVICELIBRARY_H )
    #include "DiscreteGPIADeviceLibrary.h"
#endif

#if !defined ( PORT_H )
    #include "Port.h"
#endif

#if !defined ( GPIAMESSAGE_H )
    #include "GPIAMessage.h"
#endif

#if !defined ( OPERATINGSYSTEM_WSAUDPSOCKET_H )
    #include "OperatingSystem/WSAUDPSocket.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
//  Device
//      One instance of this class manages one GPIA controller.
class Device
{
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Type Definitions
    typedef std::map< DiscreteGPIADeviceLibrary::PortID, Port* > PortMap;

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  constructor
    //
    //      Parameters:
    //          aHostName - host name of the GPIA controller.
    //          aIPAddress - IP address of the GPIA controller.
    //          aPortCount - count of ports on the device.
    //
    Device( const std::string& aHostName, unsigned long aIPAddress, unsigned int aPortCount );

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  destructor
    //
    ~Device( void );

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  openDevice
    //      opens this GPIA device.
    //
    //  Return values:
    //      true - successfully opened the device.
    //      false - failed to open the device.
    //
    bool openDevice( void );

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  closeDevice
    //      closes this GPIA device.
    //
    //  Return values:
    //      true - successfully closeed the device.
    //      false - failed to close the device.
    //
    bool closeDevice( void );

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  read
    //      read data from the given Bit.
    //
    //  Parameters:
    //      aPortID - ID of the port that the Bit to be read belongs to.
    //      aBitID - ID of the bit that to be read.
    //      aData - (output) read data.
    //
    //  Return values:
    //      true - read successfully.
    //      false - failed to read.
    //
    bool read( DiscreteGPIADeviceLibrary::PortID aPortID, IDiscreteDevice::BitID aBitID,
                IOA::UInt8& aData );

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  write
    //      write data to the given bit.
    //
    //  Parameters:
    //      aPortID - ID of the port that the  Bit to be written belongs to.
    //      aBitID - ID of the bit that to be written.
    //      aData - value of the data to be written.
    //
    //  Return values:
    //      true - write successfully.
    //      false - failed to write.
    //
    bool write( DiscreteGPIADeviceLibrary::PortID aPortID, IDiscreteDevice::BitID aBitID,
                IOA::UInt8 aData );

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  addBitGroup
    //      add a BitGroup in this device.
    //
    //  Parameters:
    //      aPortNumber - port number.
    //      aBitMask - bit mask of the BitGroup.
    //      aBitID - (output)the handle of the added BitGroup.
    //      aPortID - (output)handle of the port that the new added BitGroup belongs to.
    //
    void addBitGroup( IOA::UInt32 aPortNumber, IOA::UInt8 aBitMask,
                 IDiscreteDevice::BitID* aBitID, DiscreteGPIADeviceLibrary::PortID* aPortID );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  getPort
    //      get the portID of a port that matches the given port number. If no matches found, this
    //      function will create a port with the given port number and returns its ID.
    //
    //      Parameters:
    //          aPortNumber - number of the port to be found.
    //
    //      Return values:
    //          A valid PortID associated with the given port number.
    //
    DiscreteGPIADeviceLibrary::PortID getPort( IOA::UInt32 aPortNumber );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  getHostName
    //      returns the host name of this GPIA device.
    //
    //      Return values:
    //          The host name of this GPIA Device.
    //
    std::string getHostName( void ) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  getIPAddressStr
    //      returns the stand string representation of IP address of this GPIA device.
    //
    //      Return values:
    //          The stand string representation of IP address of this GPIA device.
    //
    std::string getIPAddressStr( void ) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  getPortCount
    //      returns the number of ports available on this device.
    //
    //      Return values:
    //          number of ports.
    //
    unsigned int getPortCount( void ) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  isActivated
    //      indicates whether this GPIA device has already activated for use.
    //
    //      Return values:
    //          true - this device is activated.
    //          false - this device is not activated.
    //
    bool isActivated( void ) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  activate
    //      mark this device as activated.
    //
    void activate( void );

private:
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Default constructor: private so it can not be called
    //
    Device( void ){}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Copy constructor; private so it can not be called
    //
    Device( const Device&){}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Assignment operator; private so it can not be called
    //
    Device& operator=( const Device& ){}

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  accessGPIA
    //      send command message to the associated GPIA device and read the result message back.
    //
    //      Parameters:
    //          aCmdMsg - a GPIAMessage object that stores the command information.
    //          aResultMsg - a GPIAMessage object to be written result information in.
    //
    //      Return values:
    //          true - access successfully.
    //          false - failed to access the GPIA device.
    //
    bool accessGPIA( const GPIAMessage& aCmdMsg, GPIAMessage& aResultMsg );

    bool mIsOpened;
    bool mIsActivated;

    std::string mHostName;
    unsigned long mIPAddress;
    unsigned int mPortCount;

    PortMap mPorts;

    OperatingSystem::WSAUDPSocket mMsgSocket;
};

#endif