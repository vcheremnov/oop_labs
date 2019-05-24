package protocol.events;

import protocol.Message;
import protocol.MessageType;

import java.util.concurrent.atomic.AtomicLong;

public class Event extends Message implements Comparable<Event> {
    private static final AtomicLong serialNumberGenerator = new AtomicLong(0);
    private long serialNumber;
    private String eventType;

    public Event(EventType eventType) {
        super(MessageType.MESSAGE_EVENT);
        this.eventType = eventType.toString();
        serialNumber = serialNumberGenerator.getAndIncrement();
    }

    public String getEventType() {
        return eventType;
    }

    public long getSerialNumber() {
        return serialNumber;
    }

    public static long getNextSerialNumber() {
        return serialNumberGenerator.get();
    }

    @Override
    public int compareTo(Event event) {
        return Long.compare(serialNumber, event.serialNumber);
    }
}
