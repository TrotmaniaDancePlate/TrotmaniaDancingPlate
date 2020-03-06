package serial.keyboard.serial

import com.fazecast.jSerialComm.SerialPort
import io.micronaut.context.annotation.Value
import io.micronaut.context.event.ApplicationEventListener
import io.micronaut.runtime.event.ApplicationShutdownEvent
import java.lang.Exception
import javax.inject.Singleton

@Singleton
class SerialPortManager constructor(
        @Value("\${application.serial.port-name}") portName: String
): ApplicationEventListener<ApplicationShutdownEvent> {
    val serialPort: SerialPort = SerialPort.getCommPort(portName)

    init {
        serialPort.parity = SerialPort.NO_PARITY
        serialPort.baudRate = 9600
        if (!serialPort.openPort()) {
            throw Exception("Couldn't open port ${portName}")
        }
    }

    override fun onApplicationEvent(event: ApplicationShutdownEvent?) {
        serialPort.closePort()
    }
}