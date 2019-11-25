package io.github.chase22.keyboardserial.ui;

import jexer.*;

import java.io.UnsupportedEncodingException;

public class Application extends TApplication implements SerialUiInterface {

    private final TLabel testLabel;
    private final SpinnerField thresholdField;

    public Application() throws UnsupportedEncodingException {
        super(BackendType.XTERM);
        addFileMenu();

        TWindow test = addWindow("Test", 200, 150);
        testLabel = test.addLabel("test", 1, 1);
        thresholdField = new SpinnerField(test, 1, 2, 1024);

    }

    @Override
    public void newValue(final short value) {
        testLabel.setLabel(Short.toString(value));
    }

    @Override
    public int getThreshold() {
        return thresholdField.getValue();
    }
}