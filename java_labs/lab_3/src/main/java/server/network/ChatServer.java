package server.network;

import protocol.events.Event;
import server.data.ChatDatabase;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ChatServer {
    private ChatDatabase database;
    private ExecutorService executorService;
    private ServerSocket serverSocket;

    private static final String defaultHostname = "localhost";
    private static final int defaultPort = 12345;

    private String hostname;
    private int port;

    public void setAddress(String hostname, int port) {
        this.hostname = hostname;
        this.port = port;
    }

    public ChatDatabase getDatabase() {
        return database;
    }

    public void start(String name, int maxClientsNumber) {
        database = new ChatDatabase();
        database.setServerName(name);
        database.setMaxClientsNumber(maxClientsNumber);

        long startingEventNumber = Event.getNextSerialNumber();
        EventSender eventSender = new EventSender(startingEventNumber);
        executorService = Executors.newCachedThreadPool();
        executorService.submit(eventSender);

        try {
            serverSocket = new ServerSocket();
            if (hostname == null) {
                serverSocket.bind(new InetSocketAddress(defaultHostname, defaultPort));
            } else {
                serverSocket.bind(new InetSocketAddress(hostname, port));
            }

            for (;;) {
                Socket socket = serverSocket.accept();
                executorService.submit(new SessionManager(socket, database, eventSender));
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        } finally {
            close();
        }
    }

    public void close() {
        if (executorService != null) {
            executorService.shutdownNow();
        }

        if (serverSocket != null) {
            try {
                serverSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
