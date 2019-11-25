package io.github.chase22.keyboardserial.ui;

public interface SerialUiInterface {

    void newValue(short value);

    boolean isActive();
    int getThreshold();
}
