package factory;

import factory.production.Car;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.time.LocalDateTime;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicInteger;

public class Dealer {
    private Thread thread;

    private final Warehouse<Car> warehouse;
    private AtomicInteger delay = new AtomicInteger(1000);
    private int dealerNo;
    private int carsSold = 0;

    private boolean logSales = true;
    private final static String logFilename = "log.txt";
    private final static String logFormatLine = "<%s>:factory.Dealer<%d>:Auto<%s>(Body:<%s>, Motor:<%s>, Accessory:<%s>)";

    public Dealer(int dealerNo, Warehouse<Car> warehouse) {
        this.warehouse = Objects.requireNonNull(warehouse, "warehouse is null");
        this.dealerNo = dealerNo;
    }

    public void setLogging(boolean logSales) {
        this.logSales = logSales;
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

    public void startSales() {
        // TODO: open logfile only if logTrue is set!!!
        thread = new Thread(() -> {
            try (PrintWriter logfile = new PrintWriter(new FileOutputStream(logFilename))) {
                while (!Thread.currentThread().isInterrupted()) {
                    Thread.sleep(getDelay());
                    Car car = warehouse.getItem();
                    ++carsSold;
                    if (logSales) {
                        writeLogs(logfile, car);
                    }
                }
            } catch (InterruptedException e) {
                System.out.println(String.format("factory.Dealer № %d has finished sales", dealerNo));
            } catch (IOException e) {
                e.printStackTrace();
            }
        });
        thread.start();
    }

    public void stopSales() {
        thread.interrupt();
    }

    public int getCarsSold() {
        return carsSold;
    }

    private void writeLogs(PrintWriter logfile, Car car) {
        String time = LocalDateTime.now().toString();
        String logLine = String.format(logFormatLine, time, dealerNo, car.getID(),
                car.getBodyID(), car.getEngineID(), car.getAccessoryID());
        logfile.println(logLine);
    }
}
