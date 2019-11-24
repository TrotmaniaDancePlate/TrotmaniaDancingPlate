#ifndef fcSerialLib_h
#define fcSerialLib_h

#include "Arduino.h"
#include "HardwareSerial.h"

class FcSerial{
    public:
        FcSerial(int syncInterval);
        void send(uint16_t value);
    private:
        int _syncInterval;
        int _syncCounter;
        
        void toBytes(uint16_t input, byte retval[2]);
        void writeToSerial(byte value[2])
}