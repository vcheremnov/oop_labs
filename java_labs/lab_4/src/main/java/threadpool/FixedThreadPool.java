package threadpool;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
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

    private final Object awaitBlock = new Object();

    public FixedThreadPool(int numThreads) {
        if (numThreads <= 0) {
            throw new IllegalArgumentException("number of thread less than or equal to zero was passed");
        }

        tasks = new LinkedBlockingQueue<>();
        workers = new PoolWorker[numThreads];
        activeWorkersCount.set(numThreads);
        for (int threadIndex = 0; threadIndex < numThreads; threadIndex++) {
            workers[threadIndex] = new PoolWorker();
            workers[threadIndex].start();
        }
    }

    @Override
    public void submit(Runnable task) {
        if (isShutdown.get()) {
            throw new IllegalStateException("thread pool was terminated");
        }

        if (task == null) {
            throw new NullPointerException("task is null");
        }

        boolean isSubmitted = tasks.offer(task);
        if (!isSubmitted) {
            throw new RejectedExecutionException("not enough space in the task queue");
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
        synchronized (awaitBlock) {
            while (isRunning.get()) {
                awaitBlock.wait();
            }
        }
    }

    @Override
    public void awaitTermination(long timeout) throws InterruptedException {
        // TODO: add loop with "wait(..)" where timeout decrements with time
        synchronized (awaitBlock) {
            if (isRunning.get()) {
                awaitBlock.wait(timeout);
            }
        }
    }

    private class PoolWorker extends Thread {
        @Override
        public void run() {
            while (!(Thread.currentThread().isInterrupted() || isShutdown() && tasks.isEmpty())) {
                // TODO: find out whether tasks can be emptied before entering try block...
                try {
                    Runnable task = tasks.take();
                    task.run();
                } catch (InterruptedException | RuntimeException e) {
                    e.printStackTrace();
                    Thread.currentThread().interrupt();
                }
            }

            finishWork();
        }

        private void finishWork() {
            if (activeWorkersCount.decrementAndGet() == 0) {
                synchronized (awaitBlock) {
                    isRunning.set(false);
                    awaitBlock.notifyAll();
                }
            }
        }
    }
}
