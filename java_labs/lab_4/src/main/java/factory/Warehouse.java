package factory;

import java.util.ArrayDeque;
import java.util.Queue;

public class Warehouse <T> {
    private Queue<T> items;
    private int capacity;

    public Warehouse(int capacity) {
        if (capacity < 1) {
            throw new IllegalArgumentException("Negative or zero capacity has been passed");
        }

        this.capacity = capacity;
        items = new ArrayDeque<>(capacity);
    }

    public synchronized void putItem(T item) throws InterruptedException {
        while (items.size() == capacity) {
            wait();
        }

        items.add(item);
        notify();
    }

    public synchronized T getItem() throws InterruptedException {
        while (items.isEmpty()) {
            wait();
        }

        T item = items.remove();
        notifyAll();

        return item;
    }

    public int getItemsNumber() {
        return items.size();
    }

    public int getCapacity() {
        return capacity;
    }

}
