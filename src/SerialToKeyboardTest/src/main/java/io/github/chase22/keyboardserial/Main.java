package io.github.chase22.keyboardserial;

import io.github.chase22.keyboardserial.serial.FcReader;
import io.github.chase22.keyboardserial.ui.Application;
import jssc.SerialPortException;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.io.IOException;

public class Main {
    public static void main(String[] args) throws SerialPortException, IOException, AWTException {
        Application app = new Application();
        SerialController controller = new SerialController(KeyEvent.VK_UP, app);

        FcReader fcReader = new FcReader(controller::newValue);

        app.run();
        
        fcReader.close();
    }
}
