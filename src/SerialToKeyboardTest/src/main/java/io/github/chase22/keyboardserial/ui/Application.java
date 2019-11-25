package io.github.chase22.keyboardserial.ui;

import jexer.*;

import java.io.UnsupportedEncodingException;

public class Application extends TApplication implements SerialUiInterface {

    private final TLabel testLabel;
    private final SpinnerField thresholdField;
    private final TCheckBox activeCheckbox;

    public Application() throws UnsupportedEncodingException {
        super(BackendType.XTERM);
        addFileMenu();

        TWindow test = addWindow("Test", 100, 10);

        activeCheckbox = test.addCheckBox(1, 1, "Active", true);
        testLabel = test.addLabel("test", 1, 2);
        thresholdField = new SpinnerField(test, 1, 3, 230);

    }

    @Override
    public void newValue(final short value) {
        testLabel.setLabel(Short.toString(value));
    }

    @Override
    public int getThreshold() {
        return thresholdField.getValue();
    }

    @Override
    public boolean isActive() {
        return activeCheckbox.isChecked();
    }
}