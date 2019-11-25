package io.github.chase22.keyboardserial.ui;

import jexer.TAction;
import jexer.TField;
import jexer.TWindow;

public class SpinnerField {
    private TField field;

    SpinnerField(TWindow parent, int x, int y, int initial) {
        field = parent.addField(x, y, 5, true, Integer.toString(initial));

        field.setUpdateAction(new TAction() {
            @Override
            public void DO() {
                field.setText(field.getText().replaceAll("[^\\d.]", ""));
            }
        });

        parent.addSpinner(x + 5, y, new SpinnerUpAction(1000), new SpinnerDownAction(1000));
        parent.addSpinner(x + 7, y, new SpinnerUpAction(100), new SpinnerDownAction(100));
        parent.addSpinner(x + 9, y, new SpinnerUpAction(10), new SpinnerDownAction(10));
        parent.addSpinner(x + 11, y, new SpinnerUpAction(1), new SpinnerDownAction(1));
    }

    public int getValue() {
        try {
            return Integer.parseInt(field.getText());
        } catch (NumberFormatException e) {
            return 0;
        }
    }

    public void setValue(int value) {
        field.setText(Integer.toString(value));
    }


    private class SpinnerUpAction extends TAction {
        private final int changeValue;

        private SpinnerUpAction(final int changeValue) {
            this.changeValue = changeValue;
        }

        @Override
        public void DO() {
            setValue(getValue() + changeValue);
        }
    }

    private class SpinnerDownAction extends TAction {
        private final int changeValue;

        private SpinnerDownAction(final int changeValue) {
            this.changeValue = changeValue;
        }

        @Override
        public void DO() {
            setValue(getValue() - changeValue);
        }
    }
}
