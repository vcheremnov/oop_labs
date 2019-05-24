package client.model.session;

import client.model.misc.Observable;
import protocol.model.ChatMessage;

import java.beans.PropertyChangeEvent;
import java.util.*;
import java.util.concurrent.atomic.AtomicReference;

public class ChatSessionData extends Observable {
    private AtomicReference<String> username = new AtomicReference<>();
    private AtomicReference<String> sessionID = new AtomicReference<>();

    private NavigableSet<String> users = new TreeSet<>();
    private Set<String> usersToRemove = new HashSet<>();

    private NavigableSet<ChatMessage> messages = new TreeSet<>();

    public static class Property extends Observable.Property {
        public static final Property MESSAGE_ADDED = new Property("MESSAGE_ADDED");
        public static final Property USER_ADDED = new Property("USER_ADDED");
        public static final Property USER_REMOVED = new Property("USER_REMOVED");
        public static final Property SESSION_ESTABLISHED = new Property("SESSION_ESTABLISHED");

        protected Property(String name) {
            super(name);
        }
    }

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
        firePropertyChanged(Property.SESSION_ESTABLISHED, false, true);
    }


    public synchronized int getMessagesNumber() {
        return messages.size();
    }

    public synchronized void putMessage(ChatMessage chatMessage) {
        messages.add(chatMessage);
        firePropertyChanged(Property.MESSAGE_ADDED, false, true);
    }

    public synchronized void putMessages(Collection<? extends ChatMessage> chatMessages) {
        messages.addAll(chatMessages);
        firePropertyChanged(Property.MESSAGE_ADDED, false, true);
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
        firePropertyChanged(Property.USER_ADDED, false, true);
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
        firePropertyChanged(Property.USER_REMOVED, false, true);
    }
}
