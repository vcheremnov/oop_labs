package factory.sales;

import factory.Factory;
import factory.production.Car;
import misc.Delayable;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import threadpool.FixedThreadPool;
import threadpool.ThreadPool;

import java.util.concurrent.atomic.AtomicInteger;

public class SalesDepartment extends Delayable {
    private final Factory factory;
    private AtomicInteger carsSold = new AtomicInteger(0);

    private Thread thread;
    private ThreadPool dealersPool;
    private Runnable dealerTask;

    private boolean isLogging = true;
    private static final Logger logger = LogManager.getLogger(SalesDepartment.class);
    private final static String logFormatLine = "Dealer<%d>:Auto<%s>(Body:<%s>, Motor:<%s>, Accessory:<%s>)";

    static public class Property extends Delayable.Property {
        public static final Property CARS_SOLD = new Property("CARS_SOLD");

        protected Property(String name) {
            super(name);
        }
    }

    public SalesDepartment(int dealersNumber, Factory factory) {
        if (dealersNumber < 1) {
            throw new IllegalArgumentException("Number of dealers is less than 1");
        }

        this.factory = factory;
        dealersPool = new FixedThreadPool(dealersNumber);

        dealerTask = () -> {
            try {
                Car car = this.factory.getCarWarehouse().takeItem();
                Thread.sleep(getDelay());

                int oldCarsSold = carsSold.getAndIncrement();
                firePropertyChanged(Property.CARS_SOLD, oldCarsSold, carsSold.get());

                if (isLogging) {
                    writeLogs(car, Thread.currentThread().getId());
                }
            } catch (InterruptedException e) {
                System.err.println("Dealer task has been interrupted");
            }
        };
    }

    public void startSales() {
        thread = new Thread(() -> {
            try {
                while (!Thread.currentThread().isInterrupted()) {
                    int carsAvailable = factory.getCarWarehouse().getItemsNumber();
                    for (int i = 0; i < carsAvailable; i++) {
                        dealersPool.submit(dealerTask);
                    }
                    factory.getCarWarehouse().wait();
                    // TODO: can this thread wait forever if it is interrupted before?
                }
            } catch (InterruptedException e) {
                System.out.println("Sales were interrupted");
            }
        });
        thread.start();
    }

    public void stopSales() {
        if (thread != null) {
            thread.interrupt();
        }
        dealersPool.shutdownNow();
    }

    public int getCarsSold() {
        return carsSold.get();
    }

    public void setLogging(boolean isLogging) {
        this.isLogging = isLogging;
    }

    private void writeLogs(Car car, long dealerNo) {
        String message = String.format(logFormatLine, dealerNo, car.getID(),
                car.getBodyID(), car.getEngineID(), car.getAccessoryID());
        logger.info(message);
    }
}
