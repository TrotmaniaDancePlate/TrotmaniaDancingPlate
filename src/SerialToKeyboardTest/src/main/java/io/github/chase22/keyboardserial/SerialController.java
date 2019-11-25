package io.github.chase22.keyboardserial;

import io.github.chase22.keyboardserial.ui.SerialUiInterface;

import java.awt.*;

public class SerialController {
    private final Robot robot;
    private final int key;
    private final SerialUiInterface serialUiInterface;

    public SerialController(final int key, final SerialUiInterface serialUiInterface) throws AWTException {
        this.robot = new Robot();
        this.key = key;
        this.serialUiInterface = serialUiInterface;
    }

    public void newValue(short value) {
        serialUiInterface.newValue(value);

        if (value > serialUiInterface.getThreshold() && serialUiInterface.isActive()) {
            robot.keyPress(key);
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally {
                robot.keyRelease(key);
            }
        }
    }
}
