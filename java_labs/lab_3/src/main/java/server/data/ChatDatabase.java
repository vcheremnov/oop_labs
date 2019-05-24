package server.data;

import protocol.model.ChatMessage;

import java.time.Instant;
import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

public class ChatDatabase {
    private AtomicReference<String> serverName = new AtomicReference<>("Chat room");
    private AtomicInteger maxClientsNumber = new AtomicInteger(10);

    private static final int LAST_MESSAGES_NUMBER = 10;
    private final Deque<ChatMessage> lastMessages = new ArrayDeque<>(LAST_MESSAGES_NUMBER);

    private final Set<String> clientsNames = new HashSet<>();
    private final Map<String, String> clientsSessions = new HashMap<>();


    public String getServerName() {
        return serverName.get();
    }

    public void setServerName(String serverName) {
        this.serverName.set(serverName);
    }


    public void setMaxClientsNumber(int clientsNumber) {
        maxClientsNumber.set(clientsNumber);
    }

    public int getMaxClientsNumber() {
        return maxClientsNumber.get();
    }

    public synchronized int getClientsNumber() {
        return clientsNames.size();
    }

    public synchronized boolean chatRoomIsFull() {
        return clientsNames.size() == maxClientsNumber.get();
    }


    public synchronized String addClient(String clientName) {
        String sessionID = UUID.randomUUID().toString();
        clientsNames.add(clientName);
        clientsSessions.put(sessionID, clientName);

        return sessionID;
    }

    public synchronized String removeClient(String sessionID) {
        String clientName = clientsSessions.remove(sessionID);
        if (clientName != null) {
            clientsNames.remove(clientName);
        }

        return clientName;
    }


    public synchronized List<String> getClientsNames() {
        return new ArrayList<>(clientsNames);
    }

    public synchronized boolean clientExists(String clientName) {
        return clientsNames.contains(clientName);
    }

    public synchronized boolean sessionExists(String sessionID) {
        return clientsSessions.containsKey(sessionID);
    }


    public synchronized List<ChatMessage> getLastMessages() {
        return new ArrayList<>(lastMessages);
    }

    public synchronized ChatMessage putMessage(String sessionID, String message) {
        String clientName = clientsSessions.get(sessionID);
        long date = Instant.now().toEpochMilli();

        ChatMessage chatMessage = new ChatMessage();
        chatMessage.setDate(date);
        chatMessage.setSenderName(clientName);
        chatMessage.setMessage(message);

        if (lastMessages.size() == LAST_MESSAGES_NUMBER) {
            lastMessages.removeFirst();
        }
        lastMessages.addLast(chatMessage);

        return chatMessage;
    }
}
