package client.model.network;

import client.model.handlers.MessageHandlerException;
import client.model.handlers.events.EventHandler;
import client.model.handlers.events.EventHandlerFactory;
import client.model.handlers.responses.ResponseHandler;
import client.model.handlers.responses.ResponseHandlerFactory;
import client.model.session.ChatSession;
import misc.itemfactory.exceptions.ItemFactoryException;
import misc.network.ClientSession;
import misc.parser.MessageParser;
import protocol.MessageType;
import protocol.commands.response.ResponseStatus;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

public class MessageReceiver implements Runnable {
    private ChatSession chatSession;
    private MessageParser messageParser = new MessageParser();
    private Queue<ErrorAction> errorActions = new ConcurrentLinkedQueue<>();

    public MessageReceiver(ChatSession chatSession) {
        this.chatSession = chatSession;
    }

    public void setNextResponseError(ErrorAction errorAction) {
        errorActions.add(errorAction);
    }

    public void run() {
        ClientSession session = chatSession.getSession();
        try {
            while (!Thread.currentThread().isInterrupted()) {
                String message = session.readMessage();
                messageParser.parseMessage(message);
                String messageType = messageParser.getMessageType();

                if (messageType.equals(MessageType.MESSAGE_EVENT.toString())) {
                    handleEvent(message);
                } else if (messageType.equals(MessageType.MESSAGE_RESPONSE.toString())) {
                    handleResponse(message);
                } else {
                    throw new RuntimeException("Received message is neither an event nor a response");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            session.close();
            // TODO: notify observers
        }
    }

    private void handleResponse(String message) throws ItemFactoryException, MessageHandlerException {
        MessageParser.ResponseParser responseParser = messageParser.getResponseParser();
        String responseType = responseParser.getResponseType();
        String responseStatus = responseParser.getResponseStatus();
        String errorReason = responseParser.getErrorReason();

        ErrorAction errorAction = errorActions.poll();
        if (responseStatus.equals(ResponseStatus.RESPONSE_ERROR.toString()) && errorAction != null) {
            // TODO: callback is executed by the message receiver! might be time consuming
            errorAction.handleError(errorReason);
            return;
        }

        ResponseHandlerFactory responseHandlerFactory = ResponseHandlerFactory.getInstance();
        ResponseHandler responseHandler = responseHandlerFactory.getResponseHandler(responseType);
        responseHandler.handleMessage(message, chatSession);
    }

    private void handleEvent(String message) throws ItemFactoryException, MessageHandlerException {
        String eventType = messageParser.getEventParser().getEventType();
        EventHandlerFactory eventHandlerFactory = EventHandlerFactory.getInstance();
        EventHandler eventHandler = eventHandlerFactory.getEventHandler(eventType);
        eventHandler.handleMessage(message, chatSession);
    }
}
