package serial.keyboard.serial

import com.fazecast.jSerialComm.SerialPort
import com.fazecast.jSerialComm.SerialPortEvent
import com.fazecast.jSerialComm.SerialPortPacketListener
import io.reactivex.BackpressureStrategy
import io.reactivex.Flowable
import java.util.function.Consumer
import javax.inject.Singleton

@Singleton
class SerialPortReader constructor(
        private val serialPortManager: SerialPortManager,
        private val reader: FcReader
) {
    fun subscribe(): Flowable<Short> {
        return Flowable.create({emitter ->
            serialPortManager.serialPort.addDataListener(PortListener(Consumer { data ->
                val value = reader.parseByte(data, serialPortManager.serialPort)
                if (value.toInt() != -1) {
                    emitter.onNext(value)
                }
            }))
        }, BackpressureStrategy.DROP)
    }

    class PortListener(private val callback: Consumer<ByteArray>) : SerialPortPacketListener {
        override fun getPacketSize(): Int = 2

        override fun serialEvent(event: SerialPortEvent?) {
            if (event!!.eventType == SerialPort.LISTENING_EVENT_DATA_RECEIVED) {
                callback.accept(event.receivedData);
            }
        }
        override fun getListeningEvents(): Int = SerialPort.LISTENING_EVENT_DATA_RECEIVED
    }
}