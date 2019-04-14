package factory.production;

import factory.Warehouse;

import java.util.Objects;

public class ProductionController {
    private Thread thread;
    private final Warehouse<Car> warehouse;
    private final CarFactory carFactory;

    public ProductionController(CarFactory carFactory, Warehouse<Car> warehouse) {
        this.carFactory = Objects.requireNonNull(carFactory, "Car carFactory is null");
        this.warehouse = Objects.requireNonNull(warehouse, "Warehouse is null");
    }

    public void startSupervision() {
        thread = new Thread(() -> {
            try {
                synchronized (warehouse) {
                    while (!Thread.currentThread().isInterrupted()) {
                        int emptyPlaces = warehouse.getCapacity() - warehouse.getItemsNumber();
                        int incomingCars = carFactory.incomingCarsNumber();
                        int requiredCars = emptyPlaces - incomingCars;

                        if (requiredCars > 0) {
                            carFactory.produceCars(requiredCars);
                        }

                        warehouse.wait();
                    }
                }
            } catch (InterruptedException e) {
                System.out.println("Production controller has been stopped");
            }
        });
        thread.start();
    }

    public void stopSupervision() {
        thread.interrupt();
    }
}
