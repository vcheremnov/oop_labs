package client.model.handlers.responses.types.chatmessage;

import client.model.session.ChatSession;
import client.model.handlers.MessageHandlerException;
import client.model.handlers.responses.ResponseHandler;
import protocol.Message;
import protocol.commands.types.chatmessage.ChatMessageResponseSuccess;

public class ChatMessageResponseHandler extends ResponseHandler {
    @Override
    protected void handleMessageImpl(Message message, ChatSession session) throws MessageHandlerException {
        ChatMessageResponseSuccess response = (ChatMessageResponseSuccess) message;
//        session.notifyObservers();
        // TODO: notify observers!!!
    }

    @Override
    protected Class<? extends Message> getMessageClass() {
        return ChatMessageResponseSuccess.class;
    }
}
