#if !defined( DISCRETEGPIADEVICELIBRARY_H )
#define DISCRETEGPIADEVICELIBRARY_H
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
#include <fstream>
#include <set>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
  
#if !defined ( IOA_IDISCRETEDEVICE_H )
    #include "IOA/IDiscreteDevice.h"
#endif

#if !defined ( OPERATINGSYSTEM_WSAUDPSOCKET_H )
    #include "OperatingSystem/WSAUDPSocket.h"
#endif

#if !defined ( GPIAMESSAGE_H )
    #include "GPIAMessage.h"
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
// DiscreteGPIADeviceLibrary
//    This class manages Discrete GPIA Devices, implemented the IDiscreteDevice interface.
//    GPIA ( General Purpose Interface Assmbly ) is designed for providing a general interface for
//    discrete signals. GPIA devices will connect to a local network with their ethernet interface.
//    This class will detect all of the GPIA devices that currently connect to the local network
//    when startup. After that, the user can privide the host names of the needed devices to
//    determine which devices will be used, then define specified discrete signals on the device.
//    Call read()/write() functions after call open() to read/write a defined discrete signal.
//    After all operations completed, call close() to close all GPIA devices.
//
class DiscreteGPIADeviceLibrary : public IDiscreteDevice
{
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Type Definitions
    //
    typedef DiscreteGPIADeviceLibrary MyType;
    typedef IDiscreteDevice InheritedType;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Default Constructor
    //
    DiscreteGPIADeviceLibrary( void );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Destructor
    //
    virtual ~DiscreteGPIADeviceLibrary( void );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // defineDevice
    //   Defines a GPIA device to associate a device ID with. The first parameter aID is not used
    //   in this implementation. Fill aConfig with host name of the GPIA device to identify
    //   different devices.
    //
    //   Parameters:
    //     aID -  Not used in this implementation, could be any value.
    //     aConfig - The host name of GPIA device.
    //     aDeviceID - The resulting device ID, to be used in further function calls.
    //
    //   Return Values:
    //     ResultSuccess - Successfully defined the device.
    //     ResultInvalidParameter - One-or-more parameters is an invalid value.
    //     ResultNotAvailable - The device id / name was not found.
    //     ResultInvalidOperation - The devices associated with the class are currently open,
    //                              and no more information can be defined.
    Result IOACall defineDevice( IOA::UInt32 aID, const IOA::Char* aConfig,
                                         DeviceID* aDeviceID );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // defineBit
    //   Defines a bit on the given GPIA device.
    //
    //   Parameters:
    //     aDeviceID - ID of the the device to define the bit on
    //     aPortNumber - The port number on the device that the bit is on
    //     aBitPosition - The numeric representing the position of the bit within the port.
    //     aDirection - The direction of the discrete bit (transmit/receive).
    //     aBitID - The resulting bit ID, to be used in further function calls.
    //
    //   Return Values:
    //     ResultSuccess - Successfully defined the receive port.
    //     ResultInvalidParameter - One-or-more parameters is an invalid value.
    //     ResultNotSupported - One-or-more parameters specified a capability that the driver does
    //                          not support.
    //     ResultInvalidOperation - The devices associated with the class are currently open,
    //                              and no more information can be defined.
    //
    Result IOACall defineBit( DeviceID aDeviceID, IOA::UInt32 aPortNumber,
                                     IOA::UInt32 aBitPosition, Direction aDirection,
                                     BitID* aBitID );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // open
    //   Opens the GPIA devices that have been defined. Note that while the devices are open,
    //  additional devices and associated elements cannot be defined.
    //
    //   Return Values:
    //     ResultSuccess - Successfully opened all devices.
    //     ResultNotAvailable - Could not open one-or-more devices.
    //
    Result IOACall open( void );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // close
    //   Closes the devices, and ceases all communication.
    //
    //     ResultSuccess - Successfully closed all devices.
    //     ResultNotAvailable - Could not close one-or-more devices.
    //
    Result IOACall close( void );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // read
    //   Initiate a read of data from the device.
    //
    //   Parameters:
    //     aBitId - id used to identify the caller of this function
    //     aData - pointer to boolean to copy the discrete value into
    //
    //   Return Values:
    //     ResultSuccess - Successfully opened all devices.
    //     ResultInvalidParameter - One-or-more parameters is an invalid value.
    //     ResultInvalidOperation - The devices associated with the class are not open, and cannot
    //                              be read.
    //
    Result IOACall read( BitID aBitId, IOA::Bool* aData );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // write
    //   Initiate a write of the data specified to the device.
    //
    //   Parameters:
    //     aBitId - id used to identify the caller of this function
    //     aData - pointer to boolean value of the bit to be written
    //
    //   Return Values:
    //     ResultSuccess - Successfully opened all devices.
    //     ResultInvalidParameter - One-or-more parameters is an invalid value.
    //     ResultInvalidOperation - The devices associated with the class are not open, and cannot
    //                              be read.
    //
    Result IOACall write( BitID aBitId, const IOA::Bool aData );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // getLastError
    //   Gets the last error message created by the Discrete device driver.
    //
    //   Parameters:
    //     aBuffer - Buffer to copy the message into.
    //     aBufferLength - The length of the buffer, in bytes.
    //
    //    Return Values:
    //     ResultSuccess - Successfully enabled port data.
    //     ResultInvalidParameter - One-or-more parameters is an invalid value.
    Result IOACall getLastError( IOA::Char* aBuffer, IOA::UInt32 aBufferLength );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // getRuntimeFeatureStatus
    //   Gets the status of the specified feature.
    //
    //   Parameters:
    //     aFeature - The feature to check.
    //
    //   Return Values:
    //     true - The feature is supported by this driver.
    //     false - The feauture is not supported by this driver.
    //
    IOA::Bool IOACall getFeatureStatus( RuntimeFeature aFeature );

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
    Result IOACall startTraceLog( DeviceID aDeviceID, const IOA::Char* aFileName );

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
   Result IOACall stopTraceLog( DeviceID aDeviceID );

protected:

private:
    struct Device
    {
        OperatingSystem::WSAUDPSocket mMsgSocket;
		OperatingSystem::WSAUDPSocket mReadSocket;
		std::tr1::shared_ptr<boost::thread> mThread;

        std::string mPartNumber;
        std::string mHostName;
        unsigned long mIPAddress;
        unsigned int mPortCount;
        bool mIsActivated;
        std::set< int > mDefinedPort;
        std::vector< std::pair< unsigned char/*Data*/, bool/*Validity*/ > > mPortCache;
        std::vector< int > mBitGroups;
        std::ofstream mRecordingStream;
        bool mIsRecording;

        boost::mutex mSocketMutex;
		boost::mutex mReadMutex;
        boost::mutex mRecordMutex;

        Device();
        Device( const Device& );

		unsigned int count;
		std::ofstream mlog;
    };

    struct BitGroup
    {
        unsigned char mBitMask;
        int mPortNumber;
        Direction mDirection;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Copy constructor; private so it can not be called
    //
    DiscreteGPIADeviceLibrary( const MyType& aOther ){}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Assignment operator; private so it can not be called
    //
    DiscreteGPIADeviceLibrary& operator=( const MyType& aOther ){}

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // defineBitGroup
    //   Defines a BitGroup on the given GPIA device.
    //
    //   Parameters:
    //     aDeviceID - ID of the the device to define the bit on
    //     aPortNumber - The port number on the device that the bit is on
    //     aBitMask - The mask indicates which bits are available within the port.
    //     aDirection - The direction of the discrete BitGroup (transmit/receive).
    //     aBitID - The resulting bit ID, to be used in further function calls.
    //
    //   Return Values:
    //     ResultSuccess - Successfully defined the receive port.
    //     ResultInvalidParameter - One-or-more parameters is an invalid value.
    //     ResultNotSupported - One-or-more parameters specified a capability that the driver does
    //                          not support.
    //     ResultInvalidOperation - The devices associated with the class are currently open,
    //                              and no more information can be defined.
    //
    Result IOACall defineBitGroup( DeviceID aDeviceID, IOA::UInt32 aPortNumber,
                                     IOA::UInt8 aBitMask, Direction aDirection,
                                     BitID* aBitID );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // readBitGroup
    //   Initiate a read of BitGroup data from the device.
    //
    //   Parameters:
    //     aBitId - id used to identify the caller of this function
    //     aData - pointer to UInt8 to copy the bit group value into
    //
    //   Return Values:
    //     ResultSuccess - Successfully opened all devices.
    //     ResultInvalidParameter - One-or-more parameters is an invalid value.
    //     ResultInvalidOperation - The devices associated with the class are not open, and cannot
    //                              be read.
    //
    Result IOACall readBitGroup( BitID aBitId, IOA::UInt8* aData );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // writeBitGroup
    //   Initiate a write of BitGroup data specified to the device.
    //
    //   Parameters:
    //     aBitId - id used to identify the caller of this function
    //     aData - value of the bit group to be written
    //
    //   Return Values:
    //     ResultSuccess - Successfully opened all devices.
    //     ResultInvalidParameter - One-or-more parameters is an invalid value.
    //     ResultInvalidOperation - The devices associated with the class are not open, and cannot
    //                              be read.
    //
    Result IOACall writeBitGroup( BitID aBitId, const IOA::UInt8 aData );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  detectDevice
    //      detect connected GPIA devices and add them to internal device map
    //
    int detectDevice( std::string aIPString );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  accessGPIA
    //      send command message to the associated GPIA device and read the result message back.
    //
    //      Parameters:
    //          aDevice - a reference to the device object that corresponding to the GPIA
    //          aCmdMsg - a GPIAMessage object that stores the command information.
    //          aResultMsg - a GPIAMessage object to be written result information in.
    //
    //      Return values:
    //          true - access successfully.
    //          false - failed to access the GPIA device.
    //
    bool accessGPIA( Device& aDevice, const GPIAMessage& aCmdMsg, GPIAMessage& aResultMsg );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // readData
    //      Thread function, used to send read data from all GPIAs periodically
    void readData( int aDeviceIndex );


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // saveData
    //
    bool saveData( int aDeviceIndex, unsigned int aPortNumber, unsigned char aData );


	void sendAllCmds(Device& aDevice, std::map<int, bool>& aSendCmdResult_map, std::map<int, GPIAMessage>& aSendCmd_map);

	void recvAll(int aCurrentDeviceIndex, std::map<int, bool>& aSendCmdResult_map, std::map<int, GPIAMessage>& aSendCmd_map);


    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Member Variables
    //

    IOA::Bool mIsOpen;

    std::string mLastError;

    OperatingSystem::WSAUDPSocket mUDPSocket;

    int mCurrentDeviceIndex;
    int mCurrentBitIndex;

    std::vector<Device> mDevices;
    std::vector<BitGroup> mBitGroups;

    //std::map<std::string, int> mDeviceMap;
    std::map<unsigned long, int> mDeviceMap;
    std::map<int/*bit group index*/, int/*device index*/> mBitGroupMap;

   // std::tr1::shared_ptr<boost::thread> mThread;

#if defined( _DEBUG )
    std::ofstream mErrorLogFile;
#endif
};

#if defined DISCRETEGPIADEVICELIBRARY_FACTORY
extern "C" __declspec(dllexport) IDiscreteDevice* createDevice( void );
extern "C" __declspec(dllexport) void destroyDevice( IDiscreteDevice* aDevice );
#endif

#endif