package serial.keyboard.serial

import com.fazecast.jSerialComm.SerialPort
import java.nio.ByteBuffer
import javax.inject.Singleton

@Singleton
class FcReader {
    fun parseByte(data: ByteArray, serialPort: SerialPort): Short {
        var upper: Byte = data[0]
        var lower: Byte = data[1]

        val singleByteBuffer = ByteArray(1)

        if (isFc(upper)) {
            if (isFc(lower)) {
                return -1;
            } else {
                serialPort.readBytes(singleByteBuffer, 1);

                upper = lower
                lower = singleByteBuffer[0]
            }
        }
        return ByteBuffer.wrap(byteArrayOf(upper, lower)).short
    }

    private fun isFc(value: Byte): Boolean {
        val firstDigit = toDigit("FC"[0])
        val secondDigit = toDigit("FC"[1])
        return value == ((firstDigit shl 4) + secondDigit).toByte()
    }

    private fun toDigit(hexChar: Char): Int {
        val digit = Character.digit(hexChar, 16)
        require(digit != -1) { "Invalid Hexadecimal Character: $hexChar" }
        return digit
    }
}