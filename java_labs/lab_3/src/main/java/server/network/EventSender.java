package server.network;

import misc.network.ClientSession;

import java.io.IOException;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

class EventSender implements Runnable {
    private long currentEventNumber;
    private Set<ClientSession> clientSessions = Collections.newSetFromMap(new ConcurrentHashMap<>());
    private final Map<Long, String> eventMessages = new HashMap<>();

    EventSender(long startingEventNumber) {
        currentEventNumber = startingEventNumber;
    }

    @Override
    public void run() {
        while (!Thread.currentThread().isInterrupted()) {
            try {
                String eventMessage = retrieveNextEvent();
                for (ClientSession session: clientSessions) {
                    try {
                        String sessionID = session.getSessionID();
                        long startingEventNumber = session.getStartingEventNumber();
                        if (sessionID != null && startingEventNumber <= currentEventNumber) {
                            session.writeMessage(eventMessage);
                        }
                    } catch (IOException ex) {
                        clientSessions.remove(session);
                        ex.printStackTrace();
                    }
                }
                ++currentEventNumber;
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                e.printStackTrace();
            }
        }
    }

    void addSession(ClientSession session) {
        clientSessions.add(session);
    }

    void removeSession(ClientSession session) {
        clientSessions.remove(session);
    }

    void submitEventMessage(String eventMessage, long serialNumber) {
        synchronized (eventMessages) {
            eventMessages.put(serialNumber, eventMessage);
            eventMessages.notify();
        }
    }

    private String retrieveNextEvent() throws InterruptedException {
        synchronized (eventMessages) {
            while (!eventMessages.containsKey(currentEventNumber)) {
                eventMessages.wait();
            }

            return eventMessages.remove(currentEventNumber);
        }
    }
}
