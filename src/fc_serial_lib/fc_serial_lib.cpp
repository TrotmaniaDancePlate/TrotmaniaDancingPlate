#include "Arduino.h"
#include "HardwareSerial.h"
#include "fc_serial_lib.h"

FcSerial::FcSerial(int syncInterval)
{
    _syncInterval = syncInterval;
}

void FcSerial::send(uint16_t value)
{
    byte retval[2];
    
    toBytes(value, retval);
    writeToSerial(retval);

    _syncCounter++;
    if (_syncCounter == _syncInterval)
    {
        toBytes(0xFCFC, retval);
        writeToSerial(retval);
        _syncCounter = 0;
    }
}

void FcSerial::toBytes(uint16_t input, byte retval[2])
{
    retval[0] = (byte)(input >> 8);
    retval[1] = (byte)(input);
}

void FcSerial::writeToSerial(byte value[2]) {
    Serial.write(value, 2);
    Serial.flush();
}