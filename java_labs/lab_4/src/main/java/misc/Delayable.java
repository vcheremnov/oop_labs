package misc;

import java.util.concurrent.atomic.AtomicInteger;

public abstract class Delayable extends Observable {
    private AtomicInteger delay = new AtomicInteger(1000);

    static public class Property extends Observable.Property {
        public static final Property DELAY = new Property("DELAY");

        protected Property(String name) {
            super(name);
        }
    }

    public void setDelay(int millis) {
        if (millis < 0) {
            throw new IllegalArgumentException("Negative delay was passed");
        }
        int oldValue = delay.getAndSet(millis);
        firePropertyChanged(Property.DELAY, oldValue, millis);
    }

    public int getDelay() {
        return delay.get();
    }
}
