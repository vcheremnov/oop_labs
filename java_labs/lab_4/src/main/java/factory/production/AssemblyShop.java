package factory.production;

import factory.Factory;
import factory.details.Accessory;
import factory.details.Body;
import factory.details.Engine;
import misc.Delayable;
import threadpool.FixedThreadPool;
import threadpool.ThreadPool;

import java.util.concurrent.atomic.AtomicInteger;

public class AssemblyShop extends Delayable {
    private final Factory factory;

    private AtomicInteger carsRequested = new AtomicInteger(0);
    private AtomicInteger carsProduced = new AtomicInteger(0);

    private int workersNumber;
    private ThreadPool workersPool;
    private Runnable workerTask;

    static public class Property extends Delayable.Property {
        public static final Property INCOMING_CARS = new Property("INCOMING_CARS");
        public static final Property CARS_PRODUCED = new Property("CARS_PRODUCED");

        protected Property(String name) {
            super(name);
        }
    }
    public AssemblyShop(int workersNumber, Factory factory) {
        if (workersNumber < 1) {
            throw new IllegalArgumentException("Number of workers is less than 1");
        }

        this.workersNumber = workersNumber;
        this.factory = factory;
        workersPool = new FixedThreadPool(workersNumber);

        workerTask = () -> {
            try {
                Thread.sleep(getDelay());

                Body body = this.factory.getBodyWarehouse().takeItem();
                Engine engine = this.factory.getEngineWarehouse().takeItem();
                Accessory accessory = this.factory.getAccessoryWarehouse().takeItem();

                Car car = new Car(engine, body, accessory);
                this.factory.getCarWarehouse().putItem(car);

                int oldCarsRequested = carsRequested.getAndDecrement();
                firePropertyChanged(Property.INCOMING_CARS, oldCarsRequested, carsRequested.get());

                int oldCarsProduced = carsProduced.getAndIncrement();
                firePropertyChanged(Property.CARS_PRODUCED, oldCarsProduced, carsProduced.get());
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        };
    }

    public void produceCars(int requestedAmount) {
        if (requestedAmount < 0) {
            throw new IllegalArgumentException("Requested amount of cars is less than 0");
        }

        int oldValue = carsRequested.getAndAdd(requestedAmount);
        for (int i = 0; i < requestedAmount; i++) {
            workersPool.submit(workerTask);
        }

        firePropertyChanged(Property.INCOMING_CARS, oldValue, carsRequested.get());
    }

    public int getWorkersNumber() {
        return workersNumber;
    }

    public int getCarsRequested() {
        return carsRequested.get();
    }

    public int getCarsProduced() {
        return carsProduced.get();
    }

    public void stopProduction() {
        workersPool.shutdownNow();
    }
}
