package factory.production;

import factory.Warehouse;

import java.util.Objects;

public class ProductionController {
    private Thread thread;
    private final Warehouse<Car> warehouse;
    private final AssemblyShop assemblyShop;

    public ProductionController(AssemblyShop assemblyShop, Warehouse<Car> warehouse) {
        this.assemblyShop = Objects.requireNonNull(assemblyShop, "Car assemblyShop is null");
        this.warehouse = Objects.requireNonNull(warehouse, "Warehouse is null");
    }

    public void startSupervision() {
        thread = new Thread(() -> {
            try {
                synchronized (warehouse) {
                    while (!Thread.currentThread().isInterrupted()) {
                        int emptyPlaces = warehouse.getCapacity() - warehouse.getItemsNumber();
                        int incomingCars = assemblyShop.getCarsRequested();
                        int requiredCars = emptyPlaces - incomingCars;

                        if (requiredCars > 0) {
                            assemblyShop.produceCars(requiredCars);
                        }

                        warehouse.wait();
                    }
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        thread.start();
    }

    public void stopSupervision() {
        if (thread != null) {
            thread.interrupt();
        }
    }
}
