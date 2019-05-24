package server.network;

import misc.network.ClientSession;
import misc.parser.MessageParser;
import protocol.MessageType;
import server.data.ChatDatabase;
import server.handlers.RequestHandler;
import server.handlers.RequestHandlerFactory;

import java.net.Socket;

class SessionManager implements Runnable {
    private Socket socket;
    private EventSender eventSender;
    private ChatDatabase database;
    private ClientSession session;

    private MessageParser messageParser = new MessageParser();

    SessionManager(Socket socket, ChatDatabase database, EventSender eventSender) {
        this.socket = socket;
        this.database = database;
        this.eventSender = eventSender;
    }

    @Override
    public void run() {
        try {
            session = new ClientSession(socket);
            eventSender.addSession(session);
            RequestHandlerFactory factory = RequestHandlerFactory.getInstance();

            while (!Thread.currentThread().isInterrupted()) {
                String request = session.readMessage();
                String requestType = getRequestType(request);

                RequestHandler requestHandler = factory.getRequestHandler(requestType, database);
                requestHandler.handleRequest(request, session);

                String event = requestHandler.getEvent();
                if (event != null) {
                    long serialNumber = requestHandler.getEventSerialNumber();
                    eventSender.submitEventMessage(event, serialNumber);
                }

                String response = requestHandler.getResponse();
                session.writeMessage(response);

                if (requestHandler.isTerminateFlagSet()) {
                    break;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            disconnect();
        }
    }

    private String getRequestType(String request) throws Exception {
        messageParser.parseMessage(request);
        String messageType = messageParser.getMessageType();
        if (!messageType.equals(MessageType.MESSAGE_REQUEST.toString())) {
            throw new Exception("Expected request message, received message of type " + messageType);
        }

        return messageParser.getRequestParser().getRequestType();
    }

    private void disconnect() {
        session.close();
        eventSender.removeSession(session);
        DisconnectHandler disconnectHandler = new DisconnectHandler(database);
        disconnectHandler.handleDisconnect(session.getSessionID());

        String logoutEventMessage = disconnectHandler.getEvent();
        if (logoutEventMessage != null) {
            long eventSerialNumber = disconnectHandler.getEventSerialNumber();
            eventSender.submitEventMessage(logoutEventMessage, eventSerialNumber);
        }
    }
}