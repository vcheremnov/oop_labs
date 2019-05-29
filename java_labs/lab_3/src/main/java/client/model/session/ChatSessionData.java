package client.model.session;

import protocol.model.ChatMessage;

import java.util.*;
import java.util.concurrent.atomic.AtomicReference;

public class ChatSessionData {
    private AtomicReference<String> username = new AtomicReference<>();
    private AtomicReference<String> sessionID = new AtomicReference<>();

    private NavigableSet<String> users = new TreeSet<>();
    private Set<String> usersToRemove = new HashSet<>();

    private NavigableSet<ChatMessage> messages = new TreeSet<>();


    public String getUsername() {
        return username.get();
    }

    public void setUsername(String username) {
        this.username.set(username);
    }


    public String getSessionID() {
        return sessionID.get();
    }

    public void setSessionID(String sessionID) {
        this.sessionID.set(sessionID);
    }


    public synchronized int getMessagesNumber() {
        return messages.size();
    }

    public synchronized void putMessage(ChatMessage chatMessage) {
        messages.add(chatMessage);
    }

    public synchronized void putMessages(Collection<? extends ChatMessage> chatMessages) {
        messages.addAll(chatMessages);
    }

    public synchronized ChatMessage[] getMessages() {
        return messages.toArray(new ChatMessage[0]);
    }

    public synchronized ChatMessage[] getMessages(int fromIndex, int toIndex) {
        List<ChatMessage> messageList = new ArrayList<>(messages);
        List<ChatMessage> messageSublist = messageList.subList(fromIndex, toIndex);
        return messageSublist.toArray(new ChatMessage[0]);
    }


    public synchronized int getUsersNumber() {
        return users.size();
    }

    public synchronized String[] getUsers() {
        return users.toArray(new String[0]);
    }

    public synchronized String[] getUsers(int fromIndex, int toIndex) {
        List<String> userList = new ArrayList<>(users);
        List<String> userSublist = userList.subList(fromIndex, toIndex);
        return userSublist.toArray(new String[0]);
    }

    public synchronized void addUser(String user) {
        if (usersToRemove.contains(user)) {
            usersToRemove.remove(user);
            return;
        }
        users.add(user);
    }

    public synchronized void addUsers(Collection<? extends String> users) {
        for (String user: users) {
            addUser(user);
        }
    }

    public synchronized void removeUser(String user) {
        if (!users.contains(user)) {
            usersToRemove.add(user);
            return;
        }
        users.remove(user);
    }
}
