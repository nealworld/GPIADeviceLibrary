using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace FPGASimulator
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        public MainWindow()
        {
            InitializeComponent();
            mCmd = new SerialPort( "COM1", 9600, Parity.None, 8, StopBits.One );
            mCmd.DataReceived += new SerialDataReceivedEventHandler( mCmd_DataReceived );

            this.DataContext = this;
        }

        private SerialPort mCmd;
        private byte mData = 0x00;

        private void button1_Click( object sender, RoutedEventArgs e )
        {
            mCmd.Open();
        }

        private void mCmd_DataReceived( object aSender, SerialDataReceivedEventArgs aEvent )
        {
            lock ( this )
            {
                int lDataNum = mCmd.BytesToRead;
                while ( lDataNum > 0 )
                {
                    byte lCmdMessage = (byte)mCmd.ReadByte();
                    lDataNum--;
                    if ( lCmdMessage < 0x7F )
                    {
                        byte[] lSend = new byte[1];
                        lSend[0] = mData;
                        mCmd.Write( lSend, 0, 1 );

                        DisplayString += "Port=" + lCmdMessage + ", send=" + mData + "\r\n";
                    }
                    else
                    {
                        mData = (byte)mCmd.ReadByte();
                        DisplayString += "Port=" + ( lCmdMessage - 0x80 ) + ", write=" + mData + "\r\n";
                        lDataNum--;
                    }
                }
            }
        }

        public string DisplayString
        {
            get
            {
                return mDisplayString;
            }
            set
            {
                mDisplayString = value;
                if ( PropertyChanged != null )
                {
                    PropertyChanged( this, new PropertyChangedEventArgs( "DisplayString" ) );
                }
            }
        }

        private string mDisplayString = string.Empty;
        public event PropertyChangedEventHandler PropertyChanged;

        private void button2_Click( object sender, RoutedEventArgs e )
        {
            DisplayString = "";
        }
    }
}