package serial.keyboard.infrastructure.actors

import com.fazecast.jSerialComm.SerialPort

class SerialActor {
    SerialPort testPort = SerialPort.getCommPort("/dev/pts/4")

    SerialActor() {
        testPort.parity = SerialPort.NO_PARITY
        testPort.baudRate = 9600
        open()
    }

    void open() {
        if (!testPort.open) {
            if (!testPort.openPort()) {
                throw new Exception("Coudln't open serial test port")
            }
        }
    }

    void close() {
        testPort.closePort()
    }

    void send(Short value) {
        send(toByteArray(value))
    }

    void send(byte[] values) {
        testPort.writeBytes(values, values.length)
    }

    private static byte[] toByteArray(Short value) {
        byte[] valueBuffer = new byte[2]
        valueBuffer[0] = (byte)(value & 0xff)
        valueBuffer[1] = (byte)((value >> 8) & 0xff)

        return valueBuffer
    }
}
