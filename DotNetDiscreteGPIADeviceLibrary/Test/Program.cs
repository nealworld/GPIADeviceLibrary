using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DotNetDiscreteGPIADeviceLibrary;

namespace CSharpGPIADriverTest
{
    class Program
    {
        static void Main ( string[] args )
        {
            DotNetDiscreteGPIADevice lDevice = new DotNetDiscreteGPIADevice ();
            int lDeviceID = -1;
            var lBitID1 = -1 ;
            var lBitID2 = -1;
            DotNetDiscreteGPIADevice.Result lResult;

            lResult = lDevice.defineDevice ( 0, "192.168.115.40", ref lDeviceID );
            if ( lResult == DotNetDiscreteGPIADevice.Result.ResultSuccess )
            {
                lResult = lDevice.defineBit( lDeviceID, 2, 4, DotNetDiscreteGPIADevice.Direction.Receive, ref lBitID1 );
                lResult = lDevice.defineBit( lDeviceID, 2, 4, DotNetDiscreteGPIADevice.Direction.Transmit, ref lBitID2 );
                if ( lDevice.open() == DotNetDiscreteGPIADevice.Result.ResultSuccess )
                {
                    var lData = true;
                    bool lLoop = true;
                    while (lLoop)
                    {
                        lResult = lDevice.read ( lBitID1, ref lData );
                        lResult = lDevice.write ( lBitID2, false );
                        lResult = lDevice.read ( lBitID1, ref lData );
                        lResult = lDevice.write ( lBitID2, true );
                        lLoop = false;
                    }
                }
                lResult = lDevice.close ();
            }
            return;
        }
    }
}
