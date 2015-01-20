// GPIADriverTest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <Windows.h>
#include <sstream>
#include <string>
#include "IOA/IDiscreteDevice.h"
extern "C" __declspec(dllimport) IDiscreteDevice* createDevice( void );

int _tmain(int argc, _TCHAR* argv[])
{
    //typedef IDiscreteDevice* (_stdcall * CreateDeviceType) ( void );

    //// The handle to the library
    //static HINSTANCE lLibraryHandle = LoadLibrary( "D:\\Sim\\Product\\DiscreteGPIADeviceLibrary\\Source\\Release\\DiscreteGPIADeviceLibrary.dll" );
    //// Handle to the lCreateDevice function
    //CreateDeviceType lCreateDevice;

    //// Get the address of the createDevice function
    //lCreateDevice = (CreateDeviceType) GetProcAddress( lLibraryHandle, "createDevice" );

    //IDiscreteDevice* lDevice = (*lCreateDevice)();
    IDiscreteDevice* lDevice = createDevice();
    IDiscreteDevice::DeviceID lDeviceID;
    IDiscreteDevice::BitID lBitID1;
    IDiscreteDevice::BitID lBitID2;
    IDiscreteDevice::Result lResult;

    lResult = lDevice->defineDevice(0,"192.168.135.40", &lDeviceID );
    if ( lResult == IDiscreteDevice::ResultSuccess )
    {
        lResult = lDevice->defineBit(lDeviceID, 0x05,4,IDiscreteDevice::Receive,&lBitID1);
        lResult = lDevice->defineBit(lDeviceID, 0x05,4,IDiscreteDevice::Transmit,&lBitID2);
        if ( lDevice->open() == IDiscreteDevice::ResultSuccess )
        {
            IOA::Bool lData = true;
            bool lLoop = true;
            while(lLoop)
            {
                lResult = lDevice->read(lBitID1,&lData);
                lResult = lDevice->write(lBitID2,false);
                lResult = lDevice->read(lBitID1,&lData);
                lResult = lDevice->write(lBitID2,true);
            }
        }
        lResult = lDevice->close();
    }
    return 0;
}