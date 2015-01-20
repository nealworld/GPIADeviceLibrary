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
#include <memory>
#if !defined ( GPIAMESSAGE_H )
#include "GPIAMessage.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
//  const variables

//size of internal buffer. Used for appling memory for internal buffer.
const unsigned int GPIAMessage::cBufferSize = 4;

//indice of different fields in internal buffer.
static const int fPortNumberIndex = 0;
static const int fCodeIndex = 1;
static const int fBitMaskIndex = 2;
static const int fDataIndex = 3;

////////////////////////////////////////////////////////////////////////////////////////////////
//  default constructor
//
GPIAMessage::GPIAMessage(void)
{
    mMessageContent = new char[cBufferSize];
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  destructor
//
GPIAMessage::~GPIAMessage(void)
{
    delete[] mMessageContent;
    mMessageContent = NULL;
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  setPortNumber
//
void GPIAMessage::setPortNumber( unsigned char aPortNumber )
{
    mMessageContent[fPortNumberIndex] = aPortNumber;
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  setCode
//
void GPIAMessage::setCode( OperationCode aCode )
{
    mMessageContent[fCodeIndex] = aCode;
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  setBitMask
//
void GPIAMessage::setBitMask( unsigned char aBitMask )
{
    mMessageContent[fBitMaskIndex] = aBitMask;
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  setData
//
void GPIAMessage::setData( unsigned char aData )
{
    mMessageContent[fDataIndex] = aData;
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  getMessageBufer
//
const char* GPIAMessage::getMessageBuffer( void ) const
{
    return mMessageContent;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  getBufferSize
//
unsigned int GPIAMessage::getBufferSize( void )
{
    return cBufferSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  setContent
//      if the given buffer length doesn't matches size of internal buffer, returns false
//
bool GPIAMessage::setContent( const char* aBuffer, unsigned int aBuffLength )
{
    bool lResult = false;
    if ( aBuffLength == cBufferSize )
    {
        std::memcpy( mMessageContent, aBuffer, aBuffLength );
        lResult = true;
    }
    return lResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  getPortNumber
//      returns the port number filed of the message.
//
//      Return values:
//          port number.
//
unsigned char GPIAMessage::getPortNumber( void )
{
    return mMessageContent[fPortNumberIndex];
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  getStatus
//      returns the status code of result message.
//
//      Return values:
//          status code.
//
GPIAMessage::StatusCode GPIAMessage::getStatus( void )
{
    StatusCode lCode = InvalidCode;
    switch ( mMessageContent[fCodeIndex] )
    {
    case Normal:
        lCode = Normal;
        break;
    case ReadFailed:
        lCode = ReadFailed;
        break;
    case WriteFailed:
        lCode = WriteFailed;
        break;
    default:
        break;
    }
    return lCode;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//  getData
//      returns the data of the message.
//
//      Return values:
//          data.
//
unsigned char GPIAMessage::getData( void )
{
    return mMessageContent[fDataIndex];
}