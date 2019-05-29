package client.model.handlers.responses.types.chatmessage;

import client.model.session.ChatSession;
import client.model.handlers.responses.ResponseHandler;
import protocol.Message;
import protocol.commands.types.chatmessage.ChatMessageResponseSuccess;

public class ChatMessageResponseHandler extends ResponseHandler {
    @Override
    protected void handleMessageImpl(Message message, ChatSession session) {
        session.notifyObservers(ChatSession.Property.SENT_MESSAGE, null, true);
    }

    @Override
    protected Class<? extends Message> getMessageClass() {
        return ChatMessageResponseSuccess.class;
    }
}
