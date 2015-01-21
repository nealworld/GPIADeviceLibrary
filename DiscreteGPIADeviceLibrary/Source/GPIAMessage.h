#if !defined ( GPIAMESSAGE_H )
#define GPIAMESSAGE_H
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//  GPIAMessage
//      this class maintains a message structure with a specified format used to communicate with
//      GPIA device. Currently the internal representation of a message contains 4 bytes:
//      index 0 - number of the port to be accessed.
//      index 1 - operate code( for command message )/ status code( for result message )
//      index 2 - bit mask( position of bits that to be operated will be set to 1 )
//      index 3 - input data( for command message )/ output data( for result message )
//
class GPIAMessage
{
public:
    //operation code definition for command message
    enum OperationCode
    {
        Read = 1,
        Write
    };

    //status code definition for result message
    enum StatusCode
    {
        Normal = 0,
        ReadFailed,
        WriteFailed,
        InvalidCode = 0xFF
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  constructor
    //
    GPIAMessage(void);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  destructor
    //
    ~GPIAMessage(void);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  setPortNumber
    //      set the PortNumber field of the message content with given port number.
    //
    //      Parameters:
    //          aPortNumber - port number to be set
    //
    void setPortNumber( unsigned char aPortNumber );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  setCode
    //      set the Code field of the message content with given code.
    //
    //      Parameters:
    //          aCode - code to be set
    //
    void setCode( OperationCode aCode );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  setBitMask
    //      set the BitMask field of the message content with given BitMask.
    //
    //      Parameters:
    //          aBitMask - BitMask to be set
    //
    void setBitMask( unsigned char aBitMask );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  setData
    //      set the Data field of the message content with given Data.
    //
    //      Parameters:
    //          aData - Data to be set
    //
    void setData( unsigned char aData );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  setContent
    //      set all fields of message content with the content of given buffer.
    //
    //      Parameters:
    //          aBuffer - a buffer stores data.
    //          aBuffLength - length of the buffer, MUST match the size of internal buffer
    //
    //      Return values:
    //          true - set content successfully.
    //          false - failed to set content because the buffer length doesn't match.
    //
    bool setContent( const char* aBuffer, unsigned int aBuffLength );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  getMessageBuffer
    //      returns the const pointer of the internal buffer, which can be used in other function
    //      call that require a buffer pointer as an input parameter.
    //
    //      Return values:
    //          The const pointer of the internal buffer.
    //
    const char* getMessageBuffer( void ) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  getBufferSize
    //      returns the size of internal buffer.
    //
    //      Return values:
    //          the size of internal buffer.
    //
    static unsigned int getBufferSize(void);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  getPortNumber
    //      returns the port number filed of the message.
    //
    //      Return values:
    //          port number.
    //
    unsigned char getPortNumber( void );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  getStatus
    //      returns the status code of result message.
    //
    //      Return values:
    //          status code.
    //
    StatusCode getStatus( void );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  getData
    //      returns the data of the message.
    //
    //      Return values:
    //          data.
    //
    unsigned char getData( void );

	///////////////////////////////////////////////////////////////////////////////////////////////
    // Copy constructor
    //
    GPIAMessage( const GPIAMessage& );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Assignment Operator
    //
    GPIAMessage& operator=( const GPIAMessage& );

private:

    char* mMessageContent;
    static const unsigned int cBufferSize;
};
#endif