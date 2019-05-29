package client.model.session;

import client.model.misc.Observable;
import client.model.network.ErrorAction;
import client.model.network.MessageReceiver;
import client.model.network.RequestSender;
import misc.network.ClientSession;
import protocol.commands.request.Request;
import protocol.commands.types.chatmessage.ChatMessageRequest;
import protocol.commands.types.login.LoginRequest;
import protocol.commands.types.logout.LogoutRequest;

import java.io.IOException;
import java.net.Socket;

public class ChatSession extends Observable {
    private ChatSessionData sessionData;
    private RequestSender requestSender;
    private MessageReceiver messageReceiver;
    private Thread messageReceiverThread;
    private ClientSession session;

    static public class Property extends Observable.Property {
        public static final Property CONNECTED = new Property("CONNECTED");
        public static final Property DISCONNECTED = new Property("DISCONNECTED");
        public static final Property LOGGED_IN = new Property("LOGGED_IN");
        public static final Property LOGGED_OUT = new Property("LOGGED_OUT");
        public static final Property USER_LOGIN = new Property("USER_LOGIN");
        public static final Property USER_LOGOUT = new Property("USER_LOGOUT");
        public static final Property SENT_MESSAGE = new Property("SENT_MESSAGE");
        public static final Property GOT_MESSAGE = new Property("GOT_MESSAGE");
        public static final Property GOT_INFO = new Property("GOT_INFO");

        protected Property(String name) {
            super(name);
        }

    }

    public ClientSession getSession() {
        return session;
    }

    public ChatSessionData getSessionData() {
        return sessionData;
    }

    public void connect(String address, int port) throws IOException {
        Socket socket = new Socket(address, port);
        session = new ClientSession(socket);
        sessionData = null;
        requestSender = new RequestSender(session);

        messageReceiver = new MessageReceiver(this);
        messageReceiverThread = new Thread(messageReceiver);
        messageReceiverThread.setDaemon(true);
        messageReceiverThread.start();

        notifyObservers(Property.CONNECTED, null, true);
    }

    public void disconnect() {
        if (messageReceiverThread != null) {
            messageReceiverThread.interrupt();
        }
        if (session != null) {
            session.close();
        }
    }

    public void startSession(String username, ErrorAction errorAction) throws IOException {
        sessionData = new ChatSessionData();
        sessionData.setUsername(username);
        LoginRequest request = new LoginRequest();
        request.setUsername(username);
        sendRequest(request, errorAction);
    }

    public void sendChatMessage(String message, ErrorAction errorAction) throws IOException {
        ChatMessageRequest request = new ChatMessageRequest();
        request.setMessage(message);
        sendRequest(request, errorAction);
    }

    public void closeSession(ErrorAction errorAction) throws IOException {
        LogoutRequest request = new LogoutRequest();
        sendRequest(request, errorAction);
        disconnect();
    }

    public void notifyObservers(Property changedProperty, Object oldValue, Object newValue) {
        firePropertyChanged(changedProperty, oldValue, newValue);
    }

    private void sendRequest(Request request, ErrorAction errorAction) throws IOException {
        String sessionID = sessionData.getSessionID();
        request.setSessionID(sessionID);
        messageReceiver.setNextResponseError(errorAction);
        requestSender.sendRequest(request);
    }
}
