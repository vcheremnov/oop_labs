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

    private AtomicInteger idGenerator = new AtomicInteger(0);
    private int dealersNumber;
    private ThreadPool dealersPool;
    private Runnable dealerTask;

    private boolean isLogging = true;
    private static final Logger logger = LogManager.getLogger();
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
        this.dealersNumber = dealersNumber;
        dealersPool = new FixedThreadPool(dealersNumber);

        dealerTask = () -> {
            int dealerNo = idGenerator.getAndIncrement();
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    Car car = this.factory.getCarWarehouse().takeItem();
                    Thread.sleep(getDelay());

                    int oldCarsSold = carsSold.getAndIncrement();
                    firePropertyChanged(Property.CARS_SOLD, oldCarsSold, carsSold.get());

                    if (isLogging) {
                        writeLogs(car, dealerNo);
                    }
                } catch (InterruptedException e) {
                    break;
                }
            }
            idGenerator.decrementAndGet();
        };
    }

    public void startSales() {
        for (int i = 0; i < dealersNumber; ++i) {
            dealersPool.submit(dealerTask);
        }
    }

    public void stopSales() {
        dealersPool.shutdownNow();
    }

    public int getDealersNumber() {
        return dealersNumber;
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
