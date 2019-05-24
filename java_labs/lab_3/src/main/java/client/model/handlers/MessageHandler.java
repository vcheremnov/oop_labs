package client.model.handlers;

import client.model.session.ChatSession;
import com.google.gson.Gson;
import com.google.gson.JsonSyntaxException;
import protocol.Message;

public abstract class MessageHandler<T extends Message> {
    private static final Gson gson = new Gson();

    public final void handleMessage(String message, ChatSession session) throws MessageHandlerException {
        try {
            T messageObject = gson.fromJson(message, getMessageClass());
            if (messageObject == null) {
                throw new MessageHandlerException("Empty message body");
            }

            handleMessageImpl(messageObject, session);
        } catch (JsonSyntaxException ex) {
            throw new MessageHandlerException("Failed to parse message", ex);
        }
    }

    protected abstract void handleMessageImpl(T message, ChatSession session) throws MessageHandlerException;

    protected abstract Class<? extends T> getMessageClass();
}
