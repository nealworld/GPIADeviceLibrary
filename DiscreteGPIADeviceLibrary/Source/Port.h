#if !defined ( PORT_H )
#define PORT_H
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
#include <map>

#if !defined ( IO_TYPES_H )
    #include "IOA/Types.h"
#endif

#if !defined ( IO_IDISCRETEDEVICE_H )
    #include "IOA/IDiscreteDevice.h"
#endif

#if !defined ( BITGROUP_H )
    #include "BitGroup.h"
#endif

#if !defined ( GPIAMESSAGE_H )
    #include "GPIAMessage.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
//  Port
//      This class represents a register port on a GPIA discrete device.
class Port
{
public:
    typedef std::map< IDiscreteDevice::BitID, BitGroup* > BitGroupMap;

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  constructor
    //
    //      parameters:
    //          aNumber - number of this port.
    Port( IOA::UInt32 aPortNumber );

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  destructor
    //
    virtual ~Port( void );

    ////////////////////////////////////////////////////////////////////////////////////////////
    // getPortNumber
    //      returns the port number of this port.
    //
    //      Return values:
    //          the port number of this port.
    //
    IOA::UInt32 getPortNumber( void ) const;

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  fillMessage
    //      fill a GPIAMessage structure with information of this port and given bit.
    //
    //      Parameters:
    //          aBitID - ID of the bit to fill the message.
    //          aMessage - a GPIAMessage structure to be filled.
    //
    //      Return values:
    //          true - filled message successfully.
    //          false - bit ID not found.
    //
    bool fillMessage( IDiscreteDevice::BitID aBitID, GPIAMessage& aMessage );

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  addBitGroup
    //      add one bit into the bit map of this port.
    //
    //      Parameters:
    //          aBitMask - bit mask of the BitGroup.
    //          aBitID - output parameter. Handle of the BitGroup to be added.
    //
    void addBitGroup( IOA::UInt8 aBitMask, IDiscreteDevice::BitID* aBitID );

private:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Default constructor; private so it can not be called
    //
    Port( void ){}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Copy constructor; private so it can not be called
    //
    Port( const Port& ){}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Assignment Operator; private so it can not be called
    //
    Port& operator=( const Port& ){}

    IOA::UInt32 mPortNumber;
    BitGroupMap mBitGroups;
};

#endif