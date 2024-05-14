using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Threading.Tasks;

namespace ModbusSensorsApp
{
    class ModbusMessage
    {
        private SerialPort mySerialPort = new();
        private readonly int slaveID = 0x05;
        private readonly int readSingleRegisterFunctionCode = 0x03;
        private readonly int writeSingleRegisterFunctionCode = 0x06;
        private readonly int led1Address = 0x1388;
        private readonly int led2Address = 0x1389;
        private readonly int led3Address = 0x138A;
        private readonly int led4Address = 0x138B;
        private readonly int led5Address = 0x138C;
        private readonly int DHT11Address = 0x1B58;
        private readonly int HCSR04Address = 0x1F40;

        public string DataSent {  get; set; } = string.Empty;
        public string DataReceived { get; set; } = string.Empty;

        public ModbusMessage()
        {
            mySerialPort = new SerialPort();
            mySerialPort.DataBits = 8;
            mySerialPort.Handshake = Handshake.None;
            mySerialPort.RtsEnable = true;
        }

        public bool Connect(string portName, int baudRate)
        {
            mySerialPort.BaudRate = baudRate;
            mySerialPort.PortName = portName;

            try
            {
                mySerialPort.Open();
                return true;
            }
            catch
            {
                return false;
            }
        }

        public void Disconnect()
        {
            mySerialPort.Close();
        }

        public ushort ModRTU_CRC(byte[] buf, int len)
        {
            ushort crc = 0xFFFF;

            for (int pos = 0; pos < len; pos++)
            {
                crc ^= buf[pos];

                for (int i = 0; i < 8; i++)
                {
                    if ((crc & 0x0001) != 0)
                    {
                        crc >>= 1;
                        crc ^= 0xA001;
                    }
                    else
                        crc >>= 1;
                }
            }
            return crc;
        }

        public bool[] LoadLEDs()
        {
            byte[] message = new byte[] { (byte)slaveID, (byte)readSingleRegisterFunctionCode,
                                  (byte)(led1Address >> 8), (byte)(led1Address & 0xFF),
                                  0x00, 0x05, 0x00, 0x00 }; // Mesajul Modbus

            ushort crc = ModRTU_CRC(message, message.Length);
            message[message.Length - 2] = (byte)(crc & 0xFF);
            message[message.Length - 1] = (byte)(crc >> 8);

            mySerialPort.Write(message, 0, message.Length); // Trimiterea mesajului

            Thread.Sleep(100); // Așteptare pentru răspuns

            byte[] response = new byte[15]; // 15 bytes pentru răspuns
            mySerialPort.Read(response, 0, response.Length);

            // Verificare pentru excepție
            if ((response[1] & 0x80) == 0x80)
            {
                throw new Exception("Exception occurred in response.");
            }


            // Interpretarea răspunsului pentru a extrage starea LED-urilor
            bool[] leds = new bool[5];
            for (int i = 3; i < 13; i += 2)
            {
                leds[(i - 3) / 2] = ((response[i] << 8) | response[i + 1]) == 1;
            }

            return leds;
        }

        public ushort CalculateDistance()
        {
            byte[] message = new byte[] { (byte)slaveID, (byte)readSingleRegisterFunctionCode,
                                      (byte)(HCSR04Address >> 8), (byte)(HCSR04Address & 0xFF),
                                      0x00, 0x01, 0x00, 0x00 }; // Mesajul Modbus

            ushort crc = ModRTU_CRC(message, message.Length - 2);
            message[message.Length - 2] = (byte)(crc & 0xFF);
            message[message.Length - 1] = (byte)(crc >> 8);

            mySerialPort.Write(message, 0, message.Length); // Trimiterea mesajului

            Thread.Sleep(2000); // Așteptare pentru răspuns

            byte[] response = new byte[7]; // 7 bytes pentru răspuns
            mySerialPort.Read(response, 0, response.Length);

            // Verificare pentru excepție
            if ((response[1] & 0x80) == 0x80)
            {
                throw new Exception("Exception occurred in response."); // Excepție detectată
            }

            // Interpretarea răspunsului pentru a extrage distanța
            ushort distance = (ushort)((response[3] << 8) | response[4]);

            return distance;
        }

        public void SetLEDState(int offset, bool state)
        {
            byte[] message = new byte[] { (byte)slaveID, (byte)writeSingleRegisterFunctionCode,
                                  (byte)((led1Address + offset) >> 8), (byte)((led1Address + offset) & 0xFF),
                                  0x00 ,state ? (byte)0x00 : (byte)0x01, 0x00, 0x00}; // Mesajul Modbus

            ushort crc = ModRTU_CRC(message, message.Length);
            message[message.Length - 2] = (byte)(crc & 0xFF);
            message[message.Length - 1] = (byte)(crc >> 8);

            mySerialPort.Write(message, 0, message.Length); // Trimiterea mesajului

            Thread.Sleep(500); // Așteptare pentru răspuns

            byte[] response = new byte[8]; // 8 bytes pentru răspuns
            mySerialPort.Read(response, 0, response.Length);

            // Verificare pentru excepție
            if ((response[1] & 0x80) == 0x80)
            {
                throw new Exception("Exception occurred in response.");
            }
        }

        public (ushort, ushort) ReadTemperatureAndHumidity()
        {
            byte[] message = new byte[] { 0x05, 0x03, 0x1B, 0x58, 0x00, 0x01, 0x00, 0x00 }; // Mesajul Modbus

            ushort crc = ModRTU_CRC(message, message.Length);
            message[message.Length - 2] = (byte)(crc & 0xFF);
            message[message.Length - 1] = (byte)(crc >> 8);

            mySerialPort.Write(message, 0, message.Length); // Trimiterea mesajului

            Thread.Sleep(500); // Așteptare pentru răspuns

            byte[] response = new byte[7]; // 7 bytes pentru răspuns
            mySerialPort.Read(response, 0, response.Length);

            // Verificare pentru excepție
            if ((response[1] & 0x80) == 0x80)
            {
                throw new Exception("Exception occurred in response.");
            }

            ushort humidity = response[3];
            ushort temperature = response[4];

            return (humidity, temperature);
        }
    }
}
