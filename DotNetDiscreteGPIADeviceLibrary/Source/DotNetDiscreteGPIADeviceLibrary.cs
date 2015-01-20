using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ManagedDiscreteGPIADeviceLibrary;

namespace DotNetDiscreteGPIADeviceLibrary
{
    /// <summary>
    /// DotNetDiscreteGPIADevice definition
    /// </summary>
    public class DotNetDiscreteGPIADevice
    {
        public enum Result
        {
            ResultSuccess = 0,
            ResultNoData = 1,
            ResultNotEnoughMemory = 2,
            ResultOverflow = 3,
            ResultInvalidParameter = 4,
            ResultNotAvailable = 5,
            ResultNotSupported = 6,
            ResultInvalidOperation = 7,
        }

        public enum Direction
        {
            Transmit = 0,
            Receive = 1
        }

        private ManagedDiscreteGPIADeviceLib mManagedDeviceLib;

        /// <summary>
        /// Constructor
        /// </summary>
		public DotNetDiscreteGPIADevice()
        {
            mManagedDeviceLib = new ManagedDiscreteGPIADeviceLib();
        }

		/// <summary>
		/// Destructor
		/// </summary>
        ~DotNetDiscreteGPIADevice( )
        {
        }

        /// <summary>
        /// Define Device
        /// </summary>
        /// <param name="aID">ID </param>
        /// <param name="aConfig">Config name</param>
        /// <param name="aDeviceID">Device ID</param>
        /// <returns>Result status</returns>
        public Result defineDevice( UInt32 aID, string aConfig, ref int aDeviceID )
        {
            Result lResult = Result.ResultSuccess;
            lResult = (Result)mManagedDeviceLib.defineDevice( aID, aConfig, ref aDeviceID );
            return lResult;
        }

        /// <summary>
        /// defineBit
        /// </summary>
        /// <param name="aDeviceID">Device ID</param>
        /// <param name="aPortNumber">Port Number</param>
        /// <param name="aBitPosition">Bit Position</param>
        /// <param name="aDirection">Direction</param>
        /// <param name="aBitID">Bit ID</param>
        /// <returns>Result status</returns>
        public Result defineBit( int aDeviceID, UInt32 aPortNumber, UInt32 aBitPosition, Direction aDirection, ref int aBitID )
        {
            Result lResult = Result.ResultSuccess;
            lResult = (Result)mManagedDeviceLib.defineBit( aDeviceID, aPortNumber, aBitPosition, (DirectionWrapper)aDirection, ref aBitID );
            return lResult;
        }

        /// <summary>
        /// Open device
        /// </summary>
        /// <returns>Result status</returns>
        public Result open( )
        {
            Result lResult = Result.ResultSuccess;
            lResult = (Result)mManagedDeviceLib.open();
            return lResult;
        }

        /// <summary>
        /// Close device
        /// </summary>
        /// <returns>Result status</returns>
        public Result close( )
        {
            Result lResult = Result.ResultSuccess;
            lResult = (Result)mManagedDeviceLib.close();
            return lResult;
        }

        /// <summary>
        /// Read bit value
        /// </summary>
        /// <param name="aBitID">Bit ID</param>
        /// <param name="aData">value</param>
        /// <returns>Result status</returns>
        public Result read( Int32 aBitID, ref bool aData )
        {
            Result lResult = Result.ResultSuccess;
            lResult = (Result)mManagedDeviceLib.read( aBitID, ref aData );
            return lResult;
        }

        /// <summary>
        /// Write value to bit
        /// </summary>
        /// <param name="aBitId">BitID"</param>
        /// <param name="aData">New value</param>
        /// <returns>Result status</returns>
        public Result write( Int32 aBitID, bool aData )
        {
            Result lResult = Result.ResultSuccess;
            lResult = (Result)mManagedDeviceLib.write( aBitID, aData );
            return lResult;
        }

        /// <summary>
        /// Get Last Error
        /// </summary>
        /// <param name="aBuffer">Buffer string</param>
        /// <param name="aBufferLength">Buffer Length</param>
        /// <returns>Result status</returns>
        public Result getLastError( ref string aBuffer, UInt32 aBufferLength )
        {
            Result lResult = Result.ResultSuccess;
            lResult = (Result)mManagedDeviceLib.getLastError( aBuffer, aBufferLength );
            return lResult;
        }

        /// <summary>
        /// Get Interface Version
        /// </summary>
        /// <returns>Interface Version Number</returns>
        public UInt32 getInterfaceVersion( )
        {
            UInt32 lResult = mManagedDeviceLib.getInterfaceVersion();
            return lResult;
        }
    };
}
