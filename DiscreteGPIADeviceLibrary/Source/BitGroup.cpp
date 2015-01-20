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

#if !defined ( BITGROUP_H )
    #include "BitGroup.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// constructor
//
BitGroup::BitGroup( IOA::UInt32 aBitMask )
    :mBitMask( aBitMask )
{
    return;
}
////////////////////////////////////////////////////////////////////////////////////////////
// destructor
//
BitGroup::~BitGroup( void )
{
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////
//  fillMessage
//      set the bit mask property of GPIAMessage with the bit mask of this bit group.
//
void BitGroup::fillMessage( GPIAMessage& aMessage )
{
    aMessage.setBitMask( mBitMask );
    return;
}