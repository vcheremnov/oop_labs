package misc.network;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketException;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;

public class ClientSession {
    private Socket socket;
    private BufferedReader socketReader;
    private PrintWriter socketWriter;

    private AtomicReference<String> sessionID = new AtomicReference<>();
    private AtomicLong startingEventNumber = new AtomicLong(Long.MAX_VALUE);

    public ClientSession(Socket socket) throws IOException {
        this.socket = socket;
        socketReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        socketWriter = new PrintWriter(socket.getOutputStream());
    }

    public String getSessionID() {
        return sessionID.get();
    }

    public void setSessionID(String sessionID) {
        this.sessionID.set(sessionID);
    }

    public long getStartingEventNumber() {
        return startingEventNumber.get();
    }

    public void setStartingEventNumber(long eventSerialNumber) {
        startingEventNumber.set(eventSerialNumber);
    }

    public void writeMessage(String message) throws IOException {
        socketWriter.println(message);
        boolean errorOccurred = socketWriter.checkError();
        if (errorOccurred) {
            throw new SocketException("Error has occurred while writing to the socket");
        }
    }

    public String readMessage() throws IOException {
        String message = socketReader.readLine();
        if (message == null) {
            throw new SocketException("Socket was closed");
        }
        return message;
    }

    public void close() {
        try {
            socket.close();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}
