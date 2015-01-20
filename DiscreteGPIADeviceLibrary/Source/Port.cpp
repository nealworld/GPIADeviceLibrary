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
#if !defined ( PORT_H )
    #include "Port.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
//  constructor
//
Port::Port( IOA::UInt32 aPortNumber )
    :mPortNumber( aPortNumber )
{
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  destructor
//      release all BitGroup objects associated with this port.
//
Port::~Port( void )
{
    for ( BitGroupMap::iterator lIter = mBitGroups.begin(); lIter != mBitGroups.end(); lIter++ )
    {
        if ( lIter->second != NULL )
        {
            delete lIter->second;
            lIter->second = NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  getPortNumber
//
IOA::UInt32 Port::getPortNumber( void ) const
{
    return mPortNumber;
}

////////////////////////////////////////////////////////////////////////////////////////////
//  fillMessage
//
bool Port::fillMessage( IDiscreteDevice::BitID aBitID, GPIAMessage& aMessage )
{
    bool lResult = false;
    if ( mBitGroups.find( aBitID ) != mBitGroups.end() )
    {
        aMessage.setPortNumber( mPortNumber );
        mBitGroups[aBitID]->fillMessage( aMessage );
        lResult = true;
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  addBitGroup
//      add one BitGroup into the BitGroup map of this port.
//
void Port::addBitGroup( IOA::UInt8 aBitMask, IDiscreteDevice::BitID* aBitID )
{
    BitGroup* lNewBitGroup = new BitGroup( aBitMask );
    *aBitID = lNewBitGroup;
    mBitGroups[*aBitID] = lNewBitGroup;

    return;
}