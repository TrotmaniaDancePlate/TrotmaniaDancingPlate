package io.github.chase22.keyboardserial.serial;

import jssc.SerialPort;
import jssc.SerialPortException;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.nio.ByteBuffer;
import java.util.function.Consumer;

import static java.lang.Thread.interrupted;
import static jssc.SerialPort.*;

public class FcReader {
    private SerialPort port;
    private Thread readerThread;
    private Consumer<Short> valueConsumer;

    public FcReader(final Consumer<Short> valueConsumer) throws SerialPortException, AWTException {
        this.valueConsumer = valueConsumer;
        port = new SerialPort("/dev/ttyUSB0");
        port.openPort();
        port.setParams(BAUDRATE_9600, DATABITS_8, STOPBITS_1, PARITY_NONE);

        readerThread = new Thread(new ReaderThread());
        readerThread.start();


        Robot robot = new Robot();
        new Thread(() -> {
            try {
                while (!Thread.interrupted()) {
                    robot.keyPress(KeyEvent.VK_RIGHT);
                    Thread.sleep(1500);
                    robot.keyRelease(KeyEvent.VK_RIGHT);
                    robot.keyPress(KeyEvent.VK_LEFT);
                    Thread.sleep(1500);
                    robot.keyRelease(KeyEvent.VK_LEFT);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }).start();
    }

    public void close() throws SerialPortException {
        port.closePort();
        readerThread.interrupt();
    }

    private class ReaderThread implements Runnable {
        @Override
        public void run() {
            try {
                while (!interrupted()) {
                    byte[] value =  port.readBytes(2);
                    byte upper = value[0];
                    byte lower = value[1];

                    if (isFc(upper)) {
                        if (isFc(lower)) {
                            continue;
                        } else {
                            upper = lower;
                            lower = port.readBytes(1)[0];
                        }
                    }
                    valueConsumer.accept(ByteBuffer.wrap(new byte[]{upper, lower}).getShort());
                    Thread.sleep(10);
                }
            } catch (SerialPortException | InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private boolean isFc(byte value) {
        int firstDigit = toDigit("FC".charAt(0));
        int secondDigit = toDigit("FC".charAt(1));
        return value == (byte) ((firstDigit << 4) + secondDigit);
    }

    private int toDigit(char hexChar) {
        int digit = Character.digit(hexChar, 16);
        if(digit == -1) {
            throw new IllegalArgumentException(
                    "Invalid Hexadecimal Character: "+ hexChar);
        }
        return digit;
    }
}
