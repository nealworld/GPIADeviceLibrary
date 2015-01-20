#include "StdAfx.h"
#include "ReqTracibility.h"

BOOL ReqTracibility::setConsoleColor( WORD aAttributes )
{
    if ( mHConsole == INVALID_HANDLE_VALUE )
    {
        return FALSE;
    }
    BOOL lResult = SetConsoleTextAttribute( mHConsole, aAttributes );
    return lResult;
}
char ReqTracibility::kbCheck( void )
{
    char lCh;
    std::cout<<"......Wait for Keyboard Input ( N/n to validate false and Q/q to quit ):" <<std::endl<<std::flush;
    *mFileOut<<"......Wait for Keyboard Input ( N/n to validate false and Q/q to quit ):" <<std::endl<<std::flush;
    lCh=_getch( );
    if( lCh =='Q' || lCh =='q' )
    {
        //printOptions();//SIGNAL ->EVENT TRIGGER!
        mFileOut->close();
       exit( 1 );
    }
    else
    {
        std::cout<<" Continue!"<<std::endl<<std::flush;
        *mFileOut<<" Continue!"<<std::endl<<std::flush;
    }
    return( lCh );
};


void ReqTracibility::sigReplace( char* aStr ,unsigned int aBegan, unsigned int aEnd,char aOld,char aNew )
{
    unsigned int lLength = strlen( aStr );
    for( unsigned int aIndex=aBegan; ( ( aIndex < aEnd ) && ( aIndex < lLength ) ); aIndex++ )
    {
        if( aStr[aIndex] == '\n' )
        {
            aStr[aIndex] = '_';
        };
        if( aStr[aIndex] == aOld )
        {
            aStr[aIndex] = aNew;
        };
    };
};
void ReqTracibility::createFileName( char* aFileName )
{
    char* lFileName=aFileName; 
    struct tm * lTmptr;
    time_t lSecNow;
    time( &lSecNow );
    lTmptr = localtime( &lSecNow );
    strcat( lFileName, " Time:" );
    strcat( lFileName, asctime( lTmptr ) );
    sigReplace( aFileName,0,strlen( lFileName ),':','_' );
};

ReqTracibility::ReqTracibility( const char* aTitle,char** aTPOptions,unsigned int aTPNumb )
{
    mCurrentOption=0;
    memset( mTestName,0,500 );
    strcpy_s( mTestName,aTitle );
    createFileName( mTestName );
    strcat_s( mTestName,".txt" );
    mFileOut =new std::ofstream(mTestName);
    if(*mFileOut ==NULL )
    {
        std::cout<<"The file was not opened"<<std::endl<<std::flush;
        *mFileOut<<"The file was not opened"<<std::endl<<std::flush;
        exit( 1 );       
    };  
    mTestProcedures=aTPOptions;
    mTestProNumb=aTPNumb;
  
    
    mHConsole = GetStdHandle( STD_OUTPUT_HANDLE );
    setConsoleColor( FOREGROUND_INTENSITY );
}
ReqTracibility::~ReqTracibility( void )
{
    mFileOut->close();            
}

 unsigned int ReqTracibility::printOptions( )
{
    setConsoleColor( FOREGROUND_BLUE |FOREGROUND_INTENSITY );
    std::cout<<"There are "<< mTestProNumb << " individual "<<" TestProcedures "<<std::endl<<std::flush;
    *mFileOut<<"There are "<< mTestProNumb << " individual "<<" TestProcedures "<<std::endl<<std::flush;
    for( unsigned int i= 0; i<mTestProNumb; i++ )
    {
        if( mCurrentOption==i )
        {
            setConsoleColor( FOREGROUND_GREEN |FOREGROUND_INTENSITY );
        }
        else
        {
            setConsoleColor( FOREGROUND_GREEN );
        };
        std::cout<<" "<<i+1<<" Test Procedure "<<mTestProcedures[i]<<std::endl<<std::flush;
        *mFileOut<<" "<<i+1<<" Test Procedure "<<mTestProcedures[i]<<std::endl<<std::flush;
    };

    std::cout<<"Please Choose A Test Procedure(1,2,3....)"<<std::endl<<std::flush;
    *mFileOut<<"Please Choose A Test Procedure(1,2,3....)"<<std::endl<<std::flush;
    char lCh;

    bool lenter=false;
    unsigned int lcz=0;
    while( !lenter )
    {
        lCh=_getch( );
        if(lCh>='1' &&lCh<='9'&&(lCh-'1')<mTestProNumb )
        {
            lcz = lCh-'1';
            mCurrentOption=lcz;
            lenter=true;
            std::cout<<"Choosen:"<<lcz+1<<" "<<mTestProcedures[lcz]<<std::endl<<std::flush;
            *mFileOut<<"Choosen:"<<lcz+1<<" "<<mTestProcedures[lcz]<<std::endl<<std::flush;
            //mTpOptions();
            //SIGNAL ->EVENT TRIGGER!
        }
        if( lCh =='Q' || lCh =='q' )
        {
            mFileOut->close();
            exit( 1 );
        }
    }
    setConsoleColor( FOREGROUND_INTENSITY );
    return(lcz);
};


void ReqTracibility::printErrorExit( const char* aComments )
{
    std::cout<<"Error happend : "<<aComments<<std::endl<<std::flush;
    *mFileOut<<"Error happend : "<<aComments<<std::endl<<std::flush;
};


void ReqTracibility::printComments( const char* aOpertion )
{
    std::cout<<aOpertion<<std::endl<<std::flush;
    *mFileOut<<aOpertion<<std::endl<<std::flush;       
};
void ReqTracibility::printComments( const char* aOpertion, __int32 aValue )
{
    std::cout<<aOpertion<<":"<<std::dec<<(int)aValue<<std::endl<<std::flush;
    *mFileOut<<aOpertion<<":"<<std::dec<<(int)aValue<<std::endl<<std::flush;     
};

void ReqTracibility::printHeadline( const char* aOpertion )
{
    setConsoleColor( FOREGROUND_GREEN |FOREGROUND_INTENSITY );
    std::cout<<aOpertion<<std::endl<<std::flush;
    *mFileOut<<aOpertion<<std::endl<<std::flush;
    setConsoleColor( BACKGROUND_INTENSITY );
};
void ReqTracibility::printHeadline( const char* aHead,const char* aOpertion,__int32 aIndex )
{
    setConsoleColor( FOREGROUND_GREEN |FOREGROUND_INTENSITY );
    std::cout<<aHead<<"_"<<std::dec<<aIndex<<aOpertion<<std::endl<<std::flush;
    *mFileOut<<aHead<<"_"<<std::dec<<aIndex<<aOpertion<<std::endl<<std::flush;     
    setConsoleColor( BACKGROUND_INTENSITY );

};
void ReqTracibility::printHeadline( const char* aHead,const char* aOpertion, __int32 aValue, __int32 aIndex )
{
    setConsoleColor( FOREGROUND_GREEN |FOREGROUND_INTENSITY );
    std::cout<<aHead<<"_"<<aIndex<<aOpertion<<"{"<<std::dec<<(int)aValue<<"}"<<std::endl<<std::flush;
    *mFileOut<<aHead<<"_"<<aIndex<<aOpertion<<"{"<<std::dec<<(int)aValue<<"}"<<std::endl<<std::flush;     
    setConsoleColor( BACKGROUND_INTENSITY );
};


void ReqTracibility::verify( const char* aReqID, const char*  aComments ,
    unsigned int aPartriallyCoverred )
{
    setConsoleColor( FOREGROUND_GREEN |FOREGROUND_INTENSITY|BACKGROUND_BLUE );
    unsigned int lPartriallyCoverred=aPartriallyCoverred;
    std::cout<<"Validate :Is this requirement [ "<< aReqID <<" ]" << "Comments {" << aComments <<"}" <<std::endl<<std::flush;
    *mFileOut<<"Validate :Is this requirement [ "<< aReqID <<" ]" << "Comments {" << aComments <<"}" <<std::endl<<std::flush;      
    mValidate=kbCheck( );
    if( mValidate =='N' || mValidate =='n' )
    {
        std::cout<<"Failed!" <<"[" << aReqID <<"]"<<std::endl<<std::flush;
        *mFileOut<<"Failed!" <<"[" << aReqID <<"]"<<std::endl<<std::flush;
    }
    else
    {
        if( lPartriallyCoverred>1 )
        {
            std::cout<<"Partially Passed! Requirement [ " << aReqID <<" ]" << "{ "<< "1 of " << lPartriallyCoverred << " }" << std::endl<<std::flush;
            *mFileOut<<"Partially Passed! Requirement [ " << aReqID <<" ]" << "{ "<< "1 of " << lPartriallyCoverred << " }" << std::endl<<std::flush;        
        }
        else
        {
            std::cout<<"Passed! Requirement" <<"[ " << aReqID <<" ]"<<std::endl<<std::flush;
            *mFileOut<<"Passed! Requirement" <<"[ " << aReqID <<" ]"<<std::endl<<std::flush; 
        }
    }
    setConsoleColor( BACKGROUND_INTENSITY );
}
void ReqTracibility::verify( const char* aReqID, const char*  aComments ,int aParam,
    unsigned int aPartriallyCoverred )
{
    setConsoleColor( FOREGROUND_GREEN |FOREGROUND_INTENSITY|BACKGROUND_BLUE );
    unsigned int lPartriallyCoverred=aPartriallyCoverred;
    std::cout<<"Validate Passed or not :Requirement [ "<< aReqID <<" ]" << "Comments {" << aComments <<"}" << std::flush;
    *mFileOut<<"Validate Passed or not :Requirement [ "<< aReqID <<" ]" << "Comments {" << aComments <<"}" << std::flush;

    std::cout<<" Param{"<<std::dec<<(int)aParam<<"}"<<std::endl<<std::flush; 
    
    *mFileOut<<" Param{"<<std::dec<<(int)aParam<<"}"<<std::endl<<std::flush; 
    mValidate= kbCheck( );
    if( mValidate =='N' || mValidate =='n' )
    {
        std::cout<<"Failed!" <<"[" << aReqID <<"]"<<std::endl<<std::flush;
        *mFileOut<<"Failed!" <<"[" << aReqID <<"]"<<std::endl<<std::flush;      
    }
    else
    {
        if( lPartriallyCoverred>1 )
        {
            std::cout<<"Partially Passed! Requirement [ " << aReqID <<" ]" << "{ "<< "1 of " << lPartriallyCoverred << " }" << std::endl<<std::flush;
            *mFileOut<<"Partially Passed! Requirement [ " << aReqID <<" ]" << "{ "<< "1 of " << lPartriallyCoverred << " }" << std::endl<<std::flush;       
        }
        else
        {
            std::cout<<"Passed! Requirement" <<"[ " << aReqID <<" ]"<<std::endl<<std::flush;
            *mFileOut<<"Passed! Requirement" <<"[ " << aReqID <<" ]"<<std::endl<<std::flush;       
        }
    }
    setConsoleColor( BACKGROUND_INTENSITY );
}

