package threadpool;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

public class FixedThreadPool implements ThreadPool {
    private BlockingQueue<Runnable> tasks;
    private PoolWorker[] workers;

    private AtomicBoolean isRunning = new AtomicBoolean(true);
    private AtomicBoolean isShutdown = new AtomicBoolean(false);
    private AtomicInteger activeWorkersCount = new AtomicInteger();

    private final Object awaitLock = new Object();

    public FixedThreadPool(int numThreads) {
        if (numThreads <= 0) {
            throw new IllegalArgumentException("Number of threads less than or equal to zero was passed");
        }

        tasks = new LinkedBlockingQueue<>();
        workers = new PoolWorker[numThreads];
        activeWorkersCount.set(numThreads);
        for (int threadIndex = 0; threadIndex < numThreads; ++threadIndex) {
            workers[threadIndex] = new PoolWorker();
            workers[threadIndex].start();
        }
    }

    @Override
    public void submit(Runnable task) {
        if (isShutdown.get()) {
            throw new IllegalStateException("Thread pool was terminated");
        }

        if (task == null) {
            throw new NullPointerException("Task is null");
        }

        boolean isSubmitted = tasks.offer(task);
        if (!isSubmitted) {
            throw new RejectedExecutionException("Not enough space in the task queue");
        }
    }

    @Override
    public void shutdown() {
        isShutdown.set(true);
    }

    @Override
    public List<Runnable> shutdownNow() {
        shutdown();

        List<Runnable> taskList = new ArrayList<>(tasks.size());
        tasks.drainTo(taskList);

        for (PoolWorker worker: workers) {
            worker.interrupt();
        }

        return taskList;
    }

    @Override
    public boolean isShutdown() {
        return isShutdown.get();
    }

    @Override
    public boolean isTerminating() {
        return isShutdown.get() && isRunning.get();
    }

    @Override
    public boolean isTerminated() {
        return !isRunning.get();
    }

    @Override
    public void awaitTermination() throws InterruptedException {
        synchronized (awaitLock) {
            while (isRunning.get()) {
                awaitLock.wait();
            }
        }
    }

    @Override
    public void awaitTermination(long timeout) throws InterruptedException {
        // TODO: add loop with "wait(..)" where timeout decrements with time
        synchronized (awaitLock) {
            if (isRunning.get()) {
                awaitLock.wait(timeout);
            }
        }
    }

    private class PoolWorker extends Thread {
        @Override
        public void run() {
            while (!isInterrupted()) {
                try {
                    Runnable task = isShutdown() ? tasks.poll() : tasks.take();
                    if (task == null) {
                        break;
                    }
                    task.run();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                } catch (RuntimeException e) {
                    System.err.println("Task has failed:");
                    e.printStackTrace();
                }
            }

            finishWork();
            System.err.printf("Pool worker \"%s\" has been interrupted\n", this.getName());
        }

        private void finishWork() {
            if (activeWorkersCount.decrementAndGet() == 0) {
                synchronized (awaitLock) {
                    isRunning.set(false);
                    awaitLock.notifyAll();
                }
            }
        }
    }
}
