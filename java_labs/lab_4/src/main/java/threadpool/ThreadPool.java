package threadpool;

import java.util.List;

public interface ThreadPool {
    void submit(Runnable task);

    void shutdown();

    List<Runnable> shutdownNow();

    boolean isShutdown();

    boolean isTerminating();

    boolean isTerminated();

    void awaitTermination() throws InterruptedException;

    void awaitTermination(long timeout) throws InterruptedException;
}
