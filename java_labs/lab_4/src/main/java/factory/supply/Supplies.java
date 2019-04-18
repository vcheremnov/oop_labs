package factory.supply;

import factory.Warehouse;
import factory.details.Detail;
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

    private Thread thread;
    private ThreadPool suppliersPool;
    private Runnable supplierTask;

    static public class Property extends Delayable.Property {
        static public final Property DETAILS_PRODUCED = new Property("DETAILS_PRODUCED");

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

        suppliersPool = new FixedThreadPool(suppliersNumber);
        supplierTask = () -> {
            try {
                Thread.sleep(getDelay());
                T detail = this.ctor.get();
                this.warehouse.putItem(detail);

                int oldDetailsProduced = detailsProduced.getAndIncrement();
                firePropertyChanged(Property.DETAILS_PRODUCED, oldDetailsProduced, detailsProduced.get());
            } catch (InterruptedException e) {
                System.out.println("Provider has finished supply");
            }
        };
    }

    public void startSupplies() {
        thread = new Thread(() -> {
            try {
                while (!Thread.currentThread().isInterrupted()) {
                    int itemsNumber = warehouse.getItemsNumber();
                    int capacity = warehouse.getCapacity();
                    int requiredItems = capacity - itemsNumber;

                    for (int i = 0; i < requiredItems; i++) {
                        suppliersPool.submit(supplierTask);
                    }

                    warehouse.wait();
                }
            } catch (InterruptedException e) {
                System.out.println("Provider has finished supplies");
            }
        });
        thread.start();
    }

    public void stopSupplies() {
        if (thread != null) {
            thread.interrupt();
        }
        suppliersPool.shutdownNow();
    }

    public int getDetailsProduced() {
        return detailsProduced.get();
    }
}
