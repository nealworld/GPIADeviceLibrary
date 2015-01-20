#if !defined ( BITGROUP_H)
#define BITGROUP_H
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
#if !defined ( IOA_TYPES_H )
    #include "IOA/Types.h"
#endif

#if !defined ( GPIAMESSAGE_H )
    #include "GPIAMessage.h"
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
//  BitGroup
//      This class represents one BitGroup of a port.
class BitGroup
{
public:
    ////////////////////////////////////////////////////////////////////////////////////////////
    //  constructor
    //
    //      Parameters:
    //          aBitMask - Mask of bits, the needed position should be set to 1.
    BitGroup( IOA::UInt32 aBitMask );

    ////////////////////////////////////////////////////////////////////////////////////////////
    //destructor
    //
    virtual ~BitGroup( void );

    ////////////////////////////////////////////////////////////////////////////////////////////
    //  fillMessage
    //      fill a GPIAMessage structure with information of this bit group.
    //
    //      Parameters:
    //          aMessage - a GPIAMessage structure to be filled.
    //
    void fillMessage( GPIAMessage& aMessage );

private:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Default constructor; private so it can not be called
    //
    BitGroup( void ){}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Copy constructor; private so it can not be called
    //
    BitGroup( const BitGroup& ){}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Assignment operator; private so it can not be called
    //
    BitGroup& operator=( const BitGroup& ){}

    IOA::UInt8 mBitMask;
};

#endif