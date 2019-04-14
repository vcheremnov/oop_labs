import factory.CarManufacturing;
import factory.exceptions.FactoryLoadingException;
import threadpool.FixedThreadPool;
import threadpool.ThreadPool;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class Main {
    public static void main(String[] args) {
        ThreadPool pool = new FixedThreadPool(3);
        for (int i = 0; i < 10; i++) {
            pool.submit(() -> {
                try {
                    Thread.sleep(1000);
                    System.out.println(Thread.currentThread().getName() + " has finished");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            });
        }

        pool.shutdown();
        System.out.println("shutdown: " + pool.isShutdown());
        System.out.println("terminating: " + pool.isTerminating());
        System.out.println("terminated: " + pool.isTerminated());
        try {
            pool.awaitTermination(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("terminated: " + pool.isTerminated());

        System.out.println("Main thread exited");
    }
}
