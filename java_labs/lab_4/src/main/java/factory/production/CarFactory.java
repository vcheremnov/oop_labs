package factory.production;

import factory.Warehouse;
import factory.details.Accessory;
import factory.details.Body;
import factory.details.Engine;

import java.util.Objects;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class CarFactory {
    private final Warehouse<Body> bodyWarehouse;
    private final Warehouse<Engine> engineWarehouse;
    private final Warehouse<Accessory> accessoryWarehouse;
    private final Warehouse<Car> carWarehouse;

    private AtomicInteger delay = new AtomicInteger(1000);
    private AtomicInteger incomingCars = new AtomicInteger(0);
    private ExecutorService executorService;
    private Runnable workerTask;

    public CarFactory(int workersNumber, Warehouse<Body> bodyWarehouse, Warehouse<Engine> engineWarehouse,
                      Warehouse<Accessory> accessoryWarehouse, Warehouse<Car> carWarehouse) {
        this.bodyWarehouse      = Objects.requireNonNull(bodyWarehouse, "Body warehouse is null");
        this.engineWarehouse    = Objects.requireNonNull(engineWarehouse, "Engine warehouse is null");
        this.accessoryWarehouse = Objects.requireNonNull(accessoryWarehouse, "Accessory warehouse is null");
        this.carWarehouse       = Objects.requireNonNull(carWarehouse, "Car warehouse is null");

        if (workersNumber < 1) {
            throw new IllegalArgumentException("Number of workers is less than 1");
        }

        executorService = Executors.newFixedThreadPool(workersNumber);
        workerTask = () -> {
            try {
                Thread.sleep(getDelay());

                Body body = bodyWarehouse.getItem();
                Engine engine = engineWarehouse.getItem();
                Accessory accessory = accessoryWarehouse.getItem();

                Car car = new Car(engine, body, accessory);
                carWarehouse.putItem(car);
                incomingCars.decrementAndGet();
            } catch (InterruptedException e) {
                System.err.println("Worker task has been interrupted");
            }
        };
    }

    public void produceCars(int requestedAmount) {
        if (requestedAmount < 0) {
            throw new IllegalArgumentException("Requested amount is less than 0");
        }

        incomingCars.addAndGet(requestedAmount);
        for (int i = 0; i < requestedAmount; i++) {
            executorService.submit(workerTask);
        }
    }

    public int incomingCarsNumber() {
        return incomingCars.get();
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

    public void stopProduction() {
        executorService.shutdownNow();
    }
}
