// TestProcedures_Discrete.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"DiscreteProcedure.h"

IDiscreteDevice * mIAnalogDLL=NULL;
bool loadDLL( )
{
    HINSTANCE lDll=LoadLibrary( TEXT( "/Sim/Product/DiscreteGPIADeviceLibrary/Source/Release/DiscreteGPIADeviceLibrary.dll" ) );
    createDev lDeviceAdd;
    bool lResult=false;
    if( lDll!=NULL )
    {
        lDeviceAdd=( createDev )GetProcAddress( lDll,"createDevice" );
        if( lDeviceAdd!=NULL )
        {
            mIAnalogDLL=( IDiscreteDevice* )( *lDeviceAdd )( );
            lResult=true;
        }
        else
        {
            std::cout<< "GetProcAddress error!"<<std::endl<<std::flush;
        }
    }
    else
    {
        std::cout<< "HINSTANCE h loaded error!"<<std::endl<<std::flush;
    }
    return( lResult );
}


void TestData_In_Out( DiscreteTestProOprt* aTestPro )
{
    DiscreteTestProOprt* lTestPro = aTestPro;

    lTestPro->mReqTracer->printHeadline( "1.1 Data Input and Output Test" );
    lTestPro->mReqTracer->printHeadline( "1.1.1 Objective Input Ports and Output Ports can receive/send Bit data correctly!" );
    lTestPro->mReqTracer->printHeadline( "1.1.2 Steps: " );
    lTestPro->StepOneDefine1Device( "GPIA000070" );
    lTestPro->mReqTracer->printComments( "Please Get the Register monitor software in use" );
    lTestPro->StepTwoDefineInputChannel( 8,6 );
    lTestPro->StepThreeDefineOutputChannel( 2,6 );
    lTestPro->StepFourOpenDevice( );

    char lFileName[500];
    strcpy_s( lFileName,lTestPro->mReqTracer->getFileName() );
    strcat_s( lFileName,".csv" );
    lTestPro->StepDynamicRecordData( lFileName,0 );


    lTestPro->mReqTracer->verify( "DISDEV_28","Discrete card device can be Opened",1 );

    lTestPro->mReqTracer->verify( "DISDEV_27","The trace log generated!",1 );
    lTestPro->mReqTracer->verify( "DISDEV_33","The recording Starts from now on ",1 );


    lTestPro->StepFiveWriteOutputChannel( true );
    Sleep(500);
    lTestPro->StepSixReadInputChannel();
    lTestPro->StepFiveWriteOutputChannel( false );
    Sleep(500);
    lTestPro->StepSixReadInputChannel();
    lTestPro->StepFiveWriteOutputChannel( true );
    Sleep(500);
    lTestPro->StepSixReadInputChannel();

    lTestPro->mReqTracer->verify( "DISDEV_45","The recording begans from now on with timestamp ",1 );
    lTestPro->mReqTracer->verify( "DISDEV_29","Output Port  are correctly Configured!",1 );//1?
    lTestPro->mReqTracer->verify( "DISDEV_31","Output Bit Data are correctly Sent!",1 );

    lTestPro->mReqTracer->verify( "DISDEV_30","Input Port are correctly Configured!",1 );
    lTestPro->mReqTracer->verify( "DISDEV_36","Input Bit Data are correctly Read!",1 );

    lTestPro->StepDynamicRecordDataStop( 0 );
    lTestPro->mReqTracer->verify( "DISDEV_35","The recording Ends form now on ",1 );
    lTestPro->StepSevenCloseDevice( );
    lTestPro->mReqTracer->verify( "DISDEV_32","Card devices can be Closed",1 );
};
void TestMultidevices( DiscreteTestProOprt* aTestPro )
{
    DiscreteTestProOprt* lTestPro=aTestPro;

    lTestPro->mReqTracer->printHeadline( "1.2 Two Discrete devices Test" );
    lTestPro->mReqTracer->printHeadline( "1.2.1 Objective The Two device Input Ports and Output Ports can receive/send Bit data correctly!" );
    lTestPro->mReqTracer->printHeadline( "1.2.2 Steps: " );
    char* mDeviceName[]={"GPIA000070","GPIA000001"};
    lTestPro->StepLastMultiDevicesOperation( 2,mDeviceName );

    lTestPro->mReqTracer->verify( "DISDEV_23","Two card devices can read/write correctly",1 );
    lTestPro->mReqTracer->verify( "DISDEV_24","Multi-ports ( Two Devices )can read/write correctly!",1 );

    lTestPro->StepSevenCloseDevice( );

};
void TestErrors( DiscreteTestProOprt* aTestPro )
{
    DiscreteTestProOprt* lTestPro = aTestPro;

    lTestPro->mReqTracer->printComments( "Error tests provides several cases to verify the error log" );
    lTestPro->mReqTracer->printHeadline( "1.3 ErrorLog Discrete device Test" );
    lTestPro->mReqTracer->printHeadline( "1.3.1 Open/Close Error " );
    lTestPro->mReqTracer->printHeadline( "1.3.1.1 Objective: When open one not correctly defined Device, it logs error" );
    lTestPro->mReqTracer->printHeadline( "1.3.1.2 Steps: " );
    lTestPro->StepOpenCloseErrorTestOne( );
    lTestPro->mReqTracer->verify( "DISDEV_38","Open error!",1 );
    lTestPro->mReqTracer->verify( "DISDEV_39","Close error!",1 );

    lTestPro->mReqTracer->printHeadline( "1.3.2 Read/Write Error " );
    lTestPro->mReqTracer->printHeadline( "1.3.2.1 Objective: When read/write one not Opened Device, it logs error" );
    lTestPro->mReqTracer->printHeadline( "1.3.2.2 Steps: " );
    lTestPro->StepTRXErrorTestTwo( );
    lTestPro->mReqTracer->verify( "DISDEV_40","Write error!",1 );
    lTestPro->mReqTracer->verify( "DISDEV_41","Read error!",1 );

    lTestPro->mReqTracer->printHeadline( "1.3.3 TraceLog Error " );
    lTestPro->mReqTracer->printHeadline( "1.3.3.1 Objective: When Device not opened and it set to recording, it logs error" );
    lTestPro->mReqTracer->printHeadline( "1.3.3.2 Steps: " );
    lTestPro->StepRecordErrorTestThree( );
    lTestPro->mReqTracer->verify( "DISDEV_44","TraceLog error!",1 );

};

int _tmain( int argc, _TCHAR* argv[] )
{
    char * lTitle = "Test Procedure( AutoTest ) For GPIA Discrete Device Library";
    char *lTP[]={"TestData_In_Out","TestMultidevices","TestErrors"};
    unsigned const int lTPNumb=3;
    void (*lTpOptions[lTPNumb])(DiscreteTestProOprt* aTestPro);
    lTpOptions[0]=TestData_In_Out;
    lTpOptions[1]=TestMultidevices;
    lTpOptions[2]=TestErrors;
    ReqTracibility* lReqTracer = new ReqTracibility( lTitle,lTP,lTPNumb );

    lReqTracer->printComments( lTitle );
    if( loadDLL( ) == false )
    {
        std::cout<< "DLL load error!" <<std::endl<<std::flush;
        lReqTracer->printComments( "Discrete DeviceLibrary DLL Load Error!" );
        exit( 1 );
    };

    DiscreteProcedure* lDeviceProcedure =new DiscreteProcedure( mIAnalogDLL,lReqTracer );
    DiscreteTestProOprt* lTestPro =new DiscreteTestProOprt( lDeviceProcedure );
    bool lcontinue=false;
    unsigned int lOption=0;
    while(1)
    {
        lOption=lTestPro->mReqTracer->printOptions();
        if(lOption>=0&&lOption<lTPNumb)
        {
            lTpOptions[lOption](lTestPro);
        }
    };
    return 0;
}

