package factory;

import misc.Observable;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.util.ArrayDeque;
import java.util.Queue;
import java.util.concurrent.atomic.AtomicInteger;

public class Warehouse <T> extends Observable {
    private Queue<T> items;
    private int capacity;
    private AtomicInteger itemsNumber = new AtomicInteger(0);

    static public class Property extends Observable.Property {
        public static final Property ITEMS_NUMBER = new Property("ITEMS_NUMBER");

        protected Property(String name) {
            super(name);
        }
    }

    public Warehouse(int capacity) {
        if (capacity < 1) {
            throw new IllegalArgumentException("Negative or zero capacity has been passed");
        }

        this.capacity = capacity;
        items = new ArrayDeque<>(capacity);
    }

    public synchronized void putItem(T item) throws InterruptedException {
        while (itemsNumber.get() == capacity) {
            wait();
        }

        int oldValue = itemsNumber.getAndIncrement();
        items.add(item);
        notify();

        firePropertyChanged(Property.ITEMS_NUMBER, oldValue, itemsNumber.get());
    }

    public synchronized T takeItem() throws InterruptedException {
        while (items.isEmpty()) {
            wait();
        }

        int oldValue = itemsNumber.getAndDecrement();
        T item = items.remove();
        notifyAll();

        firePropertyChanged(Property.ITEMS_NUMBER, oldValue, itemsNumber.get());

        return item;
    }

    public int getItemsNumber() {
        return itemsNumber.get();
    }

    public int getCapacity() {
        return capacity;
    }
}
