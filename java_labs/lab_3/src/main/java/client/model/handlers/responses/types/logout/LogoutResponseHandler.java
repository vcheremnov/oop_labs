package client.model.handlers.responses.types.logout;

import client.model.session.ChatSession;
import client.model.handlers.MessageHandlerException;
import client.model.handlers.responses.ResponseHandler;
import protocol.Message;
import protocol.commands.types.logout.LogoutResponseSuccess;

public class LogoutResponseHandler extends ResponseHandler {
    @Override
    protected void handleMessageImpl(Message message, ChatSession session) {
        session.notifyObservers(ChatSession.Property.LOGGED_OUT, null, true);
    }

    @Override
    protected Class<? extends Message> getMessageClass() {
        return LogoutResponseSuccess.class;
    }
}
