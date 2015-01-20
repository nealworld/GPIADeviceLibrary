#pragma once

#include "DiscreteGPIAConfig.h"

class DiscreteProcedure :public DeviceProcedure
{
public:
	DiscreteProcedure( IDiscreteDevice* aDeviceLibDll,ReqTracibility* aReqTracer );
	friend class DiscreteTestProOprt;
	~DiscreteProcedure( void );
	
protected:
	IDiscreteDevice* mDeviceLibDll;
	IDiscreteDevice::BitID mInBitID[GPIADISCRETE_DEVICETEST_NUMBER][BIT_TEST_NUMBER];
	IDiscreteDevice::BitID mOutBitID[GPIADISCRETE_DEVICETEST_NUMBER][BIT_TEST_NUMBER];
	IDiscreteDevice::DeviceID mDevice[GPIADISCRETE_DEVICETEST_NUMBER];

	/********DeviceProcedure Interface**********/
	virtual void cleanDefinitions( void );
	virtual bool checkResult( UInt32 aResult );
	virtual bool startIO( void );
	virtual bool closeIO( void );
	virtual bool defineMultiDevices( UInt32 aNumber, char** aConfig );
	virtual bool recordStart( UInt32 aDeviceID, const char* aName );
	virtual bool recordEnd( UInt32 aDeviceID );
	/********End DeviceProcedure Interface**********/

	/***********GPIA DISCRETE UNIQUE OPERATION*************/
	bool defineInBitPort( UInt32 aDeviceID,
	UInt32 aPortID,UInt32 aPosition , UInt32 aInBitID );
	bool defineOutBitPort( UInt32 aDeviceID,
	UInt32 aPortID,UInt32 aPosition , UInt32 aInBitID );
	bool readBit( UInt32 aDeviceID,UInt32 aInBitID,Bool* aData );
	bool writeBit( UInt32 aDeviceID,UInt32 aOutBitID,Bool aData );
    /***********END GPIA DISCRETE UNIQUE OPERATION*************/
};

class DiscreteTestProOprt : public TestProOprt
{
public:
	DiscreteTestProOprt( DiscreteProcedure* aAnalogProcedure );
private:
  DiscreteProcedure* mDeviceProcedure;

  Bool mRxData;
public:
  virtual void StepZeroInitTestData( void );
  virtual void StepEndCleanRCVbuffer( void );
  void StepTwoDefineInputChannel( UInt32 aPortID,UInt32 aPoz );
  
  void StepThreeDefineOutputChannel( UInt32 aPortID,UInt32 aPoz );

 
  void StepFiveWriteOutputChannel( Bool aBitVal );
  
  void StepSixReadInputChannel( void );

  virtual bool StepTRXErrorTestTwo( void );
  virtual bool StepLastMultiDevicesOperation( UInt32 aNumber ,char** aDeviceName );
};
