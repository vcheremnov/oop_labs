package factory;

import factory.details.Detail;

import java.util.Objects;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Supplier;

public class Provider<T extends Detail> {
    private Thread thread;
    private final Supplier<? extends T> ctor;
    private final Warehouse<T> warehouse;
    private AtomicInteger delay = new AtomicInteger(1000);
    private int detailsProduced = 0;

    public Provider(Warehouse<T> warehouse, Supplier<? extends T> ctor) {
        this.warehouse = Objects.requireNonNull(warehouse, "warehouse is null");
        this.ctor = Objects.requireNonNull(ctor, "detail constructor is null");
    }

    public void startSupplies() {
        thread = new Thread(() -> {
            try {
                while (!Thread.currentThread().isInterrupted()) {
                    Thread.sleep(getDelay());
                    T detail = produceDetail();
                    warehouse.putItem(detail);
                }
            } catch (InterruptedException e) {
                System.out.println("Provider has finished supplies");
            }
        });
        thread.start();
    }

    public void stopSupplies() {
        thread.interrupt();
    }

    public void setDelay(int millis) {
        if (millis < 0) {
            throw new IllegalArgumentException("Negative delay was passed");
        }
        delay.set(millis);
    }

    public int getDelay() {
        return delay.get();
    }

    private T produceDetail() {
        ++detailsProduced;
        return ctor.get();
    }

    public int getDetailsProduced() {
        return detailsProduced;
    }
}
