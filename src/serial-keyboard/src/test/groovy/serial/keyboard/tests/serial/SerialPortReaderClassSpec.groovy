package serial.keyboard.tests.serial

import com.fazecast.jSerialComm.SerialPort
import io.micronaut.test.annotation.MicronautTest
import io.reactivex.subscribers.TestSubscriber
import serial.keyboard.infrastructure.actors.SerialActor
import serial.keyboard.serial.SerialPortReader
import spock.lang.Specification

import javax.inject.Inject

@MicronautTest
class SerialPortReaderClassSpec extends Specification {

    @Inject
    SerialPortReader reader

    SerialActor serialActor

    TestSubscriber<Short> serialSubscriber = new TestSubscriber<>()

    def setup() {
        serialActor = new SerialActor()
    }

    def cleanup() {
        serialActor.close()
    }

    def "SerialReader should return the number passed in via a serial port"() {
        given:
        Short value = 10

        when:
        reader.subscribe().subscribe(serialSubscriber)
        serialActor.send(value)

        then:
        serialSubscriber.awaitCount(1).assertValue(value)
    }

    def "SerialReader should return the a series of numbers passed in via the serial port"() {
        given:
        List<Short> values = [10 as short, 60 as short, 1234 as short]

        when:
        reader.subscribe().subscribe(serialSubscriber)
        values.forEach { serialActor.send(it as short) }

        then:
        serialSubscriber.awaitCount(values.size()).assertValueSequence(values)
    }

    def "SerialReader should properly sync when bytes are mixed"() {
        given:
        byte[] byteArray = [0xFC, 0x0, 0x1] as byte[]

        when:
        reader.subscribe().subscribe(serialSubscriber)
        serialActor.send(byteArray)

        then:
        serialSubscriber.awaitCount(1).assertValue(1 as short)
    }
}
