package factory.supply;

import factory.Warehouse;
import misc.Delayable;
import threadpool.FixedThreadPool;
import threadpool.ThreadPool;

import java.util.Objects;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Supplier;

public class Supplies<T> extends Delayable {
    private final Supplier<? extends T> ctor;
    private final Warehouse<T> warehouse;
    private AtomicInteger detailsProduced = new AtomicInteger(0);

    private int suppliersNumber;
    private ThreadPool suppliersPool;
    private Runnable supplierTask;

    static public class Property extends Delayable.Property {
        public static final Property DETAILS_PRODUCED = new Property("DETAILS_PRODUCED");

        protected Property(String name) {
            super(name);
        }
    }

    public Supplies(int suppliersNumber, Warehouse<T> warehouse, Supplier<? extends T> ctor) {
        this.warehouse = Objects.requireNonNull(warehouse, "Warehouse is null");
        this.ctor = Objects.requireNonNull(ctor, "Detail constructor is null");
        if (suppliersNumber < 1) {
            throw new IllegalArgumentException("Number of suppliers is less than 1");
        }

        this.suppliersNumber = suppliersNumber;
        suppliersPool = new FixedThreadPool(suppliersNumber);
        supplierTask = () -> {
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    Thread.sleep(getDelay());
                    T detail = this.ctor.get();
                    this.warehouse.putItem(detail);

                    int oldDetailsProduced = detailsProduced.getAndIncrement();
                    firePropertyChanged(Property.DETAILS_PRODUCED, oldDetailsProduced, detailsProduced.get());
                } catch (InterruptedException e) {
                    break;
                }
            }
        };
    }

    public void startSupplies() {
        for (int i = 0; i < suppliersNumber; i++) {
            suppliersPool.submit(supplierTask);
        }
    }

    public void stopSupplies() {
        suppliersPool.shutdownNow();
    }

    public int getSuppliersNumber() {
        return suppliersNumber;
    }

    public int getDetailsProduced() {
        return detailsProduced.get();
    }
}
